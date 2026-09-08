# Baseline — what we inherit today

MEASURED 2026-09-08 by reading source. No `MXT_` override exists in
`sofle_procyon/config.h`, so **every sensor value below is inherited**, not chosen.

## Sensor (I2C registers) — from `vial-qmk/keyboards/.../procyon.h`

| param | value | note |
|---|---|---|
| `MXT_GAIN` | 10 | |
| `MXT_TOUCH_THRESHOLD` | 20 | |
| `MXT_TOUCH_HYST` | 10 | |
| `MXT_INTERNAL_TOUCH_THRESHOLD` | 10 | |
| `MXT_INTERNAL_TOUCH_HYST` | 5 | |
| `MXT_MOVE_HYSTERESIS_NEXT` | 16 | the plan's prime suspect for S3 |
| `MXT_MOVE_HYSTERESIS_INITIAL` | 10 | driver default |
| `MXT_T56_SHIELDLESS_ENABLE` | set | |
| `MXT_RETRANSMISSION_COMPENSATION_ENABLE` | 0 | |
| `MXT_LOW_PASS_FILTER_COEFFICIENT` | **unset** | sensor-side smoothing NOT enabled |
| `MXT_ACTIVE_ACQUISITION_INTERVAL` | driver default | not free-run |

## Firmware — `keymaps/vial/keymap.c`

| param | value | behaviour |
|---|---|---|
| `TRACKPAD_JITTER_THRESHOLD` | 5 | deltas ≤5 dropped **entirely** — binary, no smoothing |
| `TRACKPAD_SLOW_LIMIT` | 12 | deltas ≤12 are the "precision zone" |
| `TRACKPAD_SLOW_SPEED` | 50 | precision zone runs at 50% of raw |

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
