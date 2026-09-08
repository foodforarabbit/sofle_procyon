# Tuning strategy — spend flashes on the instrument, not on guesses

Written for someone with no prior context on this repo. The goal is to minimise
reflashes, so the first question is not "what value should I try" but **"which
of the three layers is even responsible?"**

## The three layers, and which ones need a flash

A finger movement passes through three places where it can be mangled:

```
MaxTouch IC  ──►  driver / gesture state machine  ──►  our keymap curve  ──►  host
(I2C registers)   (maxtouch.c, digitizer.c)           (keymap.c)
```

| layer | example params | changeable at runtime? |
|---|---|---|
| **Sensor IC** | `MXT_GAIN`, `MXT_TOUCH_HYST`, `MXT_MOVE_HYSTERESIS_*`, `MXT_LOW_PASS_FILTER_COEFFICIENT`, acquisition interval | **NO — flash required.** Written to I2C registers at init. The driver keeps the object-table addresses (`t8_acquisitionconfig_address` etc.) as `static` globals and exposes **no setters** in `maxtouch.h`, so nothing outside `maxtouch.c` can rewrite them. |
| **Driver / gestures** | tap timing, scroll divisor defaults | flash (it is upstream code) |
| **Our keymap** | `TRACKPAD_JITTER_THRESHOLD`, `SLOW_LIMIT`, `SLOW_SPEED`, scroll divisor | **YES, if we add keycodes.** This is plan items 6–8. |

**So "stop reflashing" splits in two:** keymap params can become live knobs with
one flash; sensor params cost one flash per change no matter what. That asymmetry
drives the whole plan below.

## About `maxtouch-debug` — read this before relying on it

`george-norton/maxtouch-debug` is a Tauri app that **visualises raw sensor
data**, and its README names peacock and pavonis — the two sibling boards on our
driver — so it was built against hardware close to ours.

**It does NOT currently write registers.** Its README says it *"will enable you
to change register values and inspect the results"* — future tense, explicitly a
work in progress. Verified 2026-09-08.

So it is a **diagnosis** tool, not a tuning tool. That is still the single most
valuable thing available, because it converts "feels jittery" into "here is the
raw signal", and that tells you which layer to spend a flash on.

## The plan, in flash order

### Flash 1 — build the instrument, fix nothing

`MAXTOUCH_DEBUG = yes` in `rules.mk`, plus the debug keymap the tool needs.
Change no tuning values. Then use maxtouch-debug to answer questions we
currently cannot:

- **S1 (jitter): does the raw signal jitter, or is the raw signal clean and our
  curve is amplifying it?** These have opposite fixes. If raw is noisy →
  sensor-side (`GAIN`, thresholds, low-pass). If raw is clean → it is our binary
  jitter threshold, which is firmware and therefore free to iterate later.
- **Does it jitter while the finger is HELD STILL, or only while moving?** Still
  = noise floor. Moving-only = hysteresis release pattern.
- **S2 (liftoff): watch the last few frames before `MXT_UP`.** The plan asserts
  a shifting contact area produces a bogus final delta. That is currently
  REASONED, not measured. This is where you measure it.

**Why this is first:** every blind parameter guess costs a flash. One flash spent
on seeing the signal can save five spent guessing, and it tells you whether the
expensive layer (sensor) is even involved.

### Flash 2 — make the cheap layer live

Add `CPI_UP/CPI_DN`, `SCR_UP/SCR_DN`, `TP_RST` keycodes + EEPROM persistence
(plan items 6–8), and expose the jitter/slow-zone params. After this flash,
every **firmware-side** knob is tunable by keypress and survives a power cycle.
No further flashes for that layer, ever.

Add them to `vial.json` too (plan item 8) so they are assignable in the VIAL
GUI rather than hardcoded to a layer.

### Flash 3+ — sensor params, in ONE batch, bisected not swept

Each sensor change costs a flash, so do not sweep one variable at a time. Use
the bracket from `REFERENCE-PRESETS.md`: both sibling boards run **lower**
hysteresis than we inherit (8/4 and 6/3 vs our 10/5), and **both** enable
free-run acquisition (`MXT_ACTIVE_ACQUISITION_INTERVAL 255`) which we do not.

Recommended first batch, as one flash:

```c
#define MXT_ACTIVE_ACQUISITION_INTERVAL 255  // free-run, ~300Hz
#define MXT_ACTIVE_SYNCS_PER_X          20
#define MXT_TOUCH_HYST                   8   // was 10 (peacock)
#define MXT_INTERNAL_TOUCH_HYST          4   // was 5  (peacock)
```

Rationale: free-run gives smaller, more frequent deltas, which is mechanically
the opposite of stick-then-jump; and the hysteresis pair is corroborated by two
independently tuned boards. If that batch helps, bisect within it to find which
part mattered. If it makes S1 worse, that is informative — it means our jitter
is noise the hysteresis was hiding, and the fix belongs at `GAIN`/threshold or
in a low-pass instead.

