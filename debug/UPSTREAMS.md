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

---

# The 261-commit scare — resolved 2026-09-08. Do not re-run this.

`git log HEAD..george-norton/multitouch_experiment` reports **261 commits we
"lack"**, 39 of which touch the driver, with titles that look alarming:

    e529af27b0  Fixes for stuck fingers.
    c713e8c844  Handle Un suppressed up.
    782835cb3a  Tweak the T42 touch suppression config...
    b7c4cf1ed8  Added code to activate the T56 and T65 objects, which
                purportedly help with noise.

Three of those name our exact symptoms. **We already have all of them.**
MEASURED in our tree:

| fix | where it already is |
|---|---|
| `MXT_UNSUPSUP` treated as an up event | `maxtouch.c:543` |
| suppressed-event reporting disabled | `maxtouch.c:400` (`tcheventcfg = 24`) |
| the corrected `finger_contact` tip logic | `digitizer.c:237` |
| T56 / T65 noise objects | present (2 refs) |
| T42 touch suppression | present (17 refs) |

## Why the count lies — and it is the same trap twice in one day

**George REBASES `multitouch_experiment` onto newer QMK** (there is a literal
`Rebase pavonis+peacock on develop` commit, `2ab2b3615f`). A rebase rewrites
SHAs, so the identical content arrives with new commit objects that are not
ancestors of our merge. `git log A..B` compares **commit identity**, not
**content**, so it reports work we already carry as missing.

This is exactly the squash-merge false positive that made merged mono
worktrees look unmerged the same morning. **The lesson generalises: after any
rebase or squash upstream, only a CONTENT diff answers "do we have this".**
`git log`, `git cherry` (patch-id) and `merge-base --is-ancestor` all fail here.

The reliable check:

```sh
git diff HEAD george-norton/multitouch_experiment --stat -- \
  drivers/sensors/ quantum/digitizer.c
```

## What we are genuinely missing (MEASURED, content diff)

    maxtouch.c   +25      maxtouch.h  +4
    digitizer.c  +72      procyon.h   +3      (pressure coefficient only)
    pmw3325.*   +217      unrelated optical sensor, irrelevant to us

The `digitizer.c` +72 is **contact scaling and pressure/size reporting** —
`scale_offsets`, `reset_scaling_offset`, `clamp_report_value`, a motion-pin
read. Relevant only if we ever want pressure or palm-area data.

**Nothing in the gap touches jitter or liftoff.**

**And our board's preset is unchanged upstream**: `procyon.h` still sets
`MXT_MOVE_HYSTERESIS_NEXT 16`. George has not retuned it in the seven months
since our merge, so a merge would hand us no tuning.

## The consequence for the plan — read this, it is the useful part

Merging is NOT a prerequisite for flash 1, and it would not fix S1 or S2.

More importantly: **our symptoms persist WITH every upstream liftoff fix
already active.** So S2 is not the generic stuck-finger bug George fixed — it
is either the residual contact-area shift the original plan describes, or
something specific to this pad or stackup. That makes the flash-1 measurement
the only thing that can tell us which, and it means the 1-frame liftoff buffer
stays a HYPOTHESIS until the console log actually shows a bogus final delta.

## How these forks are managed, since it comes up

Nobody fast-forwards. `qmk/qmk_firmware` moves daily; `vial-kb/vial-qmk` lands
work roughly monthly; `george-norton/qmk_firmware`'s **master is a dead 2024
snapshot** while all its work lives on `multitouch_experiment` (last commit
2026-08-06). These are divergent forks carrying real work, so the only move is
merging upstream in periodically — and because George rebases, every merge
conflicts on content you already have.

That is precisely why the structure here is right and worth preserving:
**two commits in vial-qmk** (a merge plus ~38 lines of keycode/API
reconciliation) and **the keyboard in its own repo**, symlinked into the tree.
Keeping the register tunnel in our keymap rather than patching the driver
protects that property.
