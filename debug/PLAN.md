# Trackpad tuning — executable plan, ordered by flash count

Written 2026-09-08. Supersedes the flash sequence in `STRATEGY.md` (kept for
rationale; its CORRECTION sections point here). Every claim below is labelled
MEASURED (someone ran it or read the source) or REASONED (inference).

**The headline: after ONE flash, no further flashing is needed to tune
anything.** Sensor registers become live-writable over USB, keymap curve
parameters become live-adjustable from keys you assign in the Vial GUI, and
Vial keeps working the whole time. A second flash bakes in the winning values.

---

## What is already established (do not re-verify)

- **MEASURED (build):** `MAXTOUCH_DEBUG = yes` on the vial keymap **fails to
  link** — `multiple definition of 'raw_hid_receive'` between `via.o` and
  `maxtouch.o`. The same build without the flag links clean. So the stock
  driver's debug protocol and VIA/Vial are mutually exclusive as shipped.
- **MEASURED (build):** with the tunnel patch below, the vial keymap builds
  **with** `MAXTOUCH_DEBUG=yes` and produces a working `.uf2`
  (`foodforarabbit_sofle_procyon_wt_vial.uf2`). Vial and live sensor tuning
  coexist in one firmware.
- **MEASURED (source, `vial-qmk/quantum/via.c:219-465`):** VIA's
  `raw_hid_receive` forwards any command id it does not recognise to the weak
  hook `raw_hid_receive_kb()` and then echoes the buffer back to the host.
  VIA's own ids are 0x01–0x13, 0xFE (Vial), 0xFF (unhandled) — so a prefixed
  packet under an unused id passes through untouched. This fork has **no**
  `via_custom_value_command` channel (that is VIA protocol 0x0C; this fork is
  0x0009, `via.h:42`), so `raw_hid_receive_kb` is the extension point.
- **MEASURED (source):** the maxtouch debug opcodes are 0x00–0x03; 0x01–0x03
  collide with VIA's own ids, which is why the protocol must be wrapped in a
  prefix byte rather than passed through raw.
- **MEASURED (web, 2026-09-08):** no upstream fix exists for S1 or S2 — see
  `UPSTREAMS.md` for the full search log. We are writing the first fix, not
  inheriting one.
- **MEASURED (source, `drivers/sensors/procyon.h`):** free-run acquisition
  (`MXT_ACTIVE_ACQUISITION_INTERVAL 255`, ~300 Hz) is **already enabled** on
  our board. Earlier docs said otherwise; corrected in `BASELINE.md` and
  `REFERENCE-PRESETS.md`.

## The pieces (already on branches, nothing flashed)

1. **vial-qmk branch `debug-vial-tunnel`** (commit `8989aa3`, on top of
   `vial-procyon`), currently **checked out** in
   `~/foodforarabbit/vial-qmk`. Patches `drivers/sensors/maxtouch.c/.h`:
   the debug packet handler is extracted into `maxtouch_debug_process()`;
   when `VIA_ENABLE` is defined it is exposed as
   `maxtouch_debug_hid_receive()` instead of defining `raw_hid_receive`.
   No-op unless `MAXTOUCH_DEBUG=yes`.
2. **This repo, branch `trackpad-tuning`**: `keymaps/vial/keymap.c` gained a
   `raw_hid_receive_kb()` tunnel — host wraps a debug packet as
   `[0x4D, <packet>]`, reply comes back with the same prefix. Compiled only
   when `MAXTOUCH_DEBUG` and `VIA_ENABLE` are both set.
3. **Build symlink:** `vial-qmk/keyboards/foodforarabbit/sofle_procyon_wt`
   points at this worktree (the pre-existing `sofle_procyon` symlink points at
   the base checkout). Build the worktree with:

   ```bash
   cd ~/foodforarabbit/vial-qmk        # must be on branch debug-vial-tunnel
   make foodforarabbit/sofle_procyon_wt:vial MAXTOUCH_DEBUG=yes
   ```

   (Once `trackpad-tuning` merges to master you can flip `MAXTOUCH_DEBUG` in
   `keymaps/vial/rules.mk` instead of passing it on the command line, and
   build via the normal `sofle_procyon` symlink.)

---

## FLASH 1 — the instrument and the knobs, behavior-neutral

