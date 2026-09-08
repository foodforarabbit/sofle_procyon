# Trackpad debugging log

Tuning this trackpad means: change a parameter, build, flash, and judge the
result **by feel**. That loop has one dominant failure mode, and this folder
exists to prevent it:

> **You cannot tell which firmware produced which feel.**

Two flashes later, "it felt better before" is unactionable unless every session
recorded the exact parameter set and a build identity. So the rule is:

**No feel observation without the config that produced it.**

## Files

| file | what it holds |
|---|---|
| `SYMPTOMS.md` | The catalogue. Every symptom gets a stable id (S1, S2…) so sessions can say "S1 improved, S2 unchanged" instead of "better". |
| `BASELINE.md` | The values we inherit today, measured from source. Without this, no delta means anything. |
| `REFERENCE-PRESETS.md` | Same-sensor tuning from other keyboards using this driver. Real numbers from boards that work. |
| `sessions/YYYY-MM-DD-NN.md` | One file per build+flash+test cycle. Copy `TEMPLATE.md`. |
| `TEMPLATE.md` | The per-session form. |

## The discipline, and why each rule is here

**1. Record the build identity.** Git sha + the `.uf2` filename + build date.
A feel observation attached to an unknown binary is noise. (We already lost an
afternoon this week to testing a stale build in a different context — same
class of error.)

**2. One variable per session where you can.** When you change several, SAY SO
explicitly, and accept that the session can only tell you the combination
worked — not which part did.

**3. Rate against the symptom ids, not in prose.** "S1: 4/5 → 2/5" survives
three sessions later. "Feels smoother" does not.

**4. Record what got WORSE.** Tuning trades off. Lower hysteresis fixes
stick-then-jump and can add jitter; heavier smoothing fixes jitter and adds lag.
A session that only records improvements is how you ship a regression you chose.

**5. Record what you RULED OUT.** A parameter that changed nothing is a real
result and saves the next session from retrying it.

**6. Keep the raw sensor data when you have it.** With `MAXTOUCH_DEBUG = yes`
in `rules.mk` the firmware emits deltas over the console — paste the relevant
window in, because a number beats an adjective.

## The loop

```
1. Pick ONE hypothesis from SYMPTOMS.md
2. cp debug/TEMPLATE.md debug/sessions/$(date +%F)-01.md
3. Fill in "Hypothesis" and "Changes" BEFORE building
   (writing the prediction first is what makes the result falsifiable)
4. Build + flash, record the build identity
5. Test each symptom, rate it, note regressions
6. Fill in "Verdict" and "Ruled out"
```
