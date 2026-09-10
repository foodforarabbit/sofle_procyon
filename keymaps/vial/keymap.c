

#include QMK_KEYBOARD_H
#include <stdio.h>   /* snprintf, for TP_DUMP */
#include "print.h"   /* uprintf; no-op without CONSOLE_ENABLE */
#include "i2c_master.h"
#include "drivers/sensors/maxtouch.h"

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
/*
 * FLASH 2 defaults = the values Ryan tuned and saved live over flash 1, so a
 * fresh EEPROM lands on the feel he already approved rather than back at stock.
 *
 * Measured route to each of these is in debug/sessions/2026-09-10-0*.md.
 * The two that came from measurement rather than preference:
 *   slow_limit 12 -> 20  because the Y axis produces ~1.67x the counts of X for
 *                        the same hand speed, so vertical movement was crossing
 *                        into the fast zone during what felt like slow movement
 *                        and getting amplified 1.4x.
 *   jitter      5 -> 7   brute-force suppression of direction reversals. The
 *                        directional gate below is the better tool; once it is
 *                        proven, try JITX-/JITY- back down to restore precision.
 */
#define TP_DEF_SPEED       60
#define TP_DEF_JITTER_X     7
#define TP_DEF_JITTER_Y     7
#define TP_DEF_SLOW_LIMIT_X 20
#define TP_DEF_SLOW_LIMIT_Y 20
#define TP_DEF_SLOW_SPEED   50
#define TP_DEF_FAST_SPEED  140
#define TP_DEF_SCROLL_DIV  100
#define TP_DEF_SMOOTHING     0
#define TP_DEF_DIR_OPPOSE    3

/* flags */
#define TPF_LIFT        0x01   /* 1-frame liftoff holdback (S2 candidate)  */
#define TPF_SCRINV      0x02   /* invert scroll direction                  */
#define TPF_DIRGATE     0x04   /* directional gate -- the flash-2 headline */
#define TPF_SMOOTHCURVE 0x08   /* continuous gain instead of piecewise     */

/* DIRGATE on, everything else off. The gate is the reason for this flash, so
 * it ships enabled; the other two change feel and are opt-in so a bad guess
 * costs a keypress instead of a reflash. */
#define TP_DEF_FLAGS (TPF_DIRGATE)

/* Bumped from 1: the struct grew per-axis fields. A block saved by flash 1
 * fails the version check and falls back to the defaults above -- which ARE
 * his saved values, so the reset is invisible. */
#define TP_CFG_VERSION 2

typedef struct __attribute__((packed)) {
    uint8_t  version;
    uint8_t  speed;         /*  50..200 % */
    uint8_t  jitter_x;      /*   0..15    */
    uint8_t  jitter_y;      /*   0..15    */
    uint8_t  slow_limit_x;  /*   1..40    */
    uint8_t  slow_limit_y;  /*   1..40    */
    uint8_t  slow_speed;    /*  10..100 % */
    uint16_t fast_speed;    /*  50..300 % */
    uint8_t  scroll_div;    /*   5..100   */
    uint8_t  smoothing;     /*   0..7     */
    uint8_t  flags;
    uint8_t  dir_oppose;    /*   2..8  multiplier on the opposing threshold */
    uint8_t  reserved[3];
} tp_config_t;

_Static_assert(sizeof(tp_config_t) == EECONFIG_USER_DATA_SIZE,
               "tp_config_t must exactly fill EECONFIG_USER_DATA_SIZE");

static tp_config_t tp;

static void tp_load_defaults(void) {
    tp.version      = TP_CFG_VERSION;
    tp.speed        = TP_DEF_SPEED;
    tp.jitter_x     = TP_DEF_JITTER_X;
    tp.jitter_y     = TP_DEF_JITTER_Y;
    tp.slow_limit_x = TP_DEF_SLOW_LIMIT_X;
    tp.slow_limit_y = TP_DEF_SLOW_LIMIT_Y;
    tp.slow_speed   = TP_DEF_SLOW_SPEED;
    tp.fast_speed   = TP_DEF_FAST_SPEED;
    tp.scroll_div   = TP_DEF_SCROLL_DIV;
    tp.smoothing    = TP_DEF_SMOOTHING;
    tp.flags        = TP_DEF_FLAGS;
    tp.dir_oppose   = TP_DEF_DIR_OPPOSE;
    tp.reserved[0] = tp.reserved[1] = tp.reserved[2] = 0;
}


