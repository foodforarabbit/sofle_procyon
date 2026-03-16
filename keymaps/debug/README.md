# Debug Keymap

This keymap is identical to `default` but compiled with `MAXTOUCH_DEBUG = yes`,
which enables the MaxTouch debug protocol over USB HID.

## What it does

With debug firmware flashed, you can use the
[maxtouch-debug](https://github.com/george-norton/maxtouch-debug) GUI to:

- Visualize a live capacitance heatmap of the Procyon sensor
- Adjust `MXT_TOUCH_THRESHOLD`, `MXT_GAIN`, `MXT_TOUCH_HYST`, and other
  sensor registers in real time — without reflashing
- Identify jitter, liftoff, and sensitivity issues by watching raw node data

## Build

```bash
qmk compile -kb foodforarabbit/sofle_procyon -km debug
```

Flash the resulting `.uf2` to **both halves** as usual.

## Usage

1. Flash debug firmware to both halves
2. Clone and run [maxtouch-debug](https://github.com/george-norton/maxtouch-debug)
3. Connect your keyboard — the tool will detect it automatically
4. Tune threshold/gain/hysteresis live, note the values that feel best
5. Apply those values as `#define` overrides in `config.h` (see
   [Trackpad tuning params](../../readme.md#trackpad--scroll-tuning) in the
   main README)
6. Recompile and flash the `default` or `vial` keymap with your tuned values

## Key tuning parameters (set in `config.h`)

| Define | Procyon default | Effect |
|---|---|---|
| `MXT_TOUCH_THRESHOLD` | 20 | Raise to reduce jitter / false touches |
| `MXT_TOUCH_HYST` | 10 | Raise for cleaner liftoff (hysteresis below threshold) |
| `MXT_GAIN` | 10 | Lower to reduce overall sensitivity |
| `MXT_MOVE_HYSTERESIS_NEXT` | 16 | Raise to suppress mid-track jitter |
| `MXT_LOW_PASS_FILTER_COEFFICIENT` | 0 | Set 5–15 to smooth out noisy movement |
| `MXT_CONFTHR` | 5 | Raise for more debounce on touch down/up |

> **Note:** Debug firmware is larger and slightly slower than the normal build.
> Always reflash with `default` or `vial` after tuning.
