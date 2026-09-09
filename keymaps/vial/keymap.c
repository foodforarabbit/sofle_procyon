

#include QMK_KEYBOARD_H
#include <stdio.h>   /* snprintf, for TP_DUMP */
#include "print.h"   /* uprintf; no-op without CONSOLE_ENABLE */

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
 * FLASH 1: every parameter below became a RUNTIME value held in `tp`, loaded
 * from the EEPROM user datablock at boot and adjustable from Vial-assigned
 * keys. The TP_DEF_* constants are the compiled defaults and are what TP_RST
 * restores; they equal the values this keymap shipped with, so a fresh flash
 * feels IDENTICAL. That is the point of flash 1 -- if the pad feels different
 * with defaults untouched, something else changed. Stop and diff.
 *
 * jitter      Deltas at or below this are ignored entirely.
 * slow_limit  Deltas up to this are the "precision zone", scaled by slow_speed.
 * slow_speed  Speed in the precision zone (percent of raw). Lower = finer.
 * fast_speed  Speed above the precision zone (percent). Higher = more accel.
 * speed       Global multiplier applied AFTER the curve (percent).
 * scroll_div  Scroll divisor, moved out of the driver so it is tunable.
 * smoothing   EMA strength k; 0 = off. Higher = smoother but laggier.
 * lift        1-frame liftoff holdback (S2 candidate), default OFF.
 */
#define TP_DEF_SPEED       100
#define TP_DEF_JITTER        5
#define TP_DEF_SLOW_LIMIT   12
#define TP_DEF_SLOW_SPEED   50
#define TP_DEF_FAST_SPEED  140
#define TP_DEF_SCROLL_DIV   35
#define TP_DEF_SMOOTHING     0
#define TP_DEF_LIFT          0

/* Bumping this invalidates a saved block and falls back to defaults, which is
 * also what makes a VIRGIN EEPROM safe: unwritten flash reads as 0x00/0xFF,
 * and either would otherwise load speed=0 and kill the pointer with no way
 * back except a reflash. */
#define TP_CFG_VERSION 1

typedef struct __attribute__((packed)) {
    uint8_t  version;
    uint8_t  speed;       /*  50..200 % */
    uint8_t  jitter;      /*   0..15    */
    uint8_t  slow_limit;  /*   1..30    */
    uint8_t  slow_speed;  /*  10..100 % */
    uint16_t fast_speed;  /*  50..300 % */
    uint8_t  scroll_div;  /*   5..100   */
    uint8_t  smoothing;   /*   0..7     */
    uint8_t  lift;        /*   0..1     */
    uint8_t  reserved[2];
} tp_config_t;

_Static_assert(sizeof(tp_config_t) == EECONFIG_USER_DATA_SIZE,
               "tp_config_t must exactly fill EECONFIG_USER_DATA_SIZE");

static tp_config_t tp;

static void tp_load_defaults(void) {
    tp.version     = TP_CFG_VERSION;
    tp.speed       = TP_DEF_SPEED;
    tp.jitter      = TP_DEF_JITTER;
    tp.slow_limit  = TP_DEF_SLOW_LIMIT;
    tp.slow_speed  = TP_DEF_SLOW_SPEED;
    tp.fast_speed  = TP_DEF_FAST_SPEED;
    tp.scroll_div  = TP_DEF_SCROLL_DIV;
    tp.smoothing   = TP_DEF_SMOOTHING;
    tp.lift        = TP_DEF_LIFT;
    tp.reserved[0] = 0;
    tp.reserved[1] = 0;
}

void keyboard_post_init_user(void) {
    /* Two guards, both needed. eeconfig_is_user_datablock_valid() is the
     * fork's own check that the block was ever written (quantum/eeconfig.h:153);
     * the version byte catches a block written by an OLDER layout. Either
     * failing means fall back to compiled defaults rather than loading
     * garbage -- a virgin EEPROM would otherwise give speed=0 and a dead
     * pointer with no way back but a reflash. */
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&tp, 0, sizeof(tp));
    }
    if (!eeconfig_is_user_datablock_valid() || tp.version != TP_CFG_VERSION) {
        tp_load_defaults();
    }
}

/* ------------------------------------------------------------------------ */

static inline mouse_xy_report_t trackpad_curve(int16_t delta) {
    if (delta == 0) return 0;

    int16_t sign  = (delta > 0) ? 1 : -1;
    int16_t abs_d = (delta > 0) ? delta : -delta;

    if (abs_d <= tp.jitter)
        return 0;

    int32_t result;
    if (abs_d <= tp.slow_limit) {
        result = ((int32_t)abs_d * tp.slow_speed) / 100;
    } else {
        int32_t slow_part = ((int32_t)tp.slow_limit * tp.slow_speed) / 100;
        int32_t fast_part = ((int32_t)(abs_d - tp.slow_limit) * tp.fast_speed) / 100;
        result = slow_part + fast_part;
    }

    /* Global multiplier last, so it scales the whole curve rather than one
     * zone. Widened to int32 because 300% of a large delta overflows int16. */
    result = (result * tp.speed) / 100;

    /* Never round a real movement down to nothing -- that is indistinguishable
     * from the jitter gate and makes slow tracking feel dead. */
    if (result == 0) result = 1;

    return (mouse_xy_report_t)(sign * result);
}

