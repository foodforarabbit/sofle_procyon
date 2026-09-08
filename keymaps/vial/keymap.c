

#include QMK_KEYBOARD_H

enum layer_namKC {
    _BASE,
    _LOWER,
    _RAISE,
    _ADJUST
};
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] =	LAYOUT(
        KC_ESC,     KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,   KC_0,  KC_MINS,
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
         KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
       KC_LGUI,  KC_LALT,     KC_LCTL,   KC_LGUI,   MO(1),  KC_SPC,                KC_ENT,   MO(2),  KC_BSPC,  KC_RALT, KC_RCTL, KC_RGUI 
                                //`----------------------------------'  `------------------------------------'
	),

    [_LOWER] =	LAYOUT(
        KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX,   KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_MINS,                       KC_INS, KC_HOME,   KC_UP,  KC_END, KC_PGUP, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LCTL,   KC_F5,   KC_F6,   KC_F7,   KC_F8, KC_EQL,                        KC_DEL, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT,   KC_F9,  KC_F10,  KC_F11,  KC_F12, KC_PAUS,                      XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
     _______,    _______,  _______,  KC_LGUI, _______,  KC_SPC,                    KC_ENT, _______, _______, KC_LALT, _______, _______
                                //`-----------------------------------'  `----------------------------------'
	),
	[_RAISE] =	LAYOUT(
        KC_CAPS, KC_EXLM, KC_DQUO, KC_HASH, KC_CIRC, XXXXXXX,                      KC_ASTR,    KC_7,    KC_8,    KC_9, KC_MINS, KC_BSPC,
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_CAPS, KC_EXLM, KC_DQUO, KC_HASH, KC_CIRC, XXXXXXX,                      KC_ASTR,    KC_7,    KC_8,    KC_9, KC_MINS, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LCTL, KC_LCBR, KC_RCBR, KC_LPRN, KC_RPRN, XXXXXXX,                      KC_SLSH,    KC_4,    KC_5,    KC_6, KC_PLUS, KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT, KC_LBRC, KC_RBRC, KC_LABK, KC_RABK,  KC_EQL,                         KC_0,    KC_1,    KC_2,    KC_3,  KC_DOT, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______,  _______,  _______,  KC_LGUI, _______,  KC_SPC,                  KC_ENT, _______,  _______, _______, _______, _______
                                //`----------------------------------'  `------------------------------------'
	),
   [_ADJUST] =	LAYOUT(
        XXXXXXX, XXXXXXX,   KC_AT, XXXXXXX,  KC_DLR, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX, XXXXXXX,   KC_AT, XXXXXXX,  KC_DLR, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LCTL, XXXXXXX, KC_AMPR, KC_PIPE, KC_BSLS, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_TILD,  KC_GRV,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, KC_PERC, XXXXXXX,                      RM_TOGG, RM_NEXT, RM_HUEU, RM_SATU, RM_VALU, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
        _______,  _______,   _______, KC_LGUI, _______,  KC_SPC,                   KC_ENT, _______, _______, KC_LALT, _______, _______
                                 //`---------------------------------'  `-----------------------------------'
	)

};

/*
 * Trackpad curve -- attempt at Apple-like feel.
 *
 * TRACKPAD_JITTER_THRESHOLD  Deltas at or below this are ignored entirely.
 *                            Prevents cursor jump when lifting your finger.
 *
 * TRACKPAD_SLOW_LIMIT        Deltas up to this are in the "precision zone"
 *                            and get scaled by TRACKPAD_SLOW_SPEED.
 *
 * TRACKPAD_SLOW_SPEED        Speed in the precision zone (percent of raw).
 *                            Lower = slower / more precise at low speed.
 *
 * TRACKPAD_FAST_SPEED        Speed above the precision zone (percent of raw).
 *                            Higher = more acceleration on fast swipes.
 */
#define TRACKPAD_JITTER_THRESHOLD  5
#define TRACKPAD_SLOW_LIMIT       12
#define TRACKPAD_SLOW_SPEED       50
#define TRACKPAD_FAST_SPEED      140

static inline mouse_xy_report_t trackpad_curve(int16_t delta) {
    if (delta == 0) return 0;

    int16_t sign  = (delta > 0) ? 1 : -1;
    int16_t abs_d = (delta > 0) ? delta : -delta;

    if (abs_d <= TRACKPAD_JITTER_THRESHOLD)
        return 0;

    int16_t result;
    if (abs_d <= TRACKPAD_SLOW_LIMIT) {
        result = (abs_d * TRACKPAD_SLOW_SPEED) / 100;
    } else {
        int16_t slow_part = (TRACKPAD_SLOW_LIMIT * TRACKPAD_SLOW_SPEED) / 100;
        int16_t fast_part = ((abs_d - TRACKPAD_SLOW_LIMIT) * TRACKPAD_FAST_SPEED) / 100;
        result = slow_part + fast_part;
    }

    return (mouse_xy_report_t)(sign * (result > 0 ? result : 1));
}

/*
 * Scroll tuning
 *
 * Base sensitivity is set by DIGITIZER_SCROLL_DIVISOR in config.h (default 10,
 * we use 35 for Apple-like feel).  On top of that we clamp the per-report
 * scroll amount so fast two-finger swipes don't produce huge jumps.
 */
#define SCROLL_MAX_PER_REPORT  2   /* max scroll ticks per HID report */

static inline int8_t clamp_scroll(int8_t val) {
    if (val > SCROLL_MAX_PER_REPORT)  return SCROLL_MAX_PER_REPORT;
    if (val < -SCROLL_MAX_PER_REPORT) return -SCROLL_MAX_PER_REPORT;
    return val;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    mouse_report.x = trackpad_curve(mouse_report.x);
    mouse_report.y = trackpad_curve(mouse_report.y);
    mouse_report.h = clamp_scroll(-mouse_report.h);
    mouse_report.v = clamp_scroll(-mouse_report.v);
    return mouse_report;
}

/*
 * MaxTouch debug tunnel -- lets the maxtouch debug protocol coexist with
 * VIA/Vial on the single raw-HID endpoint.
 *
 * VIA owns raw_hid_receive and forwards any command id it does not
 * recognise to raw_hid_receive_kb, then echoes the (possibly modified)
 * buffer back to the host. VIA's own command ids are 0x01-0x13, 0xFE
 * (Vial) and 0xFF (unhandled), so we claim 0x4D ('M') as a tunnel prefix:
 * the host wraps a maxtouch debug packet as [0x4D, <packet...>] and gets
 * the reply back with the same prefix. Vial GUI traffic is unaffected.
 *
 * Host-side note: because the prefix eats one byte of the 32-byte report,
 * READ/WRITE payloads through the tunnel max out at 0x1b (27) bytes, not
 * the native 0x1c -- the driver rejects oversized requests with
 * MAXTOUCH_DEBUG_INVALID_LENGTH.
 */
#if defined(MAXTOUCH_DEBUG) && defined(VIA_ENABLE)
#    include "via.h"
#    include "drivers/sensors/maxtouch.h"

#    define MXT_TUNNEL_PREFIX 0x4D

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (data[0] == MXT_TUNNEL_PREFIX) {
        maxtouch_debug_hid_receive(data + 1, length - 1);
    } else {
        data[0] = id_unhandled;
    }
}
#endif

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(MS_WHLD, MS_WHLU), ENCODER_CCW_CW(KC_AUDIO_VOL_UP, KC_AUDIO_VOL_DOWN) },
    [_LOWER] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_RAISE] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_ADJUST] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) }
};
#endif
