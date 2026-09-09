#!/usr/bin/env python3
"""
mxt.py -- live maXTouch register access over the Vial raw-HID tunnel.

This is the instrument PLAN.md's tuning phase runs on. After flash 1, every
sensor register is readable AND writable over USB while Vial keeps working, so
sensor tuning costs zero further flashes.

    ./mxt.py check-version              # is the tunnel alive?
    ./mxt.py objects                    # dump the object table (do this first)
    ./mxt.py read  0x0000 7             # read N bytes at an address
    ./mxt.py read-obj T100 0 30         # read within an object, by type
    ./mxt.py write 0x01A2 0x08 0x04     # write bytes at an address
    ./mxt.py write-obj T100 26 8 4      # write at object type + offset
    ./mxt.py mouse-mode on|off          # recovery: TP_MOUSE_TOG on the board
    ./mxt.py reboot-bootloader          # enter UF2 mode without the button

TWO THINGS THAT WILL BITE YOU.

1. CLOSE THE VIAL APP FIRST. Both it and this script listen on the same raw-HID
   endpoint and replies broadcast to every listener, so a concurrent Vial will
   eat responses and you will misread it as a dead tunnel. (REASONED.)

2. LIVE WRITES DO NOT SURVIVE A POWER CYCLE. The driver rewrites sensor config
   from compiled values at every boot (MEASURED, driver init). Unplug/replug is
   therefore a universal undo -- but it also means a value you liked is GONE
   unless you wrote it into a session file. Log as you go; bake at flash 2.

Requires: pip install hidapi
"""

import sys


VID, PID = 0xAAC6, 0xBAC6
USAGE_PAGE, USAGE = 0xFF60, 0x61  # the Vial raw interface, same one the GUI uses

REPORT_LEN = 32
PREFIX = 0x4D  # 'M' -- free because VIA claims 0x01-0x13, 0xFE, 0xFF

CMD_CHECK_VERSION, CMD_COMMAND, CMD_READ, CMD_WRITE = 0x00, 0x01, 0x02, 0x03
SUB_REBOOT_BOOTLOADER, SUB_SET_MOUSE_MODE, SUB_GET_MOUSE_MODE = 0x00, 0x01, 0x02

MAGIC, VERSION = 0x9A4D, 0x0001

# The prefix byte eats one, so payloads cap at 27 rather than the native 28.
MAX_PAYLOAD = 0x1B

STATUS = {
    0: "OK",
    1: "INVALID_VERSION",
    2: "INVALID_CMD",
    3: "INVALID_LENGTH",
    4: "I2C_ERROR",
}

# Objects worth knowing by name. T100 is where the thresholds, gain and both
# hysteresis values live; T65 is the low-pass filter; T8 is acquisition.
KNOWN = {
    8: "T8 acquisition",
    42: "T42 touch suppression",
    65: "T65 lens bending / low-pass",
    80: "T80 retransmission",
    100: "T100 multi-touch",
}


def open_device():
    """Pick the raw-HID interface, not the keyboard one."""
    # Imported here, not at module scope, so `--help` works without the dep.
    try:
        import hid
    except ImportError:
        sys.exit("need hidapi: pip install hidapi")

    candidates = [
        d
        for d in hid.enumerate(VID, PID)
        if d.get("usage_page") == USAGE_PAGE and d.get("usage") == USAGE
    ]
    if not candidates:
        # Linux hidraw often reports usage_page/usage as 0; fall back to the
        # highest interface number, which is where the raw endpoint sits.
        all_ifaces = hid.enumerate(VID, PID)
        if not all_ifaces:
            sys.exit(
                f"no device at {VID:#06x}:{PID:#06x} -- is the keyboard plugged in?"
            )
        candidates = [max(all_ifaces, key=lambda d: d.get("interface_number", 0))]
        print(
            "warning: could not match usage page; guessing the highest interface",
            file=sys.stderr,
        )

    dev = hid.device()
    dev.open_path(candidates[0]["path"])
    dev.set_nonblocking(0)
    return dev


def request(dev, payload, timeout_ms=1000):
    """Send [PREFIX, *payload] padded to 32 bytes; return the reply."""
    if len(payload) + 1 > REPORT_LEN:
        sys.exit(f"payload too long: {len(payload)} + prefix > {REPORT_LEN}")

    report = bytes([PREFIX]) + bytes(payload)
    report += b"\x00" * (REPORT_LEN - len(report))
    dev.write(report)

    reply = dev.read(REPORT_LEN, timeout_ms)
    if not reply:
        sys.exit("no reply -- is the Vial app open? close it and retry")
    if reply[0] != PREFIX:
        sys.exit(f"reply is not ours: byte0={reply[0]:#04x}, expected {PREFIX:#04x}")

    status = reply[1]
    if status != 0:
        sys.exit(f"device returned {STATUS.get(status, status)}")
    return bytes(reply)


def cmd_check_version(dev, _args):
    request(dev, [CMD_CHECK_VERSION, MAGIC >> 8, MAGIC & 0xFF, VERSION >> 8, VERSION & 0xFF])
    print("OK -- tunnel alive, magic and version accepted")


def read_bytes(dev, addr, length):
    if not 1 <= length <= MAX_PAYLOAD:
        sys.exit(f"length must be 1..{MAX_PAYLOAD} through the tunnel (got {length})")
    reply = request(dev, [CMD_READ, addr >> 8, addr & 0xFF, length])
    return reply[5 : 5 + length]  # payload starts after prefix+status+addr+len


