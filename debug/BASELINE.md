# Baseline — what we inherit today

MEASURED 2026-09-08 by reading source. No `MXT_` override exists in
`sofle_procyon/config.h`, so **every sensor value below is inherited**, not chosen.

## Sensor (I2C registers) — from `vial-qmk/drivers/sensors/procyon.h`

(Path corrected 2026-09-08: the shared Procyon preset lives in
`drivers/sensors/procyon.h`, selected by our `PROCYON_57_80` define and
included via `quantum/digitizer_driver.h:19`. Values not set there fall back
to `drivers/sensors/maxtouch.c` driver defaults.)

| param | value | note |
|---|---|---|
| `MXT_GAIN` | 10 | |
| `MXT_TOUCH_THRESHOLD` | 20 | |
| `MXT_TOUCH_HYST` | 10 | |
| `MXT_INTERNAL_TOUCH_THRESHOLD` | 10 | |
| `MXT_INTERNAL_TOUCH_HYST` | 5 | |
| `MXT_MOVE_HYSTERESIS_NEXT` | 16 | prime S3 suspect — the **driver default is 4** (`maxtouch.c:122`); the procyon preset raised it 4x |
| `MXT_MOVE_HYSTERESIS_INITIAL` | 10 | driver default |
| `MXT_T56_SHIELDLESS_ENABLE` | set | |
| `MXT_T65_LENS_BENDING_ENABLE` | **unset** | T65 gates the low-pass filter below |
| `MXT_RETRANSMISSION_COMPENSATION_ENABLE` | 0 | T80 explicitly disabled (pavonis enabled it "based on tester feedback", upstream PR #6) |
| `MXT_LOW_PASS_FILTER_COEFFICIENT` | **unset** | sensor-side smoothing NOT enabled — and setting it alone does nothing; it is only written inside the T65 block (`maxtouch.c:456-462`) |
| `MXT_ACTIVE_ACQUISITION_INTERVAL` | **255** | ~~driver default, not free-run~~ CORRECTED 2026-09-08: the procyon preset sets 255 — **we already free-run at ~300 Hz** |
| `MXT_ACTIVE_SYNCS_PER_X` | 0 (driver default) | peacock 20, pavonis 40 |
| `MXT_CONFTHR` | 5 | touch down/up debounce, set by the preset |

## Firmware — `keymaps/vial/keymap.c`

| param | value | behaviour |
|---|---|---|
| `TRACKPAD_JITTER_THRESHOLD` | 5 | deltas ≤5 dropped **entirely** — binary, no smoothing |
| `TRACKPAD_SLOW_LIMIT` | 12 | deltas ≤12 are the "precision zone" |
| `TRACKPAD_SLOW_SPEED` | 50 | precision zone runs at 50% of raw |
| `TRACKPAD_FAST_SPEED` | 140 | above the precision zone, 140% of raw (added 2026-09-08 — was missing here) |
| `SCROLL_MAX_PER_REPORT` | 2 | scroll ticks clamped to ±2 per HID report |

Plus in `config.h` (consumed by `quantum/digitizer_mouse_fallback.c`, MEASURED):
`DIGITIZER_SCROLL_DIVISOR 35` (default 10) and `DIGITIZER_MOUSE_TAP_DISTANCE 50`
(default 25).

## Build flags — `rules.mk`

| flag | value |
|---|---|
| `MAXTOUCH_DEBUG` | **no** ← the switch exists, has never been turned on |

## Why the baseline matters

Two things follow from "no overrides exist":

1. **Nothing has been tuned at the sensor level yet.** Every prior improvement
   (commits `e67e52f`, `e049657`) was firmware-side curve work on top of an
   untouched sensor. The layer the plan blames for S3 has never been changed.
2. **The binary jitter threshold is doing all the noise rejection.** A delta of
   5 is discarded and a delta of 6 passes at full magnitude — which is a
   plausible mechanism for S1 on its own, independent of the sensor.