/*
 * SENSOR TUNING AT BOOT -- the flash-2 jitter fix.
 *
 * movhystn ships at 16 against a driver default of 4. Next-move hysteresis
 * batches sub-threshold drift and releases it in quantised 17-unit dumps;
 * live-writing 4 took the worst pointer jump from 15px to 4px and halved the
 * escape rate (debug/sessions/2026-09-10-01.md).
 *
 * It has to be applied HERE rather than as a config.h define, because
 * procyon.h:30 defines it unguarded and a redefine is a -Werror failure. And
 * it has to be applied at BOOT rather than once by hand, because the driver
 * rewrites sensor config from compiled values at every power cycle -- measured
 * from both sides on flash 1.
 *
 * The T100 address is per-device and must be read from the object table, never
 * hardcoded. Address bytes go through SWAP_BYTES the way maxtouch.c:220 does.
 */
#define TP_SWAP16(a) ((uint16_t)((((a) << 8) & 0xff00) | (((a) >> 8) & 0xff)))
#define TP_T100_MOVHYSTN_OFFSET 49   /* PACKED mxt_touch_multiscreen_t100 */
#define TP_MOVHYSTN_VALUE        4

static uint8_t tp_sensor_tries = 0;

static bool tp_apply_sensor_tuning(void) {
    uint8_t info[7];
    if (i2c_read_register16(MXT336UD_ADDRESS, TP_SWAP16(0x0000), info, sizeof(info),
                            MXT_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
        return false;
    }
    const uint8_t num_objects = info[6];

    for (uint8_t i = 0; i < num_objects; i++) {
        uint8_t e[6];
        if (i2c_read_register16(MXT336UD_ADDRESS, TP_SWAP16(7 + i * 6), e, sizeof(e),
                                MXT_I2C_TIMEOUT_MS) != I2C_STATUS_SUCCESS) {
            return false;
        }
        if (e[0] != 100) continue;

        const uint16_t t100 = (uint16_t)e[1] | ((uint16_t)e[2] << 8);
        uint8_t v[2] = {TP_MOVHYSTN_VALUE, 0};   /* uint16, little-endian */
        return i2c_write_register16(MXT336UD_ADDRESS,
                                    TP_SWAP16(t100 + TP_T100_MOVHYSTN_OFFSET),
                                    v, sizeof(v), MXT_I2C_TIMEOUT_MS)
               == I2C_STATUS_SUCCESS;
    }
    return false;   /* no T100 -- nothing to tune */
}

void keyboard_post_init_user(void) {
    /* Two guards. eeconfig_is_user_datablock_valid() is the fork's own check
     * that the block was ever written; the version byte catches one written by
     * an older layout. Either failing means compiled defaults, because a
     * virgin block reads 0x00/0xFF and would otherwise load speed=0 and leave
     * a dead pointer recoverable only by reflashing. */
    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(&tp, 0, sizeof(tp));
    }
    if (!eeconfig_is_user_datablock_valid() || tp.version != TP_CFG_VERSION) {
        tp_load_defaults();
    }

    if (!tp_apply_sensor_tuning()) {
        tp_sensor_tries = 1;   /* retried from the pointing task below */
    }
}

/* ------------------------------------------------------------------------ */
/*
 * THE DIRECTIONAL GATE -- the point of flash 2.
 *
 * MEASURED over ~30k reports on flash 1: slow movement reversed direction on
 * up to 7% of reports, while fast movement reversed on 0.2%. Position on the
 * pad was irrelevant (tested both pass orders; the apparent gradient was hand
 * speed). So the noise is a roughly fixed-size error: when real deltas are 5 it
 * flips the sign, when they are 10 it does not.
 *
 * A magnitude gate cannot fix that, and flash 1 proved it empirically. Raising
 * the threshold to 7 killed the reversals and made slow movement sluggish;
 * raising slow-zone GAIN to compensate brought the jitter straight back,
 * because in that zone gain multiplies signal and noise equally. One knob was
 * doing two jobs and could not do both.
 *
 * The fix is a different discriminator. Real movement is directionally
 * CONSISTENT; noise reverses. So remember the last accepted direction per axis
 * and make an opposing delta clear a higher bar than an agreeing one. Small
 * forward movement passes (precision preserved); small backward movement is
 * dropped (jitter suppressed). No gain change, so no trade.
 *
 * Reset on contact loss, or the direction from the last stroke biases the
 * first report of the next one.
 */