/*
 * EMA smoothing, applied to the RAW delta before the curve.
 *
 * out += (in - out) >> k, with the accumulator scaled by TP_EMA_SCALE so that
 * small deltas do not truncate to zero on the way in. k = 0 is a true bypass:
 * the shift is a no-op and out lands exactly on in.
 */
#define TP_EMA_SCALE 16

static int32_t ema_x = 0, ema_y = 0;

static inline int16_t tp_smooth(int32_t *acc, int16_t in) {
    if (tp.smoothing == 0) {
        *acc = (int32_t)in * TP_EMA_SCALE;
        return in;
    }
    const int32_t target = (int32_t)in * TP_EMA_SCALE;
    *acc += (target - *acc) >> tp.smoothing;
    return (int16_t)(*acc / TP_EMA_SCALE);
}

/*
 * Scroll: the driver's divisor is forced to 1 in config.h, so the raw value
 * arrives here and we divide by tp.scroll_div ourselves, carrying the
 * remainder the way digitizer_mouse_fallback.c:220-224 does -- otherwise every
 * sub-divisor movement is discarded and slow scrolling never moves at all.
 * The +/-2 clamp is kept AFTER division, as before.
 */
#define SCROLL_MAX_PER_REPORT  2   /* max scroll ticks per HID report */

static int16_t scroll_rem_h = 0, scroll_rem_v = 0;

static inline int8_t clamp_scroll(int8_t val) {
    if (val > SCROLL_MAX_PER_REPORT)  return SCROLL_MAX_PER_REPORT;
    if (val < -SCROLL_MAX_PER_REPORT) return -SCROLL_MAX_PER_REPORT;
    return val;
}

static inline int8_t tp_scroll(int16_t *rem, int16_t raw) {
    const int16_t div = (tp.scroll_div < 1) ? 1 : tp.scroll_div;
    *rem += raw;
    const int16_t ticks = *rem / div;
    *rem -= ticks * div;
    return clamp_scroll((int8_t)ticks);
}

/*
 * Liftoff holdback -- S2 fix CANDIDATE, default OFF.
 *
 * Theory being tested: the last frames before MXT_UP carry a garbage delta, so
 * the pointer jumps as the finger leaves. This holds each frame's movement one
 * report and drops it if the next report is empty (finger gone or still),
 * releasing it otherwise. Costs one report of latency (~3-8 ms at 132-300 Hz)
 * while enabled.
 *
 * REASONED, not measured: an all-zero report is used as the lift signal
 * because report_mouse_t carries no tip state. Flash 1's console log is what
 * confirms or kills the premise -- look for the bogus final delta BEFORE
 * flipping this on, per PLAN.md's experiment order.
 */
static mouse_xy_report_t held_x = 0, held_y = 0;
static bool             holding = false;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    const int16_t raw_x = mouse_report.x;
    const int16_t raw_y = mouse_report.y;

    mouse_report.x = trackpad_curve(tp_smooth(&ema_x, mouse_report.x));
    mouse_report.y = trackpad_curve(tp_smooth(&ema_y, mouse_report.y));
    mouse_report.h = tp_scroll(&scroll_rem_h, -mouse_report.h);
    mouse_report.v = tp_scroll(&scroll_rem_v, -mouse_report.v);

    if (tp.lift) {
        const bool moving = (mouse_report.x != 0 || mouse_report.y != 0);
        if (moving) {
            const mouse_xy_report_t out_x = holding ? held_x : 0;
            const mouse_xy_report_t out_y = holding ? held_y : 0;
            held_x  = mouse_report.x;
            held_y  = mouse_report.y;
            holding = true;
            mouse_report.x = out_x;
            mouse_report.y = out_y;
        } else {
            /* Candidate lift: discard whatever we were holding. */
            held_x = held_y = 0;
            holding = false;
        }
    }

#ifdef CONSOLE_ENABLE
    /* Turns "it feels jittery" into numbers. Only meaningful with
     * `qmk console` attached; a no-op otherwise. The pair (raw -> out) is what
     * answers S1: jitter with the finger HELD STILL means a noisy raw signal
     * and a sensor-side fix; jitter only WHILE MOVING means hysteresis or the
     * curve. Those have opposite fixes -- do not guess. */
    if (raw_x || raw_y || mouse_report.x || mouse_report.y) {
        uprintf("tp raw=%d,%d out=%d,%d hv=%d,%d\n",
                raw_x, raw_y, mouse_report.x, mouse_report.y,
                mouse_report.h, mouse_report.v);
    }
#else
    (void)raw_x;
    (void)raw_y;
