# Reference presets — same driver, other keyboards

MEASURED 2026-09-08 from `~/foodforarabbit/vial-qmk`, which is
`george-norton/qmk_firmware` — **George Norton wrote the MaxTouch driver**, so
these are the author's own tunings rather than internet folklore.

Three keyboards in that fork use this sensor: `georgen/peacock` (his own board),
`ploopyco/pavonis`, and `bastardkb/dilemma` (no MXT overrides — inherits).

| param | procyon (ours) | peacock | pavonis |
|---|---|---|---|
| `MXT_GAIN` | 10 | **5** | 11 |
| `MXT_TOUCH_THRESHOLD` | 20 | **18** | 24 |
| `MXT_TOUCH_HYST` | 10 | **8** | **6** |
| `MXT_INTERNAL_TOUCH_HYST` | 5 | **4** | **3** |
| `MXT_INTERNAL_TOUCH_THRESHOLD` | 10 | 10 | 8 |
| `MXT_ACTIVE_ACQUISITION_INTERVAL` | **255** (corrected 2026-09-08 — set by the shared procyon preset, was listed as "default") | **255** (free-run) | **255** |
| `MXT_ACTIVE_SYNCS_PER_X` | 0 (driver default) | 20 | 40 |
| `MXT_LOW_PASS_FILTER_COEFFICIENT` | unset | unset | **10** (with `MXT_T65_LENS_BENDING_ENABLE` — the coefficient only takes effect when T65 is enabled, `maxtouch.c:456-462`) |
| `MXT_CHARGE_TIME` | default | 10 | — |

## What this actually tells us

**Both reference boards run LOWER hysteresis than we inherit** — 8/4 and 6/3
against our 10/5. Two independently tuned boards, same author, both below us.
That is corroboration of the plan's S3 diagnosis from a source that had no
knowledge of our problem.

~~**Both set `ACTIVE_ACQUISITION_INTERVAL 255` (free-run, ~300 Hz).** We do
not.~~ **CORRECTED 2026-09-08: we DO — the shared preset in
`drivers/sensors/procyon.h` sets 255.** The free-run recommendation in
STRATEGY.md's Flash 3 batch was a no-op. What the siblings actually have that
we lack: `MXT_ACTIVE_SYNCS_PER_X` 20/40 (ours 0), and pavonis's T65+low-pass.

**pavonis uses a sensor-side low-pass (`COEFFICIENT 10`); we use none.** Our
plan plans a *firmware-side* EMA to fix S1. If the sensor can do it, that is
strictly better — it filters before the hysteresis logic rather than after, and
costs no firmware complexity. **Try this before writing the EMA.**

## Caveats — do not paste these values in wholesale

- **Pad geometry differs.** peacock is 91×156 mm, pavonis 156×99. Thresholds and
  gain interact with sensor size and stackup, so a value that suits one pad is
  not automatically right for ours.
- **`GAIN` disagrees between the two references** (5 vs 11), so there is no
  single "correct" value to copy — it is genuinely board-specific.
- Treat these as **a bracketed range and a direction**, not a preset to adopt.

## Rejected sources, recorded so nobody re-researches them

- **xcmkb SoflePLUS changelog** — no tuning specs published. Only useful
  finding: they expose a runtime `DPI-` key, which supports our `CPI_DN` plan
  item. Different hardware (Trackpad43/62), so no numbers transfer.
- **`JeffOwOSun/sofle-procyon`, `yushi-shiplight-ai/sofle-procyon`** — case and
  docs only, no firmware. Not forks of ours; independent repos.
