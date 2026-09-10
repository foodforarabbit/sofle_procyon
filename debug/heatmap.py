#!/usr/bin/env python3
"""
heatmap.py -- read the raw per-node capacitance map off the sensor.

This is the dead-spot instrument. The delta stream cannot answer "which AREAS
feel bad" because a delta carries no position -- it says how far the finger
moved, never where it was. T37 gives the whole 14x24 matrix node by node, so a
weak or dead node shows up regardless of how steady anyone's hand is.

    ./debug/heatmap.py                # reference mode, no finger needed
    ./debug/heatmap.py --mode delta   # live signal; put a finger down
    ./debug/heatmap.py --raw          # print numbers instead of the map

PLAN.md filed the heatmap as a side quest needing the upstream maxtouch-debug
GUI and two extra flashes. That was wrong in one respect: the GUI is only one
way to read T37. The object is reachable over the Vial tunnel like any other,
so this costs no flash and no GUI.

HOW IT WORKS. T6's `diagnostic` byte (offset 5) selects a mode and resets the
page to 0; writing T6_DIAGNOSTIC_PAGE_UP advances one page. T37 then holds
[mode, page, 64 x int16] -- 64 nodes per page, so a 14x24 matrix needs 6 pages.
The tunnel caps a read at 27 bytes, so each page takes 5 reads.

Requires hidapi. With the repo venv: tmp/hidvenv/bin/python debug/heatmap.py
"""

import argparse
import importlib.util
import os
import sys
import time

MODES = {
    "delta": 0x10,       # mutual capacitance delta -- live signal, needs a touch
    "reference": 0x11,   # mutual capacitance reference -- baseline, no touch
    "dc": 0x38,
}
PAGE_UP = 0x01

T6_DIAGNOSTIC_OFFSET = 5     # maxtouch.h:54-62, PACKED
NODES_PER_PAGE = 64
RAMP = " .:-=+*#%@"


def load_mxt():
    here = os.path.dirname(os.path.abspath(__file__))
    spec = importlib.util.spec_from_file_location("mxt", os.path.join(here, "mxt.py"))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def read_long(m, dev, addr, length):
    """Read more than the tunnel's 27-byte cap by chunking."""
    out = b""
    while len(out) < length:
        n = min(27, length - len(out))
        out += m.read_bytes(dev, addr + len(out), n)
    return out[:length]


def read_page(m, dev, t37_addr, want_mode, want_page, tries=40):
    """Wait for T37 to publish the page we asked for, then read its 64 nodes."""
    for _ in range(tries):
        hdr = m.read_bytes(dev, t37_addr, 2)
        if hdr[0] == want_mode and hdr[1] == want_page:
            data = read_long(m, dev, t37_addr + 2, NODES_PER_PAGE * 2)
            return [
                int.from_bytes(data[i * 2 : i * 2 + 2], "little", signed=True)
                for i in range(NODES_PER_PAGE)
            ]
        time.sleep(0.02)
    sys.exit(
        f"T37 never published mode {want_mode:#04x} page {want_page} "
        f"(saw mode {hdr[0]:#04x} page {hdr[1]}). "
        "If the sensor is busy, retry; if it never advances, the diagnostic "
        "write is not landing."
    )


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--mode", choices=sorted(MODES), default="reference")
    ap.add_argument("--raw", action="store_true", help="print values, not a map")
    args = ap.parse_args()
    mode = MODES[args.mode]

    m = load_mxt()
    dev = m.open_device()
    try:
        header, objs = m.object_table(dev)
        xs, ys = header[4], header[5]
        for t in (6, 37):
            if t not in objs:
                sys.exit(f"T{t} not present on this device")
        t6, t37 = objs[6]["addr"], objs[37]["addr"]
        total = xs * ys
        pages = (total + NODES_PER_PAGE - 1) // NODES_PER_PAGE

        print(f"matrix {xs} x {ys} = {total} nodes, {pages} pages, mode {args.mode}")
        if args.mode == "delta":
            print("delta mode: values are meaningless without a finger on the pad")

        m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [mode])
        nodes = []
        for p in range(pages):
            if p:
                m.write_bytes(dev, t6 + T6_DIAGNOSTIC_OFFSET, [PAGE_UP])
            nodes += read_page(m, dev, t37, mode, p)
        nodes = nodes[:total]
    finally:
        dev.close()

    if args.raw:
        print(" ".join(str(v) for v in nodes))
        return

    # LAYOUT, measured rather than assumed. The info block reports 14 x 24, but
    # the T37 diagnostic buffer is laid out differently: unused slots carry
    # INT16_MIN, and on this device they fall at positions 10 and 11 of every
    # 12, i.e. 28 blocks of 12 with 10 active each = 280 real nodes. Reshaping
    # to 14 x 24 folds two blocks into one row and scatters the sentinels
    # through the middle of the map, which reads as dead columns that are not
    # there. So derive the stride from where the sentinels actually land.
    SENT = -32768
    sent_idx = [i for i, v in enumerate(nodes) if v == SENT]
    stride = None
    if sent_idx:
        for cand in range(4, 65):
            if len({i % cand for i in sent_idx}) <= max(1, cand // 6) and len(nodes) % cand == 0:
                stride = cand
                break
    if stride is None:
        stride = ys
        print(f"no sentinels found; assuming stride {stride} from the info block")
    else:
        dead = sorted({i % stride for i in sent_idx})
        print(f"detected stride {stride}; slots {dead} unused in every block "
              f"({len(nodes)//stride} blocks x {stride - len(dead)} active "
              f"= {len(nodes)//stride*(stride-len(dead))} nodes)")

    active_slots = [j for j in range(stride) if all(
        nodes[b * stride + j] != SENT for b in range(len(nodes) // stride))]
    blocks = len(nodes) // stride
    grid = [[nodes[b * stride + j] for j in active_slots] for b in range(blocks)]
    flat = [v for row in grid for v in row]

    lo, hi = min(flat), max(flat)
    span = (hi - lo) or 1
    print(f"range {lo} .. {hi}\n")
    print("     " + "".join(f"{j%10}" for j in range(len(active_slots))))
    for b, row in enumerate(grid):
        cells = "".join(RAMP[min(int((v - lo) / span * (len(RAMP) - 1)), len(RAMP) - 1)] for v in row)
        print(f" {b:02d}  {cells}")
    print(f"\n  legend  low [{RAMP}] high")

    # A weak or dead node sits far from the median of the whole surface. A
    # smooth gradient toward the edges is normal; an isolated outlier is not.
    srt = sorted(flat)
    med = srt[len(srt) // 2]
    mad = sorted(abs(v - med) for v in flat)[len(flat) // 2] or 1
    bad = [(b, j, grid[b][j]) for b in range(blocks) for j in range(len(active_slots))
           if abs(grid[b][j] - med) > 6 * mad]
    print(f"\nmedian {med}, MAD {mad}   (spread {(hi-lo)/mad:.0f}x MAD)")
    print(f"nodes beyond 6x MAD: {len(bad)} of {len(flat)}")
    for b, j, v in bad[:25]:
        print(f"  block {b:02d} slot {j:02d}  {v:7d}   ({(v-med)/mad:+.1f} MAD)")
    if len(bad) > 25:
        print(f"  ... {len(bad)-25} more")


if __name__ == "__main__":
    main()