#endif

    return mouse_report;
}

/*
 * Runtime tuning keycodes (flash 1).
 *
 * Vial maps customKeycodes[i] to QK_KB_0 + i, so THIS ENUM MUST STAY IN THE
 * SAME ORDER AS vial.json's customKeycodes array. Append only; reordering
 * silently changes what every already-assigned key does.
 */
enum tp_keycodes {
    TP_SPD_UP = QK_KB_0, TP_SPD_DN,
    TP_JIT_UP,           TP_JIT_DN,
    TP_SLL_UP,           TP_SLL_DN,
    TP_SLS_UP,           TP_SLS_DN,
    TP_FSS_UP,           TP_FSS_DN,
    TP_SCR_UP,           TP_SCR_DN,
    TP_SMO_UP,           TP_SMO_DN,
    TP_LIFT_TOG,
    TP_SAVE,
    TP_RST,
    TP_DUMP,
    TP_MOUSE_TOG,
};

/* Clamp helpers -- deliberately saturating rather than wrapping, so holding a
 * key down parks a knob at its limit instead of jumping to the other end. */
#define TP_ADD_U8(field, step, lo, hi)                                    \
    do {                                                                  \
        int32_t v = (int32_t)(tp.field) + (step);                         \
        if (v < (lo)) v = (lo);                                           \
        if (v > (hi)) v = (hi);                                           \
        tp.field = (uint8_t)v;                                            \
    } while (0)

#define TP_ADD_U16(field, step, lo, hi)                                   \
    do {                                                                  \
        int32_t v = (int32_t)(tp.field) + (step);                         \
        if (v < (lo)) v = (lo);                                           \
        if (v > (hi)) v = (hi);                                           \
        tp.field = (uint16_t)v;                                           \
    } while (0)

extern bool digitizer_send_mouse_reports;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case TP_SPD_UP: TP_ADD_U8(speed,       10,  50, 200); return false;
        case TP_SPD_DN: TP_ADD_U8(speed,      -10,  50, 200); return false;
        case TP_JIT_UP: TP_ADD_U8(jitter,       1,   0,  15); return false;
        case TP_JIT_DN: TP_ADD_U8(jitter,      -1,   0,  15); return false;
        case TP_SLL_UP: TP_ADD_U8(slow_limit,   1,   1,  30); return false;
        case TP_SLL_DN: TP_ADD_U8(slow_limit,  -1,   1,  30); return false;
        case TP_SLS_UP: TP_ADD_U8(slow_speed,   5,  10, 100); return false;
        case TP_SLS_DN: TP_ADD_U8(slow_speed,  -5,  10, 100); return false;
        case TP_FSS_UP: TP_ADD_U16(fast_speed, 10,  50, 300); return false;
        case TP_FSS_DN: TP_ADD_U16(fast_speed,-10,  50, 300); return false;
        case TP_SCR_UP: TP_ADD_U8(scroll_div,   5,   5, 100); return false;
        case TP_SCR_DN: TP_ADD_U8(scroll_div,  -5,   5, 100); return false;
        case TP_SMO_UP: TP_ADD_U8(smoothing,    1,   0,   7); return false;
        case TP_SMO_DN: TP_ADD_U8(smoothing,   -1,   0,   7); return false;

        case TP_LIFT_TOG:
            tp.lift = !tp.lift;
            held_x = held_y = 0;
            holding = false;
            return false;

        /* Save is EXPLICIT, never automatic on every press: EEPROM wear, and
         * you want to experiment freely and TP_RST back out. */
        case TP_SAVE:
            eeconfig_update_user_datablock(&tp, 0, sizeof(tp));
            return false;

        case TP_RST:
            tp_load_defaults();
            eeconfig_update_user_datablock(&tp, 0, sizeof(tp));
            held_x = held_y = 0;
            holding = false;
            ema_x = ema_y = 0;
            scroll_rem_h = scroll_rem_v = 0;
            return false;

        case TP_DUMP: {
            /* Types the live values into whatever field has focus, so a tuning
             * session can be pasted straight into debug/sessions/. */
            char buf[80];
            snprintf(buf, sizeof(buf),
                     "spd=%u jit=%u sll=%u sls=%u fss=%u scr=%u smo=%u lift=%u",
                     (unsigned)tp.speed, (unsigned)tp.jitter,
                     (unsigned)tp.slow_limit, (unsigned)tp.slow_speed,
                     (unsigned)tp.fast_speed, (unsigned)tp.scroll_div,
                     (unsigned)tp.smoothing, (unsigned)tp.lift);
            send_string(buf);
            return false;
        }

        /* Recovery path: a crashed host script can leave mouse reports off,
         * and this is the only way back without a reflash. Deliberately NOT
         * persisted -- it should come back on at every boot. */
        case TP_MOUSE_TOG:
            digitizer_send_mouse_reports = !digitizer_send_mouse_reports;
            return false;
    }

    return true;
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