static int8_t  dir_x = 0, dir_y = 0;

static inline int16_t dir_gate(int16_t d, int8_t *dir, uint8_t jitter) {
    if (d == 0) return 0;

    const int8_t  s = (d > 0) ? 1 : -1;
    const int16_t a = (d > 0) ? d : -d;

    uint16_t thr = jitter;
    if ((tp.flags & TPF_DIRGATE) && *dir != 0 && s != *dir) {
        thr = (uint16_t)jitter * tp.dir_oppose;
    }
    if (a <= thr) return 0;

    *dir = s;
    return d;
}

/*
 * Gain curve.
 *
 * PIECEWISE (default, and what Ryan approved on flash 1): flat slow_speed up to
 * slow_limit, flat fast_speed above.
 *
 * SMOOTH (opt-in via TPF_SMOOTHCURVE): gain ramps linearly from slow_speed at
 * rest to fast_speed at slow_limit, then holds. Ryan described the piecewise
 * curve as feeling like "a ledge" -- with speed=60 the marginal gain steps
 * 30% -> 84% at the boundary, a 2.8x jump the hand can feel. The ramp removes
 * the step. It is opt-in because it changes feel everywhere, and the piecewise
 * setting is the one he has actually signed off.
 */
static inline mouse_xy_report_t trackpad_curve(int16_t delta, uint8_t slow_limit) {
    if (delta == 0) return 0;

    const int16_t sign  = (delta > 0) ? 1 : -1;
    const int16_t abs_d = (delta > 0) ? delta : -delta;
    const int16_t sll   = (slow_limit < 1) ? 1 : slow_limit;

    int32_t result;
    if (tp.flags & TPF_SMOOTHCURVE) {
        const int32_t capped = (abs_d < sll) ? abs_d : sll;
        const int32_t gain   = (int32_t)tp.slow_speed
                             + ((int32_t)tp.fast_speed - tp.slow_speed) * capped / sll;
        result = (int32_t)abs_d * gain / 100;
    } else if (abs_d <= sll) {
        result = ((int32_t)abs_d * tp.slow_speed) / 100;
    } else {
        const int32_t slow_part = ((int32_t)sll * tp.slow_speed) / 100;
        const int32_t fast_part = ((int32_t)(abs_d - sll) * tp.fast_speed) / 100;
        result = slow_part + fast_part;
    }

    /* Global multiplier last, so it scales the whole curve. int32 throughout:
     * 300% of a large delta overflows int16. */
    result = (result * tp.speed) / 100;

    /* Never round a real movement to nothing -- that is indistinguishable from
     * the gate and makes slow tracking feel dead. */
    if (result == 0) result = 1;

    return (mouse_xy_report_t)(sign * result);
}

/*
 * EMA smoothing on the RAW delta. k=0 is a true bypass.
 *
 * Kept, but NOTE THE MEASURED WARNING: the sensor's own position-domain
 * smoothing (T100 movsmooth) made reversals ~15x WORSE, because smoothing a
 * position and then differencing it amplifies noise in the derivative. This
 * smooths the DELTA, which is a different operation and does not have that
 * failure mode -- but it is still off by default and should be measured, not
 * assumed.
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
 * Scroll. The driver's divisor is forced to 1 in config.h so the raw value
 * arrives here and we divide by tp.scroll_div, carrying the remainder the way
 * digitizer_mouse_fallback.c:215-224 does -- without the carry, every
 * sub-divisor movement is discarded and slow scrolling never moves.
 *
 * FIXED IN FLASH 2: the remainder is now reset on contact loss. It was static
 * with no reset (the driver has the same flaw), so leftover from one gesture
 * leaked into the next and could pull its first tick the wrong way. Bounded at
 * +/-(divisor-1), so it was one wrong tick, not a reversal -- but it is real.
 */
