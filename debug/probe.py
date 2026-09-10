#!/usr/bin/env python3
"""
probe.py -- where is the finger, and how strongly does the pad see it?

Reads T37 in mutual-capacitance DELTA mode and reports the peak node plus its
signal strength. Two jobs:

  1. CALIBRATION. The heatmap's block/slot axes are not labelled to the physical
     pad. Hold a finger in a known spot, run this, and the peak node tells us
     which corner is which.

  2. SIGNAL COMPARISON. Once calibrated, holding a finger in a "good" area and
     then a "bad" one gives two peak numbers. If the bad area reads weaker, the
     cause is signal margin (gain / tchthr) rather than anything downstream.

    ./debug/probe.py            # one reading
    ./debug/probe.py -n 5       # five readings, ~2s apart

A full matrix read is 6 pages x 5 chunked tunnel reads, so each reading takes a
second or two. Hold still through it.

Requires hidapi. With the repo venv: tmp/hidvenv/bin/python debug/probe.py
"""

import argparse
import importlib.util
import os
import sys
import time

DELTA_MODE = 0x10
PAGE_UP = 0x01
T6_DIAGNOSTIC_OFFSET = 5
NODES_PER_PAGE = 64
SENT = -32768


def load(name):
    here = os.path.dirname(os.path.abspath(__file__))
    spec = importlib.util.spec_from_file_location(name, os.path.join(here, f"{name}.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


REFERENCE_MODE = 0x11


def read_mode(m, hm, dev, t6, t37, total, pages, mode):
    m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [mode])
    nodes = []
    for p in range(pages):
        if p:
            m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [PAGE_UP])
        nodes += hm.read_page(m, dev, t37, mode, p)
    return nodes[:total]


def active_layout(m, hm, dev, t6, t37, total, pages):
    """Stride and live slots, taken from a REFERENCE read.

    Only reference mode marks unused slots with INT16_MIN. Delta mode fills
    them with noise, so deriving the mask from a delta read finds no sentinels,
    silently keeps 12 slots per block, and reports peaks in slots that are not
    part of the pad at all.
    """
    ref = read_mode(m, hm, dev, t6, t37, total, pages, REFERENCE_MODE)
    sent = [i for i, v in enumerate(ref) if v == SENT]
    stride = 12
    for cand in range(4, 65):
        if sent and len({i % cand for i in sent}) <= max(1, cand // 6) \
                and len(ref) % cand == 0:
            stride = cand
            break
    slots = [j for j in range(stride)
             if all(ref[b * stride + j] != SENT for b in range(len(ref) // stride))]
    return stride, slots


def read_matrix(m, hm, dev, t6, t37, total, pages):
    m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [DELTA_MODE])
    nodes = []
    for p in range(pages):
        if p:
            m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [PAGE_UP])
        nodes += hm.read_page(m, dev, t37, DELTA_MODE, p)
    return nodes[:total]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", type=int, default=1, help="number of readings")
    args = ap.parse_args()

    m, hm = load("mxt"), load("heatmap")
    dev = m.open_device()
    try:
        header, objs = m.object_table(dev)
        xs, ys = header[4], header[5]
        t6, t37 = objs[6]["addr"], objs[37]["addr"]
        total = xs * ys
        pages = (total + NODES_PER_PAGE - 1) // NODES_PER_PAGE

        stride, slots = active_layout(m, hm, dev, t6, t37, total, pages)
        print(f"layout: {total // stride} blocks x {len(slots)} active slots")

        for k in range(args.n):
            if k:
                time.sleep(2)
            nodes = read_matrix(m, hm, dev, t6, t37, total, pages)

            blocks = len(nodes) // stride
            grid = [[nodes[b * stride + j] for j in slots] for b in range(blocks)]

            flat = [(abs(v), b, j, v) for b, row in enumerate(grid)
                    for j, v in enumerate(row)]
            flat.sort(reverse=True)
            peak = flat[0]
            noise = sorted(a for a, _, _, _ in flat)[len(flat) // 2]

            strong_n = sum(1 for a, _, _, _ in flat if a > peak[0] * 0.4)
            if peak[0] < max(30, noise * 8) or strong_n > 20:
                print(f"[{k+1}/{args.n}] no touch detected "
                      f"(peak |delta| {peak[0]}, noise {noise}, "
                      f"{strong_n} nodes near peak — a real touch is big AND compact)")
                continue

            print(f"[{k+1}/{args.n}] peak at block {peak[1]:02d} slot {peak[2]:02d}"
                  f"   delta {peak[3]:+d}   (noise floor {noise})")
            print("      top 5 nodes: " + ", ".join(
                f"b{b:02d}s{j:02d}={v:+d}" for _, b, j, v in flat[:5]))
            # Contact spread says how much of the pad the finger lights up; a
            # weak region shows a smaller, dimmer footprint for the same finger.
            strong = [(b, j) for a, b, j, _ in flat if a > peak[0] * 0.4]
            bs = [b for b, _ in strong]
            js = [j for _, j in strong]
            print(f"      footprint: {len(strong)} nodes above 40% of peak, "
                  f"blocks {min(bs)}-{max(bs)}, slots {min(js)}-{max(js)}")
    finally:
        dev.close()


if __name__ == "__main__":
    main()