One flash carries everything below. Each piece is independently verifiable
after flashing, so combining them does not cost attribution. All defaults
equal today's compiled values, so **the trackpad feels identical after this
flash** — that is the point, and also the first thing to verify.

### What changes

1. `keymaps/vial/rules.mk`: add `MAXTOUCH_DEBUG = yes` (tunnel already in
   keymap.c — done on this branch).
2. `keymaps/vial/keymap.c`: make the five curve parameters runtime variables
   (RAM struct, loaded from EEPROM), handle the custom keycodes below in
   `process_record_user`, add EEPROM persistence via
   `eeconfig_read/update_user_datablock` (`EECONFIG_USER_DATA_SIZE 12` in
   keymap config.h; API confirmed present, `quantum/eeconfig.h:154`
   MEASURED).
3. `keymaps/vial/vial.json`: add the `customKeycodes` block (spec below).
4. Optional but recommended, costs nothing at runtime: `CONSOLE_ENABLE = yes`
   plus a `uprintf` of each post-curve mouse report and each tip-state
   transition in `pointing_device_task_user`. With `qmk console` attached this
   turns S1/S2 from adjectives into logged numbers; detached it does nothing.
5. Recommended: implement the 1-frame liftoff holdback **behind the
   `TP_LIFT_TOG` runtime toggle, default OFF** (REASONED: behavior-neutral
   when off; having it compiled in saves a whole flash if the S2 measurement
   confirms the bogus-final-delta theory). Logic: hold each frame's delta one
   report; if the next event is a lift, drop the held delta, else release it.
   Adds one report (~3–8 ms at 132–300 Hz) of pointer latency while enabled.

### What it buys

- Live read/write of **every** sensor register over USB while Vial still
  remaps keys (mechanism MEASURED end-to-end at build level; the HID round
  trip itself needs hardware — see verification).
- Live adjustment of every keymap-side parameter from assigned keys, surviving
  power cycles via TP_SAVE.
- Numbers instead of feel for S1/S2 via console logging.

### What question it answers

- S1: does the pointer jitter with the finger held still (noise floor) or only
  while moving (hysteresis/curve)? Raw register experiments then split
  sensor-vs-curve.
- S2: is there a garbage `MXT_MOVE` delta in the last frames before `MXT_UP`?
  (The console log shows it directly; toggling TP_LIFT_TOG then tests the fix.)
- Whether the tunnel + Vial coexistence works on real hardware, not just at
  link time.

### What would make it a wasted flash

- Not verifying behavior-neutrality first (if the pad feels different with all
  defaults unchanged, something else changed — stop and diff).
- Flashing without exporting the Vial layout first: adding
  `EECONFIG_USER_DATA_SIZE` moves the eeconfig layout, which can reset
  VIA/Vial EEPROM including key assignments (REASONED — QMK bumps/relocates
  eeconfig when the datablock appears). **In Vial: File → Save current layout
  before flashing; File → Load saved layout after.** One-time cost.
- Fat-fingering the keycode bounds so a knob can reach a pathological value
  with no way back: TP_RST exists precisely so any tuning state is one
  keypress from compiled defaults.

### Post-flash verification checklist (in order)

1. Typing works on both halves. If not: reflash previous `.uf2` (keep it!),
   keyboard restored, investigate on the bench.
2. Trackpad feels unchanged (defaults are the old constants).
3. Vial GUI sees the keyboard, layers load, a test remap sticks.
4. Custom keycodes appear in Vial's User tab; assign TP_DUMP somewhere and
   press it: current values type out.
5. Tunnel answers: run the script's `check-version` (below). Expect status OK.
6. `qmk console` shows per-report deltas while moving a finger.

### Flashing mechanics (for the record)

Double-tap the reset button (or hold BOOT while plugging in) → RP2040 mounts
as a UF2 drive → copy the `.uf2` → repeat for the other half. Flash **both**
halves with the same image. After flash 1, `MAXTOUCH_DEBUG_REBOOT_BOOTLOADER`
(script command `reboot-bootloader`) enters the bootloader over USB — no
button reach needed for flash 2.

---

## Between flashes — the actual tuning (zero flashes)

All sensor-register work happens over the tunnel with a small host script.
All curve work happens on the assigned keys. Record every session in
`debug/sessions/` per `TEMPLATE.md` — live writes make it EASIER to lose
track of what produced which feel, not harder.