#define SCROLL_MAX_PER_REPORT  2

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
 * Liftoff holdback -- S2 candidate, still default OFF.
 *
 * MEASURED across 28 lifts on flash 1: 10 of 28 spiked 3-8x a normal frame,
 * worst 38. But the peak is NOT always the final frame -- one sample ran
 * 16, 37, 21, 15 -- so a 1-frame holdback drops the wrong report in those
 * cases. It is kept as-is rather than extended, because the honest answer is
 * that the shape varies and a fixed-length holdback is the wrong instrument.
 * Left toggleable so it can be measured; not enabled on a guess.
 */
static mouse_xy_report_t held_x = 0, held_y = 0;
static bool             holding = false;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    /* Retry the boot-time sensor write for a few cycles. If i2c was not ready
     * at post-init the write silently does nothing, and a silent no-op here
     * means the jitter fix is absent with no indication -- which is exactly the
     * class of failure that made movhystn hard to find in the first place. */
    if (tp_sensor_tries && tp_sensor_tries < 20) {
        if (tp_apply_sensor_tuning()) {
            tp_sensor_tries = 0;
#ifdef CONSOLE_ENABLE
            uprintf("tp sensor tuning applied on retry\n");
#endif
        } else {
            tp_sensor_tries++;
        }
    }

    const int16_t raw_x = mouse_report.x;
    const int16_t raw_y = mouse_report.y;
    const int16_t raw_h = mouse_report.h;
    const int16_t raw_v = mouse_report.v;

    /* Contact loss: clear every piece of cross-stroke state. Direction memory
     * would bias the next stroke's first report; scroll remainder would leak a
     * tick into the next gesture. */
    if (raw_x == 0 && raw_y == 0 && raw_h == 0 && raw_v == 0) {
        dir_x = dir_y = 0;
        scroll_rem_h = scroll_rem_v = 0;
        held_x = held_y = 0;
        holding = false;
    }

    const int16_t gx = dir_gate(tp_smooth(&ema_x, mouse_report.x), &dir_x, tp.jitter_x);
    const int16_t gy = dir_gate(tp_smooth(&ema_y, mouse_report.y), &dir_y, tp.jitter_y);

    mouse_report.x = trackpad_curve(gx, tp.slow_limit_x);
    mouse_report.y = trackpad_curve(gy, tp.slow_limit_y);

    int16_t sh = -mouse_report.h, sv = -mouse_report.v;
    if (tp.flags & TPF_SCRINV) { sh = -sh; sv = -sv; }
    mouse_report.h = tp_scroll(&scroll_rem_h, sh);
    mouse_report.v = tp_scroll(&scroll_rem_v, sv);

    if (tp.flags & TPF_LIFT) {
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
            held_x = held_y = 0;
            holding = false;
        }
    }

#ifdef CONSOLE_ENABLE
    /* FIXED IN FLASH 2: scroll-only reports are logged. The flash-1 condition
     * gated on x/y alone, so a two-finger scroll -- where the driver zeroes
     * x/y -- never logged at all. 1,127 reports were captured during active
     * scrolling with ZERO hv values, which is why the scroll-direction
     * question could not be answered from the log.
     * `g=` is the post-gate, pre-curve delta: the pair (raw -> g) shows what
     * the directional gate dropped, which is the whole thing being tested. */
    if (raw_x || raw_y || raw_h || raw_v ||
        mouse_report.x || mouse_report.y || mouse_report.h || mouse_report.v) {
        uprintf("tp raw=%d,%d g=%d,%d out=%d,%d hv=%d,%d\n",
                raw_x, raw_y, gx, gy,
                mouse_report.x, mouse_report.y,
                mouse_report.h, mouse_report.v);
    }
#endif

    return mouse_report;
}

/*
 * Runtime tuning keycodes.
 *
 * Vial maps customKeycodes[i] to QK_KB_0 + i, so THIS ENUM MUST STAY IN THE
 * SAME ORDER AS vial.json. Indices 0-18 are UNCHANGED from flash 1 on purpose:
 * Ryan has SPD-, SCR+, TPSV and TPDU already assigned to physical keys, and
 * reordering would silently repoint them. Everything new is APPENDED.
 *
 * JIT+/- and SLL+/- now move BOTH axes together; the per-axis keys are the
 * appended ones. That keeps the old keys meaningful rather than dead.
 */
