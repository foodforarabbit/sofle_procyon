#pragma once

/* Unique VIAL keyboard identifier */
#define VIAL_KEYBOARD_UID {0xF0, 0x2D, 0x57, 0x4C, 0x0C, 0xC2, 0x91, 0x91}

/* Unlock VIAL by holding ESC (row 0, col 0) + Enter on right half (row 9, col 0) */
#define VIAL_UNLOCK_COMBO_ROWS { 0, 9 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }

/*
 * Runtime trackpad tuning (flash 1).
 *
 * 12-byte user datablock holds the tuning struct in keymap.c, persisted by
 * TP_SAVE and restored at boot. API confirmed present:
 * quantum/eeconfig.h declares eeconfig_read_user_datablock /
 * eeconfig_update_user_datablock when this is defined.
 *
 * WARNING: adding this relocates the eeconfig layout, which can reset
 * VIA/Vial EEPROM including key assignments on first boot. Export the layout
 * from the Vial GUI (File -> Save current layout) BEFORE flashing.
 */
#define EECONFIG_USER_DATA_SIZE 16

/*
 * Scroll division moves into the keymap so it becomes a runtime knob.
 * The keyboard-level config.h sets 35; we take the raw value here and divide
 * by tp.scroll_div in pointing_device_task_user, carrying the remainder the
 * same way digitizer_mouse_fallback.c does. Redefined rather than edited at
 * the keyboard level so the `default` keymap keeps its existing behaviour.
 */
#undef DIGITIZER_SCROLL_DIVISOR
#define DIGITIZER_SCROLL_DIVISOR 1

/*
 * FLASH 2 -- sensor + gesture fixes, all MEASURED on flash 1.
 */

/* The jitter fix. movhystn shipped at 16 against a driver default of 4, so
 * next-move hysteresis batched sub-threshold drift and released it in
 * quantised 17-unit dumps. Live-writing 4 took the worst pointer jump from
 * 15px to 4px and halved the escape rate. Baked here because sensor registers
 * are rewritten from compiled values at EVERY boot -- a live write does not
 * survive a power cycle, which is the whole reason this flash exists.
 * See debug/sessions/2026-09-10-01.md. */
/* NOT settable here: procyon.h:30 defines MXT_MOVE_HYSTERESIS_NEXT with no
 * per-define #ifndef guard, so this collides (-Werror: "redefined"). PLAN.md
 * claimed procyon.h was "all #ifndef-guarded -- MEASURED"; the 5 #ifndef lines
 * in it are board-SELECTION guards, not per-define guards. Applied from
 * keymap.c at every boot over i2c instead, which keeps vial-qmk at zero
 * changes and survives power cycles just the same. */

/*
 * The two-finger-scroll dead zone.
 *
 * This board raised DIGITIZER_MOUSE_TAP_DISTANCE to 50, double the driver
 * default of 25. digitizer_mouse_fallback.c:190-201 only escapes `Down` to
 * `MoveScroll` if distance exceeds it, so at 50 a short quick drag never
 * qualifies and falls into `Tapped` on lift. In `Tapped` (lines 228-243) two
 * fingers arriving within the timeout match NO branch -- not contacts==0, not
 * duration>timeout -- so the state machine sits there ignoring them until the
 * timeout expires. That is the "buffer time" where a fast move-then-scroll
 * gets swallowed.
 *
 * 25 restores the driver default so real moves are classified as moves; 120ms
 * shortens the dead window for the cases that still land in `Tapped`. Cost:
 * taps must be a little steadier, double-taps a little quicker.
 */
#undef DIGITIZER_MOUSE_TAP_DISTANCE
#define DIGITIZER_MOUSE_TAP_DISTANCE 50
#define DIGITIZER_MOUSE_TAP_DETECTION_TIMEOUT 120

/*
 * TAP_DISTANCE back to 50 -- REVERTING MY OWN FLASH-2 CHANGE.
 *
 * Flash 2 lowered it to 25 to shorten the two-finger-scroll dead zone. That was
 * wrong twice over: it targeted the wrong branch of the state machine (the gate
 * is `Down`, not `Tapped`), and 50 was not arbitrary -- it was tuned for how
 * Ryan actually taps.
 *
 * MEASURED at 25: his taps drift 28-66 accumulated units over 53-82ms, and 6 of
 * 8 logged contacts emitted movement INSTEAD OF CLICKING. Roughly half his taps
 * silently failed, which he reported as the tap feeling "laggy" -- it was not
 * latency, it was retries.
 *
 * The scroll dead zone this was chasing is now fixed properly in the driver
 * (branch procyon-tap-scroll-fix): two fingers landing goes straight to
 * MoveScroll and no longer has to satisfy the tap-rejection tests. So this
 * threshold serves only tap classification again, which is the one job it can
 * actually do.
 *
 * TIMEOUT stays at 120 (driver default 200). With the driver fix it no longer
 * gates two-finger scroll; it now only sets tap-click latency and single-finger
 * move onset, and lower is better for both.
 */