**Key property of live register writes (MEASURED from driver init code): they
do not survive a power cycle.** The driver rewrites the sensor config from
compiled values at every boot. This is a feature — unplug/replug is the
universal undo — but it means a "good" register value is only kept by baking
it into config.h (flash 2) and only tested across reboots after that.

### The host script

~80 lines of Python + `hidapi`. Spec (all packet layouts MEASURED from
`maxtouch.c` + `via.c`):

- Open VID `0xAAC6` PID `0xBAC6`, usage page `0xFF60`, usage `0x61` (the
  Vial raw interface — same one the Vial GUI uses).
- Every request is a 32-byte report: `[0x4D, cmd, args...]`. The reply echoes
  back with `0x4D` in byte 0 and status in byte 1 (0 = OK, 4 = I2C error...).
- `check-version`: `[0x4D, 0x00, 0x9A, 0x4D, 0x00, 0x01]`
- `read  addr len`: `[0x4D, 0x02, addr_hi, addr_lo, len]`, len ≤ 27 through
  the tunnel (the prefix eats one byte; the driver rejects more). Payload
  returns at bytes 5+.
- `write addr len bytes...`: `[0x4D, 0x03, addr_hi, addr_lo, len, ...]`, ≤ 27.
- `set-mouse-mode on|off`: `[0x4D, 0x01, 0x01, 0|1]` — and note **TP_MOUSE_TOG
  on the keyboard is the recovery path** if a script crash leaves mouse
  reports off.
- Register addresses are per-device: read the maXTouch info block at address
  0x0000 (7-byte header ending in `num_objects`, then `num_objects` 6-byte
  table entries: type, addr LSB, addr MSB, size-1, instances-1, report_ids)
  and index objects by type: T8 acquisition, T42 touch suppression, T65 lens
  bending / low-pass, T80 retransmission, T100 multi-touch (thresholds,
  hysteresis, gain, MOVHYSTI/MOVHYSTN live here). (Object-table layout
  REASONED from the datasheet + mirrors what `maxtouch.c` does at init.)
- Do not run the script and the Vial GUI at the same time — both listen on the
  same HID endpoint and replies broadcast to all listeners (REASONED; close
  the Vial app during register sessions).

### Suggested experiment order (one variable per session)

1. **Characterise before changing anything** (console log): S1 still vs
   moving; S2 last-frames delta. This decides everything below.
2. **S1, if raw signal is noisy:** enable T65 + low-pass — write T65 ctrl=1,
   lpfiltcoef=10 (pavonis value). NOTE compile-time equivalent needs BOTH
   `MXT_T65_LENS_BENDING_ENABLE` **and** `MXT_LOW_PASS_FILTER_COEFFICIENT 10`
   — the coefficient alone is dead (MEASURED: `maxtouch.c:456-462` only
   writes it inside the `#ifdef MXT_T65_LENS_BENDING_ENABLE` block; this
   corrects STRATEGY.md, which suggested the coefficient alone).
   Also per procyon README (MEASURED, upstream): raise `MXT_TOUCH_THRESHOLD`,
   and raise `MXT_GAIN` to make headroom for the higher threshold.
3. **S1/S3 hysteresis bracket:** T100 hysteresis to peacock's 8/4, then
   pavonis's 6/3 (ours: 10/5). Then `MOVHYSTN` (`MXT_MOVE_HYSTERESIS_NEXT`):
   ours is 16 while the **driver default is 4** (MEASURED `maxtouch.c:122`)
   — the procyon preset raised it 4×, which is exactly the stick-then-jump
   mechanism. Try 8, then 4.
4. **S2:** if the liftoff log shows the bogus final delta, flip TP_LIFT_TOG on
   and re-measure. If sensor-side suppression works instead (T42, or CONFTHR
   debounce), prefer it — no added latency.
5. **Curve params** (keys, not script): jitter threshold, slow zone — note the
   binary threshold drops ≤5 and passes 6 at full size, itself a plausible S1
   mechanism at slow speeds (REASONED, from `BASELINE.md`).

---

## FLASH 2 — bake in the winners

### What changes
Winning sensor values become `#define`s in `config.h` (they override
`drivers/sensors/procyon.h`, which is all `#ifndef`-guarded — MEASURED);
winning curve values become the new compiled defaults in keymap.c. Optionally
set `MAXTOUCH_DEBUG = no` again if you want the debug surface gone; keeping
it costs a little flash space and keeps the instrument available (REASONED:
leave it on until S1/S2 are confirmed dead across a week of use).

