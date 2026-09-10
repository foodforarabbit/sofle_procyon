# START HERE — Sofle Procyon trackpad tuning

**Point any new agent or session at this file.** It is the entry point for the
whole effort. Read it top to bottom before touching anything; it takes 5 minutes
and it will stop you redoing a day of research.

---

## The one-paragraph brief

The Sofle Procyon's MaxTouch trackpad has two reported problems: **movement is
jittery (S1)** and **lifting the finger moves the pointer (S2)**. A plan exists
and is solid. **Nothing has been flashed and no product code has changed.** The
remaining work is ~200 lines in `keymaps/vial/keymap.c`, a JSON block, two
`rules.mk` lines, and an ~80-line host script — all specified in `PLAN.md`.

## Read these, in this order

| # | file | why |
|---|---|---|
| 1 | **`PLAN.md`** | **The plan of record.** Flash-ordered. If it disagrees with anything else here, PLAN.md wins. |
| 2 | `SYMPTOMS.md` | Stable symptom ids (S1…S5). Use them; never renumber. Note S1/S2 are REPORTED by Ryan, S3–S5 only SUSPECTED. |
| 3 | `BASELINE.md` | Every value we inherit today, measured. |
| 4 | `REFERENCE-PRESETS.md` | Same-driver tunings from `georgen/peacock` and `ploopyco/pavonis`, both by the driver's own author. |
| 5 | `README.md` | The logging discipline. Read before your first session. |
| 6 | `TEMPLATE.md` | Copy per flash/test cycle into `sessions/`. |
| 7 | `UPSTREAMS.md` | The five upstreams + the (already-done, empty) search for prior reports. |
| — | `STRATEGY.md` | **SUPERSEDED by PLAN.md.** Kept for its reasoning and its two self-corrections. Do not execute from it. |
| — | `ORIGINAL-PLAN.md` | Ryan's own first diagnosis, verbatim. Historical; PLAN.md supersedes. |

## The state of the world, so you don't rediscover it

**Two repos are involved and BOTH have pushed branches:**

| repo | branch | holds |
|---|---|---|
| `foodforarabbit/sofle_procyon` | `trackpad-tuning` | all of `debug/`, this file, and the tunnel |
| `foodforarabbit/vial-qmk` | `procyon-tap-scroll-fix` | **BUILD FROM THIS** (2026-09-10). `vial-procyon` is still pristine. |

**vial-qmk now carries exactly ONE commit, added 2026-09-10 with Ryan's
explicit approval** (`procyon-tap-scroll-fix`, 26 lines in
`quantum/digitizer_mouse_fallback.c`). It fixes two upstream state-machine
defects that no config value could reach: two-finger scroll having to satisfy
the tap-rejection tests, and the contact-count-change frame emitting a
centroid jump as movement. `vial-procyon` is untouched and still builds.

Everything else still lives in `keymaps/vial/keymap.c` and `MAXTOUCH_DEBUG`
stays `no`. The zero-changes rule held for the whole tunnel and register-tuning
effort; it was relaxed only for a defect that is genuinely upstream's and is
worth sending to george-norton.

`origin/debug-vial-tunnel` exists as a record of the driver-patch approach that
was tried first. **Do not merge it** — it is superseded and would reintroduce
fork divergence. It also carries a genuine upstream bounds-check fix worth
sending to george-norton as a PR.

Toolchain note: `arm-none-eabi-gcc@8` is brew keg-only, so it is off PATH.
Build with:
```
export PATH="$(brew --prefix arm-none-eabi-gcc@8)/bin:$(brew --prefix arm-none-eabi-binutils)/bin:$PATH"
```

Local-only, not in git, recreate if missing: a symlink
`vial-qmk/keyboards/foodforarabbit/sofle_procyon_wt` → this worktree, so builds
use the worktree instead of the base checkout.

## The five things worth knowing before you form an opinion

1. **Sensor registers are LIVE-WRITABLE over USB.** `MAXTOUCH_DEBUG = yes`
   exposes a raw-HID protocol with READ **and WRITE** opcodes
   (`maxtouch.c:590-675`). So sensor tuning costs **zero** flashes after flash 1.
   The `maxtouch-debug` tool's README calls register writing "planned" — the
   *firmware* half is already implemented; the GUI is what's missing.
2. **Live register writes do NOT survive a power cycle.** The driver rewrites
   config from compiled values at boot. Unplug = universal undo. Nothing is
   real until it's baked in at flash 2 and cold-boot tested.
3. **`raw_hid_receive` was a hard conflict with Vial** — VIA and maxtouch.c
   both define it (`multiple definition`, MEASURED). Solved with a tunnel:
   prefix byte `0x4D` through Vial's weak `raw_hid_receive_kb` hook, with the
   protocol reimplemented in OUR keymap so the fork stays clean. Do not "fix"
   this by disabling Vial or by patching the driver — keeping Vial usable AND
   keeping vial-qmk close to upstream are both explicit requirements.
4. **Adding a Vial keycode costs a flash; assigning one does not.** That's why
   flash 1 declares 19 knobs, generously.
5. **Nothing to inherit upstream.** Zero issues across all three
   george-norton repos; no liftoff/jitter fix in any PR or commit. The S2 fix
   will be ours. Closest prior art: `ploopyco/trackpad#3`, open and unanswered.

## Ryan's constraints — these are requirements, not preferences

1. **Minimal flashing.**
2. **The keyboard stays usable while tuning** — non-trackpad typing must work.
3. **Keybindings assignable from the Vial GUI**, not hardcoded.

## The single most important instruction

**Do not tune before you measure.** Flash 1 is deliberately behaviour-neutral;
its job is to answer one question — does S1 jitter with the finger *held still*
(noise floor → sensor-side fix) or only *while moving* (hysteresis/curve →
firmware fix)? **Those two have opposite fixes**, so guessing first means a
50% chance of making it worse and not knowing why.

## Hard rules

- **Never flash without recording the build identity** in a session file.
  See `README.md` for why.
- **Push every branch immediately.** Both repos have already lost work to
  `squad create`, which hard-syncs the parent checkout to origin and discards
  unpushed commits and untracked files (`todo-b220b52e`). Unpushed = at risk.
- **Export the Vial layout before flash 1** (File → Save current layout).
  Adding `EECONFIG_USER_DATA_SIZE` can reset VIA/Vial EEPROM including key
  assignments.
- Label every claim **MEASURED** or **REASONED**. The docs already carry
  several self-corrections; adding another is welcome, not embarrassing.
- Do not flash hardware on Ryan's behalf without being asked.

## What to do next

Implement flash 1 per `PLAN.md` §"FLASH 1 → What changes" — items 1–5. It is
specified down to the API calls and the keycode table. Then hand back to Ryan
to flash; he tunes from there with keys and the host script.
