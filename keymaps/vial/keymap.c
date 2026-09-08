

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
/*
 * MaxTouch register tunnel -- live sensor read/write over USB, alongside Vial.
 *
 * WHY THIS LIVES HERE AND NOT IN THE DRIVER. maxtouch.c implements this same
 * protocol, but under `MAXTOUCH_DEBUG` it defines `raw_hid_receive`, which
 * VIA/Vial also defines -- they collide at link time
 * (`multiple definition of raw_hid_receive`, MEASURED). Upstream's own answer
 * is a separate debug keymap with VIA switched off (peacock and pavonis both
 * do exactly that), which would cost us Vial remapping while tuning.
 *
 * Patching the driver works, but every line in vial-qmk is rebase burden on a
 * fork that is already 1090 commits divergent. Everything the protocol needs is
 * public -- `MXT336UD_ADDRESS` and `MXT_I2C_TIMEOUT_MS` from maxtouch.h, the
 * i2c register helpers, and `digitizer_send_mouse_reports` via extern (which is
 * how the driver itself reaches it). So we implement it here and leave
 * `MAXTOUCH_DEBUG = no`: **zero changes to vial-qmk.**
 *
 * HOW IT REACHES US. VIA owns raw_hid_receive and forwards command ids it does
 * not recognise to the weak hook `raw_hid_receive_kb` (via.c:207,284,298),
 * then echoes the buffer back itself. VIA's ids are 0x01-0x13, 0xFE (Vial),
 * 0xFF (unhandled), so 0x4D ('M') is free as a tunnel prefix. The host wraps a
 * packet as [0x4D, <packet...>].
 *
 * CONSEQUENCE: the prefix eats one byte, so payloads cap at 0x1b (27) rather
 * than the native 0x1c. The upstream maxtouch-debug GUI speaks the unprefixed
 * protocol and will NOT work through this tunnel -- use the host script. That
 * costs nothing real: the GUI's register-write UI is unfinished anyway, and it
 * cannot run with VIA enabled regardless.
 */
#if defined(VIA_ENABLE)
#    include "via.h"
#    include "i2c_master.h"
#    include "drivers/sensors/maxtouch.h"

#    define MXT_TUNNEL_PREFIX 0x4D
#    define MXT_TUNNEL_MAX_PAYLOAD 0x1b

/*
 * Opcodes. Copied from the driver's own debug protocol so the host script
 * and the upstream maxtouch-debug tool speak the same language, minus the
 * prefix byte. Kept here rather than #included because maxtouch.c declares
 * them inside `#ifdef MAXTOUCH_DEBUG`, and we deliberately leave that flag
 * OFF -- see the comment block above.
 */
enum {
    MXT_DBG_CHECK_VERSION = 0,
    MXT_DBG_COMMAND       = 1,
    MXT_DBG_READ          = 2,
    MXT_DBG_WRITE         = 3,
};
enum { MXT_DBG_CMD_REBOOT_BOOTLOADER = 0, MXT_DBG_CMD_SET_MOUSE_MODE, MXT_DBG_CMD_GET_MOUSE_MODE };
enum { MXT_DBG_OK = 0, MXT_DBG_INVALID_VERSION, MXT_DBG_INVALID_CMD, MXT_DBG_INVALID_LENGTH, MXT_DBG_I2C_ERR };

#    define MXT_DBG_MAGIC 0x9A4D
#    define MXT_DBG_VERSION 0x0001

/* Owned by digitizer.c; the driver reaches it the same way. */
extern bool digitizer_send_mouse_reports;

static void mxt_tunnel_process(uint8_t *data, uint8_t length) {
    uint8_t status = MXT_DBG_OK;

    switch (data[0]) {
        case MXT_DBG_CHECK_VERSION: {
            const uint16_t magic   = (data[1] << 8) | data[2];
            const uint16_t version = (data[3] << 8) | data[4];
            if (magic != MXT_DBG_MAGIC || version != MXT_DBG_VERSION) {
                status = MXT_DBG_INVALID_VERSION;
            }
            break;
        }
        case MXT_DBG_COMMAND: {
            switch (data[1]) {
                case MXT_DBG_CMD_REBOOT_BOOTLOADER:
                    reset_keyboard();
                    break;
                case MXT_DBG_CMD_SET_MOUSE_MODE:
                    digitizer_send_mouse_reports = (bool)data[2];
                    break;
                case MXT_DBG_CMD_GET_MOUSE_MODE:
                    data[1] = digitizer_send_mouse_reports;
                    break;
                default:
                    status = MXT_DBG_INVALID_CMD;
                    break;
            }
            break;
        }
        case MXT_DBG_READ: {
            const uint16_t addr = (data[1] << 8) | data[2];
            const uint16_t len  = data[3];
            /* Bounds-check against the ACTUAL buffer, not just the protocol
             * max. Upstream checks only `len > 0x1c` and would overread a
             * short report; that is a real upstream bug (worth a PR) and
             * costs us nothing to get right here. */
            if (len > MXT_TUNNEL_MAX_PAYLOAD || len + 4 > length) {
                status = MXT_DBG_INVALID_LENGTH;
            } else if (i2c_read_register16(MXT336UD_ADDRESS, addr, &data[4], len, MXT_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
                status = MXT_DBG_I2C_ERR;
            }
            break;
        }
        case MXT_DBG_WRITE: {
            const uint16_t addr = (data[1] << 8) | data[2];
            const uint16_t len  = data[3];
            if (len > MXT_TUNNEL_MAX_PAYLOAD || len + 4 > length) {
                status = MXT_DBG_INVALID_LENGTH;
            } else if (i2c_write_register16(MXT336UD_ADDRESS, addr, &data[4], len, MXT_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
                status = MXT_DBG_I2C_ERR;
            }
            break;
        }
        default:
            status = MXT_DBG_INVALID_CMD;
            break;
    }

    data[0] = status;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (length > 1 && data[0] == MXT_TUNNEL_PREFIX) {
        mxt_tunnel_process(data + 1, length - 1);
        /* VIA sends the buffer back for us -- do NOT call raw_hid_send. */
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