### What it buys
The tuned state survives power cycles and is independent of EEPROM contents.

### What question it answers
Whether the tuned feel reproduces from a cold boot with no live writes — i.e.
that every value you liked was actually captured. (Live writes are volatile;
this is the only way to test persistence.)

### What would make it a wasted flash
Baking values that were never re-confirmed in one combined live session, or
skipping the session log so you bake a value you *think* was active. Before
building: re-apply the full winning set via script in one go, re-test feel,
TP_DUMP + script-read every register you changed, and copy the numbers into
the session file. Then flash.

---

## FLASH 3 — only if needed

Reserved for: the liftoff buffer if it was NOT included in flash 1 and
measurement says it is needed; or driver-level changes (e.g. contributing the
S2 fix upstream properly in `digitizer.c`); or a keycode you failed to declare
in flash 1. If you follow the declare-generously spec below, this flash
should never happen for a missing knob.

---

## Optional side-quest: the maxtouch-debug GUI (heatmap)

The Tauri GUI speaks the UNPREFIXED protocol, so it cannot talk to the vial
tunnel build (REASONED from MEASURED opcode collision: its READ/WRITE = 0x02/
0x03 are swallowed by VIA as get/set_keyboard_value). If you want its live
capacitance heatmap, that is a separate 2-flash detour: build `keymaps/debug`
(copy of `default` + `MAXTOUCH_DEBUG = yes`, NO Vial — same pattern as
upstream peacock/pavonis debug keymaps, MEASURED), flash, visualise, flash
back. The `origin/add-debug-keymap` branch has a README for this but no
actual keymap files (MEASURED). Everything the plan needs — register R/W —
works without the GUI, so treat this as diagnosis-of-last-resort. The GUI's
register-WRITE UI is unfinished upstream anyway (see STRATEGY.md).

---

## customKeycodes spec (flash 1, paste-ready)

Mechanics, so this section stands alone:

- **Declaring a keycode costs a flash; assigning it never does.** The names
  live in `vial.json` (compiled into the firmware and read back by the Vial
  app); the behavior lives in `process_record_user`.
- Vial maps `customKeycodes[i]` to firmware keycode `QK_KB_0 + i`
  (0x7E00 + i). REASONED from the Vial protocol (this fork is Vial protocol 6,
  `quantum/vial.h:25` MEASURED; QK_KB range 0x7E00–0x7E3F, 64 slots,
  `vial_ensure_keycode.h:830` MEASURED). Empirical check after flashing: the
  User tab shows exactly these names in this order.
- **How to assign in the Vial GUI** (first time): install/open the Vial app
  (vial.rocks needs a browser with WebHID; the desktop app is easier) → the
  keyboard is auto-detected, no sideloading needed because the layout JSON is
  read from the firmware → pick a layer tab (suggestion: put tuning keys on
  layer 3/_ADJUST) → click the target key on the on-screen keyboard → in the
  keycode palette below, open the **User** tab → click the tuning keycode.
  The assignment writes to on-board EEPROM instantly; no save button, no
  flash. To remove, assign `KC_TRNS`/`KC_NO` over it.

JSON to add to `keymaps/vial/vial.json` (order defines the QK_KB index —
append only, never reorder after flashing once, or existing assignments
change meaning):