def write_bytes(dev, addr, data):
    if not 1 <= len(data) <= MAX_PAYLOAD:
        sys.exit(f"can write 1..{MAX_PAYLOAD} bytes through the tunnel")
    request(dev, [CMD_WRITE, addr >> 8, addr & 0xFF, len(data), *data])


def object_table(dev):
    """
    Read the info block, then the object table.

    Register addresses are PER DEVICE -- they are not stable across parts and
    must never be hardcoded. Header is 7 bytes ending in num_objects, then
    num_objects entries of 6 bytes: type, addr_lsb, addr_msb, size-1,
    instances-1, report_ids. (Layout REASONED from the datasheet; it mirrors
    what maxtouch.c does at init.)
    """
    header = read_bytes(dev, 0x0000, 7)
    num_objects = header[6]

    objects = {}
    for i in range(num_objects):
        e = read_bytes(dev, 7 + i * 6, 6)
        objects[e[0]] = {
            "addr": e[1] | (e[2] << 8),
            "size": e[3] + 1,
            "instances": e[4] + 1,
            "report_ids": e[5],
        }
    return header, objects


def resolve(name):
    """'T100' or '100' -> 100."""
    n = name[1:] if name[:1].upper() == "T" else name
    if not n.isdigit():
        sys.exit(f"bad object type {name!r} -- use e.g. T100 or 100")
    return int(n)


def cmd_objects(dev, _args):
    header, objects = object_table(dev)
    fam, variant, ver, build = header[0], header[1], header[2], header[3]
    print(f"family {fam:#04x} variant {variant:#04x} fw {ver >> 4}.{ver & 0xF} build {build:#04x}")
    print(f"{len(objects)} objects\n")
    print(f"{'type':>6}  {'addr':>6}  {'size':>5}  {'inst':>4}  name")
    for t in sorted(objects):
        o = objects[t]
        print(
            f"  T{t:<4} {o['addr']:#06x}  {o['size']:>5}  {o['instances']:>4}  "
            f"{KNOWN.get(t, '')}"
        )


def cmd_read(dev, args):
    if len(args) != 2:
        sys.exit("usage: read <addr> <len>")
    addr, length = int(args[0], 0), int(args[1], 0)
    data = read_bytes(dev, addr, length)
    print(f"{addr:#06x}: " + " ".join(f"{b:02x}" for b in data))


def cmd_write(dev, args):
    if len(args) < 2:
        sys.exit("usage: write <addr> <byte> [byte...]")
    addr = int(args[0], 0)
    data = [int(a, 0) for a in args[1:]]
    write_bytes(dev, addr, data)
    print(f"wrote {len(data)} byte(s) at {addr:#06x} -- volatile, gone at next power cycle")


def cmd_read_obj(dev, args):
    if len(args) != 3:
        sys.exit("usage: read-obj <T-type> <offset> <len>")
    t, off, length = resolve(args[0]), int(args[1], 0), int(args[2], 0)
    _, objects = object_table(dev)
    if t not in objects:
        sys.exit(f"T{t} not present on this device -- run `objects`")
    o = objects[t]
    if off + length > o["size"]:
        sys.exit(f"T{t} is {o['size']} bytes; offset {off} + len {length} overruns it")
    data = read_bytes(dev, o["addr"] + off, length)
    print(f"T{t}+{off} ({o['addr'] + off:#06x}): " + " ".join(f"{b:02x}" for b in data))


def cmd_write_obj(dev, args):
    if len(args) < 3:
        sys.exit("usage: write-obj <T-type> <offset> <byte> [byte...]")
    t, off = resolve(args[0]), int(args[1], 0)
    data = [int(a, 0) for a in args[2:]]
    _, objects = object_table(dev)
    if t not in objects:
        sys.exit(f"T{t} not present on this device -- run `objects`")
    o = objects[t]
    if off + len(data) > o["size"]:
        sys.exit(f"T{t} is {o['size']} bytes; offset {off} + {len(data)} overruns it")
    write_bytes(dev, o["addr"] + off, data)
    print(
        f"T{t}+{off} <- " + " ".join(f"{b:02x}" for b in data)
        + " -- volatile, gone at next power cycle"
    )


def cmd_mouse_mode(dev, args):
    if not args:
        reply = request(dev, [CMD_COMMAND, SUB_GET_MOUSE_MODE])
        print("mouse reports:", "on" if reply[2] else "off")
        return
    if args[0] not in ("on", "off"):
        sys.exit("usage: mouse-mode [on|off]")
    request(dev, [CMD_COMMAND, SUB_SET_MOUSE_MODE, 1 if args[0] == "on" else 0])
    print(f"mouse reports {args[0]}")


def cmd_reboot_bootloader(dev, _args):
    # The board resets, so there is no reply to wait for.
    report = bytes([PREFIX, CMD_COMMAND, SUB_REBOOT_BOOTLOADER])
    dev.write(report + b"\x00" * (REPORT_LEN - len(report)))
    print("reset -- the UF2 drive should mount shortly")


COMMANDS = {
    "check-version": cmd_check_version,
    "objects": cmd_objects,
    "read": cmd_read,
    "write": cmd_write,
    "read-obj": cmd_read_obj,
    "write-obj": cmd_write_obj,
    "mouse-mode": cmd_mouse_mode,
    "reboot-bootloader": cmd_reboot_bootloader,
}


def main():
    if len(sys.argv) < 2 or sys.argv[1] in ("-h", "--help", "help"):
        print(__doc__.strip())
        return
    name = sys.argv[1]
    if name not in COMMANDS:
        sys.exit(f"unknown command {name!r}; try --help")

    dev = open_device()
    try:
        COMMANDS[name](dev, sys.argv[2:])
    finally:
        dev.close()


if __name__ == "__main__":
    main()
