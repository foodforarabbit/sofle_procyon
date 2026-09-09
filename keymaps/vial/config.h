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
#define EECONFIG_USER_DATA_SIZE 12

/*
 * Scroll division moves into the keymap so it becomes a runtime knob.
 * The keyboard-level config.h sets 35; we take the raw value here and divide
 * by tp.scroll_div in pointing_device_task_user, carrying the remainder the
 * same way digitizer_mouse_fallback.c does. Redefined rather than edited at
 * the keyboard level so the `default` keymap keeps its existing behaviour.
 */
#undef DIGITIZER_SCROLL_DIVISOR
#define DIGITIZER_SCROLL_DIVISOR 1