enum tp_keycodes {
    TP_SPD_UP = QK_KB_0, TP_SPD_DN,   /*  0  1 */
    TP_JIT_UP,           TP_JIT_DN,   /*  2  3  both axes */
    TP_SLL_UP,           TP_SLL_DN,   /*  4  5  both axes */
    TP_SLS_UP,           TP_SLS_DN,   /*  6  7 */
    TP_FSS_UP,           TP_FSS_DN,   /*  8  9 */
    TP_SCR_UP,           TP_SCR_DN,   /* 10 11 */
    TP_SMO_UP,           TP_SMO_DN,   /* 12 13 */
    TP_LIFT_TOG,                      /* 14 */
    TP_SAVE,                          /* 15 */
    TP_RST,                           /* 16 */
    TP_DUMP,                          /* 17 */
    TP_MOUSE_TOG,                     /* 18 */
    /* ---- appended in flash 2 ---- */
    TP_SCR_INV,                       /* 19 */
    TP_DIR_TOG,                       /* 20 */
    TP_CURVE_TOG,                     /* 21 */
    TP_DOP_UP,           TP_DOP_DN,   /* 22 23 */
    TP_JITX_UP,          TP_JITX_DN,  /* 24 25 */
    TP_JITY_UP,          TP_JITY_DN,  /* 26 27 */
    TP_SLLX_UP,          TP_SLLX_DN,  /* 28 29 */
    TP_SLLY_UP,          TP_SLLY_DN,  /* 30 31 */
};

/* Saturating, not wrapping: holding a key parks a knob at its limit instead of
 * jumping to the other end. */
#define TP_ADD(field, step, lo, hi)                                       \
    do {                                                                  \
        int32_t v = (int32_t)(tp.field) + (step);                         \
        if (v < (lo)) v = (lo);                                           \
        if (v > (hi)) v = (hi);                                           \
        tp.field = v;                                                     \
    } while (0)

