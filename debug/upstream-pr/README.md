# Upstream PRs for george-norton — THREE findings

**Ryan opens these, not an agent.** They go to `george-norton/qmk_firmware`,
someone else's repo, under his GitHub identity. **Needs a fork of
george-norton first** — different fork network from `foodforarabbit/vial-qmk`.

| # | finding | severity | status |
|---|---|---|---|
| 1 | debug-protocol READ/WRITE bounds check | overread on a short report | patch below, written 2026-09-08 |
| 2 | debug-protocol register address not byte-swapped | debug R/W broken at every address except 0 | MEASURED on hardware 2026-09-10 |
| 3 | `BUILD_ID` randomised per build | wipes every Vial user's layout on every flash | fix implemented + measured, see below |

Findings 2 and 3 were found while using the debug protocol for real trackpad
tuning, which is how they surfaced at all: 1 is a code-reading defect, 2 and 3
only show up when you actually drive the thing.

Suggest sending them as separate PRs — 1 and 2 both touch `maxtouch.c` and could
share one, 3 is unrelated (`util/build_id.py`) and stands alone. 3 is the one
with the widest blast radius: it affects every Vial user of the fork, not just
Procyon owners.

---

## Finding 1 — bounds check

## Why bother

`MAXTOUCH_DEBUG_READ` / `_WRITE` validate the caller's requested length only
against the protocol maximum, never against the actual report buffer:

```c
if (read_length > 0x1c) {          // upstream, maxtouch.c:668 and :680
```

A host sending a short raw-HID report with a large `length` byte makes the
driver read or write past the end of `data`. Two lines fix it. Confirmed still
present on `multitouch_experiment` as of 2026-09-08 (MEASURED).

Worth sending rather than carrying: if he takes it, we inherit it on any future
merge instead of maintaining a fork delta — which is the whole point of keeping
vial-qmk close to upstream.

## The patch

`drivers/sensors/maxtouch.c`, inside `#ifdef MAXTOUCH_DEBUG`:

```diff
         case MAXTOUCH_DEBUG_READ: {
             const uint16_t read_address = (data[1] << 8) | data[2];
             const uint16_t read_length  = data[3];
-            if (read_length > 0x1c) {
+            if (read_length > 0x1c || read_length + 4 > length) {
                 status = MAXTOUCH_DEBUG_INVALID_LENGTH;
```

```diff
         case MAXTOUCH_DEBUG_WRITE: {
             const uint16_t write_address = (data[1] << 8) | data[2];
             const uint16_t write_length  = data[3];
-            if (write_length > 0x1c) {
+            if (write_length > 0x1c || write_length + 4 > length) {
                 status = MAXTOUCH_DEBUG_INVALID_LENGTH;
```

## Why you need a fork first

GitHub only allows PRs within one fork network, and these are two:

| repo | fork source |
|---|---|
| `foodforarabbit/vial-qmk` | `vial-kb/vial-qmk` |
| `george-norton/qmk_firmware` | `qmk/qmk_firmware` |

So a branch on our vial-qmk **cannot** be proposed to his repo. You need a fork
of *his* repo, which lands in the qmk network.

## Steps

```sh
# 1. Fork his repo (puts it in the qmk fork network)
gh repo fork george-norton/qmk_firmware --clone=false --remote=false

# 2. Branch from his feature branch, not master (master is a dead 2024 snapshot)
cd /tmp && git clone --depth 1 --branch multitouch_experiment \
  https://github.com/george-norton/qmk_firmware.git gn-qmk
cd gn-qmk
git checkout -b maxtouch-debug-bounds-check

# 3. Apply the two-line change above to drivers/sensors/maxtouch.c

# 4. Push to YOUR fork
git remote add mine git@github.com:foodforarabbit/qmk_firmware.git
git push -u mine maxtouch-debug-bounds-check
```

Then open the PR — this URL prefills it once the branch exists:

<https://github.com/george-norton/qmk_firmware/compare/multitouch_experiment...foodforarabbit:maxtouch-debug-bounds-check?expand=1>

**Base must be `multitouch_experiment`.** His `master` last moved 2024-01-23 and
the debug protocol does not exist there.

## Suggested PR text

> **Title:** maxtouch: bounds-check debug read/write against the report buffer
>
> `MAXTOUCH_DEBUG_READ` and `MAXTOUCH_DEBUG_WRITE` validate the requested
> length against the protocol maximum (`0x1c`) but not against the actual
> `length` of the received report, so a host sending a short report with a
> large length byte causes an out-of-bounds read or write on `data`.
>
> Adds `+ 4 > length` to both checks — 4 being the header the payload starts
> after. Behaviour is unchanged for well-formed requests.
>
> Found while building a host-side tuning script against this protocol for a
> Procyon trackpad; the short-report case is easy to hit by accident when the
> host packs its own reports.