**Try `MXT_LOW_PASS_FILTER_COEFFICIENT 10` (pavonis) BEFORE writing the
firmware-side EMA the plan proposes.** Sensor-side filtering happens *before*
the hysteresis logic rather than after, and costs zero firmware complexity. The
plan was written without knowing this parameter existed.

### Before any of it — one hour of reading

Check upstream for our exact symptoms first (`UPSTREAMS.md` has the search
list). S2 is a generic capacitive-trackpad problem and pavonis is a **shipping
product** on this driver, so someone has very likely already hit and fixed it.
Inheriting a fix beats writing one.

## What NOT to do

- **Don't tune by feel without recording the build.** See `README.md`.
- **Don't change sensor and firmware params in the same flash.** You lose the
  ability to attribute the result, and sensor changes are the expensive ones.
- **Don't copy peacock/pavonis wholesale.** Their pads are different sizes
  (91×156 and 156×99 mm) and their `GAIN` values disagree with each other
  (5 vs 11), so there is no single right answer to inherit — treat them as a
  range and a direction.
- **Don't write the 1-frame liftoff buffer until you have measured the liftoff
  frames.** If the bogus delta is not there, the buffer adds a frame of latency
  to every gesture for nothing.

---

# CORRECTION 2026-09-08 — sensor params ARE runtime-writable

Everything above about "sensor params need a flash" is **WRONG**, and the
correction is the best news in this document. Measured by reading
`vial-qmk/drivers/sensors/maxtouch.c:590-675`.

With `MAXTOUCH_DEBUG = yes` the firmware exposes a **raw-HID protocol** with:

| opcode | what it does |
|---|---|
| `MAXTOUCH_DEBUG_READ` | `i2c_read_register16` at **any** address, up to 0x1c bytes |
| `MAXTOUCH_DEBUG_WRITE` | `i2c_write_register16` at **any** address ← **live register writes** |
| `MAXTOUCH_DEBUG_SET_MOUSE_MODE` | toggle mouse reports at runtime |
| `MAXTOUCH_DEBUG_REBOOT_BOOTLOADER` | enter bootloader over USB, no reset button |

So **one flash buys live read/write of every sensor register over USB.** No
further flashes for sensor tuning at all. The `maxtouch-debug` README describes
register writing as "planned", but the **firmware side is already implemented** —
if the GUI lacks a write field, the protocol can be driven by a ~50-line script
over raw HID.

## THE BLOCKER THIS CREATES — resolve before flashing anything

`maxtouch.c:605` defines **`raw_hid_receive`**. Vial **also** owns
`raw_hid_receive`, and our vial keymap has `VIA_ENABLE = yes` +
`VIAL_ENABLE = yes`.

Two consumers, one callback. So `MAXTOUCH_DEBUG = yes` on the vial keymap will
either fail to link (duplicate symbol) or one will silently hijack the other.

**This is exactly the constraint Ryan set** — the keyboard must stay usable and
keymapping should be drivable from Vial while tuning. If enabling the debug
protocol kills Vial, we lose remapping; if we keep Vial, we may lose live
tuning. **Establishing which, and whether they can coexist, is the first task.**

Candidate resolutions, in order of preference — all UNVERIFIED:

1. **Route the maxtouch protocol through Vial's custom-command channel**
   (`via_custom_value_command` / Vial's raw-HID extension) instead of claiming
   `raw_hid_receive`. Best outcome: both work, one firmware, zero compromise.
2. **Check whether Vial's `raw_hid_receive` is weak/overridable**, or whether it
   forwards unrecognised report IDs to a user hook. If it forwards, we may be
   able to co-exist with a small patch.
3. **Two keymaps**: `vial` (normal use) and `debug` (tuning). Costs a flash to
   switch, but the debug build can still carry a full working keymap — the
   keyboard stays typeable either way. Fallback, not the goal.
4. Worst case: accept a tuning firmware where Vial is unavailable, and do
   remapping in a separate session.

## What this means for key mapping

Vial custom keycodes are declared in `keymaps/vial/vial.json` under
`customKeycodes`. **Ours declares NONE today** (verified: the file has only
name / vendorId / productId / matrix / layouts).

The important property: **adding a keycode needs a flash; ASSIGNING one does
not.** Once a keycode is compiled in and declared in `vial.json`, Vial can bind
it to any key, any layer, live.

So the one flash that adds runtime keycodes should declare **generously** —
every knob we might want (CPI up/down, scroll up/down, jitter threshold
up/down, smoothing up/down, reset, dump-current-values) — because an unused
declared keycode costs nothing but a missing one costs a flash.
