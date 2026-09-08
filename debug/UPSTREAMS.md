# Upstreams — where this firmware comes from, and where to look for prior reports

All from `readme.md`, verified 2026-09-08.

| upstream | what it is | why you'd read it |
|---|---|---|
| [george-norton/procyon](https://github.com/george-norton/procyon) | The trackpad **hardware** + its docs | Sensor stackup, pad geometry, official tuning guidance. First stop for "is this value sane for this pad". |
| [george-norton/qmk_firmware](https://github.com/george-norton/qmk_firmware/tree/multitouch_experiment) branch `multitouch_experiment` | The **MaxTouch driver**. Not in mainline QMK. | Where `maxtouch.c` / `digitizer.c` live. Issues here are the closest thing to a bug tracker for our symptoms. |
| [george-norton/maxtouch-debug](https://github.com/george-norton/maxtouch-debug) | Tauri app that **visualises raw sensor data** | The diagnosis tool. See STRATEGY.md — it reads but does **not yet** write registers. |
| [foodforarabbit/vial-qmk](https://github.com/foodforarabbit/vial-qmk) branch `vial-procyon` | Ryan's fork merging the driver into vial-qmk | What we actually build against. Local at `~/foodforarabbit/vial-qmk`. |
| [LXF-YZP/KafkaSplit](https://github.com/LXF-YZP/KafkaSplit) | The base keyboard design | Unlikely to help with trackpad; relevant for matrix/RGB/encoder. |

## Sibling boards on the same driver — the most useful prior art

`georgen/peacock` and `ploopyco/pavonis` both live *inside* the driver fork and
both are tuned by the driver's author. Their configs are in
`REFERENCE-PRESETS.md`. maxtouch-debug names those same two boards as its
targets, which means **the tool was built against hardware very close to ours**.

## Upstream search — DONE 2026-09-08, all locations MEASURED

**Bottom line: no upstream fix exists for S1 or S2.** Nobody has even filed
the plain liftoff-jump report. We write the first fix, not inherit one.

| location | S1 (jitter) | S2 (liftoff) |
|---|---|---|
| george-norton/qmk_firmware issues | zero issues exist in the repo | zero |
| george-norton/qmk_firmware PRs (9 total) | PRs #4/#5/#6 (ploopyco, Sep 2024): noise-object tuning — T7 free-run, T42 suppression, T56/T65 ("purportedly help with noise"), T80 retransmission "based on tester feedback". Indirectly S1-relevant. | nothing |
| george-norton/procyon | README (the only explicit S1 advice anywhere): "If your sensor is jittery... increase the touch threshold"; raise transmit gain to make headroom | nothing |
| george-norton/maxtouch-debug issues | zero issues | zero |
| Ploopy community (pavonis) | nothing (reddit/blog/reviews searched; no jitter complaints) | ploopyco/trackpad#3 "Mouse jumping due to rapid input" (open Jul 2025, unanswered) — adjacent (two rapid taps merge into one motion), not the single-finger liftoff jump |
| multitouch_experiment commits | commit-search for liftoff/jitter/smoothing: 0 hits. e529af2 + c713e8c fix *stuck fingers* (the opposite failure); 6165918 "Fix jumping when adjusting DPI" is unrelated | same |

REASONED from the above: the driver has no liftoff-motion suppression and no
delta-smoothing code path at all — both would be new contributions. The
sanctioned knobs are maXTouch object registers (thresholds, gain, T56/T65/T80)
— which, per STRATEGY.md CORRECTION and PLAN.md, we can now write live.

Original search list, kept for reference:

1. **Issues + PRs on `george-norton/qmk_firmware`**, especially the
   `multitouch_experiment` branch. Search terms that match our symptoms:
   `liftoff`, `lift off`, `jitter`, `jump`, `hysteresis`, `cursor moves`.
2. **Issues on `george-norton/procyon`** — hardware-level reports.
3. **Issues on `maxtouch-debug`** — people tuning these sensors talk there, and
   the author answers.
4. **The Ploopy community** (pavonis is a shipping product on this driver) —
   a shipping product means someone has already fought S1 and S2.

S2 in particular (pointer moves on liftoff) is a *generic* capacitive-trackpad
problem, not something specific to our build. ~~It is very likely already
discussed upstream, possibly already fixed in the driver.~~ Checked 2026-09-08:
it is not — see the table above. The 1-frame buffer (or a sensor-side
suppression) has to be ours.