```json
"customKeycodes": [
  {"name": "Pointer speed +", "shortName": "SPD+", "title": "Global pointer speed +10% (50-200%, default 100)"},
  {"name": "Pointer speed -", "shortName": "SPD-", "title": "Global pointer speed -10%"},
  {"name": "Jitter thresh +", "shortName": "JIT+", "title": "Jitter threshold +1 (0-15, default 5); deltas at/below are dropped"},
  {"name": "Jitter thresh -", "shortName": "JIT-", "title": "Jitter threshold -1"},
  {"name": "Slow limit +",    "shortName": "SLL+", "title": "Precision-zone limit +1 (1-30, default 12)"},
  {"name": "Slow limit -",    "shortName": "SLL-", "title": "Precision-zone limit -1"},
  {"name": "Slow speed +",    "shortName": "SLS+", "title": "Precision-zone speed +5% (10-100, default 50)"},
  {"name": "Slow speed -",    "shortName": "SLS-", "title": "Precision-zone speed -5%"},
  {"name": "Fast speed +",    "shortName": "FSS+", "title": "Fast-zone speed +10% (50-300, default 140)"},
  {"name": "Fast speed -",    "shortName": "FSS-", "title": "Fast-zone speed -10%"},
  {"name": "Scroll divisor +","shortName": "SCR+", "title": "Scroll divisor +5, less sensitive (5-100, default 35)"},
  {"name": "Scroll divisor -","shortName": "SCR-", "title": "Scroll divisor -5, more sensitive"},
  {"name": "Smoothing +",     "shortName": "SMO+", "title": "EMA smoothing strength +1 (0-7, default 0=off)"},
  {"name": "Smoothing -",     "shortName": "SMO-", "title": "EMA smoothing strength -1"},
  {"name": "Liftoff buffer",  "shortName": "LIFT", "title": "Toggle 1-frame liftoff holdback (S2 fix candidate, default off)"},
  {"name": "TP save",         "shortName": "TPSV", "title": "Persist all tuning values to EEPROM"},
  {"name": "TP reset",        "shortName": "TPRS", "title": "Reset tuning to compiled defaults and save"},
  {"name": "TP dump",         "shortName": "TPDU", "title": "Type out current tuning values (focus a text field first)"},
  {"name": "Mouse reports",   "shortName": "MSTG", "title": "Toggle digitizer mouse reports (recovery if a debug script left them off)"}
]
```

Firmware-side contract for each (implement in `process_record_user`, keyed
off `QK_KB_0 + index`):

| keycode | adjusts | step | bounds | default | persists |
|---|---|---|---|---|---|
| SPD+/− | global multiplier applied after the curve | 10 pct-pts | 50–200% | 100% | on TP_SAVE |
| JIT+/− | `jitter_threshold` (was `TRACKPAD_JITTER_THRESHOLD`) | 1 | 0–15 | 5 | on TP_SAVE |
| SLL+/− | `slow_limit` | 1 | 1–30 | 12 | on TP_SAVE |
| SLS+/− | `slow_speed` | 5 pct-pts | 10–100% | 50 | on TP_SAVE |
| FSS+/− | `fast_speed` | 10 pct-pts | 50–300% | 140 | on TP_SAVE |
| SCR+/− | runtime scroll divisor (see note) | 5 | 5–100 | 35 | on TP_SAVE |
| SMO+/− | EMA strength k in `out += (in-out)>>k` terms, 0=off | 1 | 0–7 | 0 | on TP_SAVE |
| LIFT | liftoff holdback on/off | toggle | — | off | on TP_SAVE |
| TPSV | writes the 12-byte struct via `eeconfig_update_user_datablock` | — | — | — | is the persist |
| TPRS | restores compiled defaults, then saves | — | — | — | yes |
| TPDU | `send_string` of all current values, e.g. `spd=100 jit=5 sll=12 sls=50 fss=140 scr=35 smo=0 lift=0` | — | — | — | n/a |
| MSTG | `digitizer_send_mouse_reports` flag | toggle | — | on | no (deliberately) |

Notes:
- Deliberate non-choice: no auto-save on every press — EEPROM wear plus you
  want to experiment freely and TP_RST back. Save is explicit.
- Scroll divisor at runtime requires moving the division into the keymap:
  set `DIGITIZER_SCROLL_DIVISOR 1` in config.h and divide (with carried
  remainder, mirroring `digitizer_mouse_fallback.c:220-224` MEASURED) in
  `pointing_device_task_user`. Keep the existing ±2 clamp after division.
- 19 of 64 QK_KB slots used; plenty of headroom to append later — but
  appending is still a flash, hence the generous list now.
- Sensor-side values (GAIN, thresholds, hysteresis) intentionally have NO
  keycodes: they are per-session script writes with the object table in front
  of you, not blind single-steps from the keyboard. (REASONED; revisit only
  if script round-trips prove annoying in practice.)

---

## Things hardware must answer (cannot be established from the desk)

1. Does the tunnel actually round-trip on the wire (flash 1 checklist item 5)?
   Link-level coexistence is MEASURED; the HID path is REASONED until then.
2. Does `EECONFIG_USER_DATA_SIZE` reset Vial assignments on first boot?
   Export the layout first regardless.
3. Everything about feel: S1 characterisation, S2 final-delta existence, and
   every parameter's effect. That is what the instrument is for.
