#!/usr/bin/env python3
"""
console.py -- read the QMK console off the keyboard.

Stands in for `qmk console`, which on this mac cannot load its native hidapi
(`libhidapi.dylib` missing) even though the CLI itself is installed. The python
hidapi wheel bundles its own copy, so this works with no system-level install.

    ./debug/console.py                    # stream to stdout
    ./debug/console.py > tmp/console.log  # capture for later reading

Each keymap line looks like:

    tp raw=3,-1 out=0,0 hv=0,0

raw = the delta straight off the sensor, out = after jitter gate, curve,
smoothing and the liftoff holdback, hv = scroll after division. The pair
(raw -> out) is the whole point: it says whether a movement was dropped by the
gate, scaled by the curve, or passed through.

Requires hidapi. If you have the repo venv: tmp/hidvenv/bin/python debug/console.py
"""

import sys
import time

CONSOLE_USAGE_PAGE = 0xFF31
CONSOLE_USAGE = 0x74
VID, PID = 0xAAC6, 0xBAC6


def main():
    try:
        import hid
    except ImportError:
        sys.exit("need hidapi: pip install hidapi")

    ifaces = [
        d
        for d in hid.enumerate(VID, PID)
        if d.get("usage_page") == CONSOLE_USAGE_PAGE and d.get("usage") == CONSOLE_USAGE
    ]
    if not ifaces:
        sys.exit(
            "no console interface found.\n"
            "  CONSOLE_ENABLE = yes must be in keymaps/vial/rules.mk, and the\n"
            "  firmware carrying it must actually be flashed."
        )

    dev = hid.device()
    dev.open_path(ifaces[0]["path"])
    dev.set_nonblocking(1)

    t0 = time.monotonic()
    print(f"# console attached  {time.strftime('%H:%M:%S')}", flush=True)
    print("# move a finger on the trackpad. Ctrl-C to stop.", flush=True)

    # Reports arrive as fixed-size chunks that do not respect line boundaries,
    # so buffer and split on newlines rather than printing per report.
    buf = ""
    try:
        while True:
            data = dev.read(32)
            if not data:
                time.sleep(0.005)
                continue
            buf += bytes(data).split(b"\x00")[0].decode("utf-8", "replace")
            while "\n" in buf:
                line, buf = buf.split("\n", 1)
                if line.strip():
                    print(f"{(time.monotonic()-t0)*1000:9.1f} {line}", flush=True)
    except KeyboardInterrupt:
        pass
    finally:
        if buf.strip():
            print(f"{(time.monotonic()-t0)*1000:9.1f} {buf}", flush=True)
        dev.close()


if __name__ == "__main__":
    main()