---

## Finding 2 — the debug register address is not byte-swapped

**MEASURED on hardware, 2026-09-10.** A debug READ at address `0` works; a READ
at ANY non-zero address returns all zeros with status `OK`. Zero is the one
byte-order-agnostic address, which is what gives the cause away.

`maxtouch.c:643` builds the address and passes it straight through:

```c
case MAXTOUCH_DEBUG_READ: {
    const uint16_t read_address = (data[1] << 8) | data[2];
    ...
    i2c_read_register16(MXT336UD_ADDRESS, read_address, &data[4], read_length, ...)
```

But the same file, for the identical call during init, wraps it:

```c
// maxtouch.c:220
i2c_read_register16(MXT336UD_ADDRESS, SWAP_BYTES(object_table_element_address), ...)
```

maXTouch wants the register pointer little-endian; `i2c_read_register16`
transmits big-endian. So the swap is required, and the debug path omits it on
**both** READ (`:643`) and WRITE (`:660`). The write path is the more dangerous
one: a wrong address there scribbles into an unrelated register.

### Proof

With the address pre-swapped host-side, a 6-byte read at `0x0700` returns exactly
the bytes a 27-byte read from address `0` shows sitting at offset 7 — i.e. the
first object-table entry:

```
read(0x0000, 27) -> a6 1c 20 aa 0e 18 22 | 02 d6 00 08 00 00 ...
read(0x0007,  6) -> 00 00 00 00 00 00        <- wrong, silently
read(0x0700,  6) -> 02 d6 00 08 00 00        <- correct
```

### Fix

```c
const uint16_t read_address  = SWAP_BYTES((data[1] << 8) | data[2]);   // :643
const uint16_t write_address = SWAP_BYTES((data[1] << 8) | data[2]);   // :660
```

Note this is technically a behaviour change for any existing host tool that had
already compensated for it. The upstream `maxtouch-debug` GUI speaks the
unswapped protocol, so it presumably has the same bug and reads the wrong
registers too — worth mentioning in the PR so george-norton can decide whether
to fix the firmware, the GUI, or both.

---

## Finding 3 — `BUILD_ID` is randomised, so every build wipes the user layout

**Widest blast radius of the three: affects every Vial user of the fork.**

`via_eeprom_is_valid()` (`quantum/via.c:96-106`, under `VIAL_ENABLE`) compares the
stored VIA/Vial EEPROM magic against `BUILD_ID`:

```c
uint8_t magic0 = BUILD_ID & 0xFF;
uint8_t magic1 = (BUILD_ID >> 8) & 0xFF;
uint8_t magic2 = (BUILD_ID >> 16) & 0xFF;
```

And `util/build_id.py` generates it randomly on every build:

```python
print("#define BUILD_ID ((uint32_t)0x{:08X})".format(random.randrange(0, 2 ** 24 - 1)))
```

So an identical rebuild — same source, no layout change whatsoever — produces a
different magic, fails the validity check, and Vial wipes the user's keymap.

**Observed across six consecutive flashes on a Sofle Procyon**, including flashes
that changed neither `vial.json` nor the eeconfig layout. Restoring a saved
layout and reflashing loses it again immediately.

### Fix (implemented, `foodforarabbit/vial-qmk` branch `procyon-tap-scroll-fix`)

Honour an env var, keep random as the default so nothing changes for anyone not
opting in:

```python
pinned = os.environ.get("VIAL_BUILD_ID")
value = int(pinned, 0) & 0xFFFFFF if pinned else random.randrange(0, 2 ** 24 - 1)
```

Measured: pinned gives `0x0050F1E1` on repeated runs; unset still gives fresh
random values (`0x008B49C9`, `0x00B48E0A`).

Pinning changes the semantics from "differs on every build" to "differs when the
maintainer says the layout changed", which is what the validity check actually
wants. Bump it deliberately when `vial.json`, the matrix, or
`EECONFIG_USER_DATA_SIZE` change.

### Caveat worth stating in the PR

There is a second, independent reason a split-keyboard layout appears to reset,
and it is NOT a bug: each half has its own EEPROM and VIA/Vial writes only to the
USB-master half (no `EE_HANDS`, no mirroring in `split_common`). Restoring while
attached to one half and reconnecting via the other reads as a wipe. Mentioning
it pre-empts the obvious "are you sure that is the cause" response — the
`BUILD_ID` randomisation is separately and independently sufficient, and the
measurement above isolates it.
