# Symptom catalogue

Stable ids so sessions can reference them. Add new ones; never renumber.

Each symptom records **who observed it** and **whether it is confirmed**, because
a symptom taken from a plan document is a hypothesis until someone feels it.

---

## S1 — Jittery movement
**Source:** Ryan, reported 2026-09-08. **Status:** REPORTED, not yet characterised.

Pointer movement is jittery during travel.

Not yet established, and worth pinning down because they have different fixes:
- Does it jitter while the finger is MOVING, or also while HELD STILL?
- Is it worse at slow speeds, fast speeds, or uniform?
- Is it position-dependent (edges vs centre of the pad)?

Candidate causes: sensor noise passing the touch threshold; `MXT_GAIN` too high;
missing sensor-side low-pass filter; the firmware's binary jitter threshold
letting through anything above 5 units unchanged.

## S2 — Liftoff moves the pointer
**Source:** Ryan, reported 2026-09-08. **Status:** REPORTED.

Lifting the finger makes the pointer move — a jump at the end of every gesture,
which lands the cursor somewhere other than where you left it.

Mechanism (from `TRACKPAD_TUNING_PLAN.md`, REASONED not yet measured): as the
finger peels off, its contact area shifts, and the IC reports that shift as a
`MXT_MOVE` **before** it reports `MXT_UP`. So the last delta of every gesture is
garbage.

Planned fix: 1-frame buffering in `pointing_device_task_user` — hold each frame
and discard it if the next event is a lift.

---

## Suspected, from the plan — NOT yet confirmed by observation

Listed separately so nobody treats a document as an observation.

## S3 — Stick-then-jump at slow speed
**Source:** `TRACKPAD_TUNING_PLAN.md`. **Status:** SUSPECTED.

Slow movement produces nothing, nothing, then a jump. Attributed to
`MXT_MOVE_HYSTERESIS_NEXT = 16` holding position until accumulated movement
crosses the threshold, compounded by the firmware dropping sub-5-unit deltas.

Note S1 and S3 may be the same underlying thing seen at different speeds, or
they may be opposites (too much filtering vs too little). Worth deciding early
which it is — the fixes pull in opposite directions.

## S4 — Scroll feel
**Source:** plan. **Status:** SUSPECTED / unquantified.

## S5 — Tap reliability
**Source:** plan; commit `e049657` claims "improve tap reliability" already
landed. **Status:** UNKNOWN whether still an issue after that commit.