extern bool digitizer_send_mouse_reports;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
        case TP_SPD_UP: TP_ADD(speed,       10,  50, 200); return false;
        case TP_SPD_DN: TP_ADD(speed,      -10,  50, 200); return false;

        case TP_JIT_UP: TP_ADD(jitter_x,      1, 0, 15);
                        TP_ADD(jitter_y,      1, 0, 15); return false;
        case TP_JIT_DN: TP_ADD(jitter_x,     -1, 0, 15);
                        TP_ADD(jitter_y,     -1, 0, 15); return false;
        case TP_SLL_UP: TP_ADD(slow_limit_x,  1, 1, 40);
                        TP_ADD(slow_limit_y,  1, 1, 40); return false;
        case TP_SLL_DN: TP_ADD(slow_limit_x, -1, 1, 40);
                        TP_ADD(slow_limit_y, -1, 1, 40); return false;

        case TP_JITX_UP: TP_ADD(jitter_x,      1, 0, 15); return false;
        case TP_JITX_DN: TP_ADD(jitter_x,     -1, 0, 15); return false;
        case TP_JITY_UP: TP_ADD(jitter_y,      1, 0, 15); return false;
        case TP_JITY_DN: TP_ADD(jitter_y,     -1, 0, 15); return false;
        case TP_SLLX_UP: TP_ADD(slow_limit_x,  1, 1, 40); return false;
        case TP_SLLX_DN: TP_ADD(slow_limit_x, -1, 1, 40); return false;
        case TP_SLLY_UP: TP_ADD(slow_limit_y,  1, 1, 40); return false;
        case TP_SLLY_DN: TP_ADD(slow_limit_y, -1, 1, 40); return false;

        case TP_SLS_UP: TP_ADD(slow_speed,   5,  10, 100); return false;
        case TP_SLS_DN: TP_ADD(slow_speed,  -5,  10, 100); return false;
        case TP_FSS_UP: TP_ADD(fast_speed,  10,  50, 300); return false;
        case TP_FSS_DN: TP_ADD(fast_speed, -10,  50, 300); return false;
        case TP_SCR_UP: TP_ADD(scroll_div,   5,   5, 100); return false;
        case TP_SCR_DN: TP_ADD(scroll_div,  -5,   5, 100); return false;
        case TP_SMO_UP: TP_ADD(smoothing,    1,   0,   7); return false;
        case TP_SMO_DN: TP_ADD(smoothing,   -1,   0,   7); return false;
        case TP_DOP_UP: TP_ADD(dir_oppose,   1,   2,   8); return false;
        case TP_DOP_DN: TP_ADD(dir_oppose,  -1,   2,   8); return false;

        case TP_LIFT_TOG:
            tp.flags ^= TPF_LIFT;
            held_x = held_y = 0;
            holding = false;
            return false;

        case TP_SCR_INV:
            tp.flags ^= TPF_SCRINV;
            scroll_rem_h = scroll_rem_v = 0;   /* stale remainder has the old sign */
            return false;

        case TP_DIR_TOG:
            tp.flags ^= TPF_DIRGATE;
            dir_x = dir_y = 0;
            return false;

        case TP_CURVE_TOG:
            tp.flags ^= TPF_SMOOTHCURVE;
            return false;

        /* Save is EXPLICIT, never automatic: EEPROM wear, and you want to
         * experiment freely and TP_RST back out. */
        case TP_SAVE:
            eeconfig_update_user_datablock(&tp, 0, sizeof(tp));
            return false;

        case TP_RST:
            tp_load_defaults();
            eeconfig_update_user_datablock(&tp, 0, sizeof(tp));
            dir_x = dir_y = 0;
            held_x = held_y = 0;
            holding = false;
            ema_x = ema_y = 0;
            scroll_rem_h = scroll_rem_v = 0;
            return false;

        case TP_DUMP: {
            char buf[160];
            snprintf(buf, sizeof(buf),
                     "spd=%u jitx=%u jity=%u sllx=%u slly=%u sls=%u fss=%u "
                     "scr=%u smo=%u dop=%u dir=%u inv=%u lift=%u curve=%s",
                     (unsigned)tp.speed, (unsigned)tp.jitter_x,
                     (unsigned)tp.jitter_y, (unsigned)tp.slow_limit_x,
                     (unsigned)tp.slow_limit_y, (unsigned)tp.slow_speed,
                     (unsigned)tp.fast_speed, (unsigned)tp.scroll_div,
                     (unsigned)tp.smoothing, (unsigned)tp.dir_oppose,
                     (unsigned)!!(tp.flags & TPF_DIRGATE),
                     (unsigned)!!(tp.flags & TPF_SCRINV),
                     (unsigned)!!(tp.flags & TPF_LIFT),
                     (tp.flags & TPF_SMOOTHCURVE) ? "smooth" : "piecewise");
            send_string(buf);
            return false;
        }

        /* Recovery: a crashed host script can leave mouse reports off, and this
         * is the only way back without a reflash. Deliberately NOT persisted --
         * it should come back on at every boot. */
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
#    define MXT_TUNNEL_SWAP(a) ((uint16_t)((((a) << 8) & 0xff00) | (((a) >> 8) & 0xff)))

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
            /* SWAP_BYTES: upstream's debug handler passes this address to
             * i2c_read_register16 unswapped (maxtouch.c:643) while the SAME
             * FILE swaps it for the identical call during init (:220). maXTouch
             * wants the register pointer little-endian and i2c_read_register16
             * transmits big-endian, so without the swap every non-zero address
             * reads the wrong register -- MEASURED on flash 1: address 0 worked
             * and everything else returned zeros with status OK. Fixed here
             * rather than in vial-qmk to keep the fork at zero changes; also
             * sent upstream. debug/mxt.py probes for this per run, so it keeps
             * working against either firmware. */
            const uint16_t addr = MXT_TUNNEL_SWAP((data[1] << 8) | data[2]);
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
            /* Same byte-order bug on the write path (maxtouch.c:660). Getting
             * this wrong on a WRITE scribbles into an unrelated register. */
            const uint16_t addr = MXT_TUNNEL_SWAP((data[1] << 8) | data[2]);
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
