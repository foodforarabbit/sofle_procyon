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

## Where to search for existing reports of our symptoms

Not yet done — worth an hour before changing anything:

1. **Issues + PRs on `george-norton/qmk_firmware`**, especially the
   `multitouch_experiment` branch. Search terms that match our symptoms:
   `liftoff`, `lift off`, `jitter`, `jump`, `hysteresis`, `cursor moves`.
2. **Issues on `george-norton/procyon`** — hardware-level reports.
3. **Issues on `maxtouch-debug`** — people tuning these sensors talk there, and
   the author answers.
4. **The Ploopy community** (pavonis is a shipping product on this driver) —
   a shipping product means someone has already fought S1 and S2.

S2 in particular (pointer moves on liftoff) is a *generic* capacitive-trackpad
problem, not something specific to our build. It is very likely already
discussed upstream, possibly already fixed in the driver. **Check before writing
the 1-frame buffer** the plan proposes — inheriting a fix beats writing one.
