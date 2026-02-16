# Sofle Custom Trackpad

A Sofle-based split keyboard with an integrated [Procyon](https://github.com/george-norton/procyon) trackpad, RGB matrix lighting, and rotary encoder.

* Keyboard Maintainer: [foodforarabbit](https://github.com/foodforarabbit)
* Hardware Supported: RP2040-based Sofle split PCB with Procyon 57x80 trackpad on the right half
* Based on: [KafkaSplit](https://github.com/LXF-YZP/KafkaSplit) by LXF-YZP (yuezp)
* Trackpad hardware: [george-norton/procyon](https://github.com/george-norton/procyon)

## Features

* Split ergonomic layout (4x6 + 6 thumb keys per side)
* RP2040 processor
* RGB matrix with full animation support
* Rotary encoder
* Integrated trackpad (Procyon / MaxTouch digitizer) on the right half
* Full-duplex serial communication between halves
* VIA support for easy keymap configuration
* Caps Word support

## Prerequisites

**Important:** This keyboard uses the MaxTouch digitizer driver for the Procyon trackpad. This driver is **not part of mainline QMK**. You must use [george-norton's QMK firmware fork](https://github.com/george-norton/qmk_firmware/tree/multitouch_experiment) (the `multitouch_experiment` branch) which includes the MaxTouch driver.

## Build Environment Setup

### 1. Install QMK CLI

On macOS:

    brew install qmk/qmk/qmk

Then run the initial setup (clones the QMK repo and installs the ARM toolchain):

    qmk setup -H ~/qmk_firmware

### 2. Switch to george-norton's QMK fork

The standard QMK repo does not have the MaxTouch driver. Add george-norton's fork as a remote and check out the required branch:

    cd ~/qmk_firmware
    git remote add george-norton https://github.com/george-norton/qmk_firmware.git
    git fetch george-norton multitouch_experiment
    git checkout george-norton/multitouch_experiment -b multitouch_experiment
    make git-submodule

### 3. Symlink this repo into the QMK tree

QMK expects keyboard files inside its `keyboards/` directory. Create a symlink so that the QMK build system can find this repo directly -- this way you always edit files in this repo and changes are immediately available for compilation:

    mkdir -p ~/qmk_firmware/keyboards/foodforarabbit
    ln -s /path/to/sofle_procyon ~/qmk_firmware/keyboards/foodforarabbit/sofle_custom_trackpad

Replace `/path/to/sofle_procyon` with the absolute path to this repository (e.g. `$HOME/Downloads/sofle_procyon`).

You can verify it worked:

    ls ~/qmk_firmware/keyboards/foodforarabbit/sofle_custom_trackpad/keyboard.json

### 4. Add the ARM toolchain to your PATH

The Homebrew-installed ARM toolchain needs to be on your PATH for compilation. Add this to your shell profile (`~/.zshrc`), or run it before each compile session:

    export PATH="/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-binutils/bin:$PATH"

### 5. Compile the firmware

    cd ~/qmk_firmware
    qmk compile -kb foodforarabbit/sofle_custom_trackpad -km default

If successful, this produces a `.uf2` file in the QMK firmware root directory (e.g. `foodforarabbit_sofle_custom_trackpad_default.uf2`).

## Backing Up Current Firmware

Before flashing new firmware, you can back up what is currently on the keyboard using `picotool`.

### 1. Install picotool

On macOS:

    brew install picotool

### 2. Enter bootloader mode

See the "Entering Bootloader" section below.

### 3. Save the current firmware

Once the keyboard is in bootloader mode (the `RPI-RP2` drive appears):

    picotool save -a backup.uf2

This dumps the entire flash contents to `backup.uf2`. Keep this file safe -- you can restore it later by copying it back to the `RPI-RP2` drive.

## Flashing

This is a **split keyboard** -- both halves have their own RP2040 controller and **both must be flashed separately** with the same firmware file.

For each half:

1. Disconnect the serial/TRRS cable between the halves
2. Enter bootloader mode on the half you want to flash (see below)
3. Flash using `picotool` (recommended):

        picotool load foodforarabbit_sofle_custom_trackpad_default.uf2
        picotool reboot

4. Repeat for the other half
5. Reconnect the serial/TRRS cable and plug USB into the left half

Alternatively, you can copy the `.uf2` file to the `RPI-RP2` drive that appears in Finder, but `picotool` is more reliable on macOS.

## Entering Bootloader

There are several ways to enter the bootloader:

* **Bootmagic reset**: Hold down the **ESC key** (top-left key, matrix position 0,0) and plug in the USB cable. The keyboard will enter bootloader mode instead of booting normally.
* **Double-tap reset**: If you have access to the reset button, double-tap it within 500ms to enter the RP2040 UF2 bootloader.
* **Physical reset button**: Hold the BOOT button while pressing RESET (or while plugging in USB).
* **Via VIA**: If you have VIA connected, you can remap any key to `QK_BOOT` (under the "Special" category), then press that key to enter bootloader mode. Note: `QK_BOOT` is **not** mapped in the default keymap.

## VIA Support

This keyboard has VIA support enabled. To use VIA for keymap customization:

1. Go to [usevia.app](https://usevia.app/) in a Chromium-based browser (Chrome, Edge, Brave, etc.)
2. Enable the Design tab: click the gear icon (Settings) and toggle **Show Design Tab**
3. Go to the **Design** tab and click **Load Draft Definition**
4. Select the `via_definition.json` file from this repository
5. Go back to the **Configure** tab -- your keyboard should now be detected
6. Remap keys, configure layers, and adjust lighting as needed

Changes made in VIA are saved directly to the keyboard's EEPROM and persist across reboots.

## Trackpad & Scroll Tuning

The default keymap applies an Apple-like acceleration curve to the trackpad cursor and scroll. All tuning parameters live in the firmware source so they persist across flashes.

### Cursor (single-finger)

Defined in `keymaps/default/keymap.c`:

| Parameter | Default | Description |
|---|---|---|
| `TRACKPAD_JITTER_THRESHOLD` | 3 | Deltas at or below this are dropped (prevents cursor jump on finger lift-off) |
| `TRACKPAD_SLOW_LIMIT` | 12 | Deltas up to this are in the precision zone |
| `TRACKPAD_SLOW_SPEED` | 50 | Speed in the precision zone (% of raw) -- lower = more precise |
| `TRACKPAD_FAST_SPEED` | 140 | Speed above the precision zone (% of raw) -- higher = more acceleration |

### Scroll (two-finger)

Two settings work together:

* **`DIGITIZER_SCROLL_DIVISOR`** (in `config.h`, default upstream is 10, we use **35**) -- raw finger movement is divided by this before producing scroll ticks. Higher = less sensitive.
* **`SCROLL_MAX_PER_REPORT`** (in `keymaps/default/keymap.c`, default **2**) -- clamps the maximum scroll ticks per HID report, preventing large jumps on fast swipes.

### Hardware Sensitivity

The Procyon trackpad uses a Microchip MaxTouch (mXT336UD) sensor IC. The sensor sensitivity may need tuning depending on the surface material over the trackpad.

* If the trackpad is not detecting touches well, reduce the touch threshold
* If the trackpad is jittery or detecting false touches, increase the touch threshold
* If there is ungrounded metal near the sensor, move it away

For real-time tuning, you can build a debug firmware (`MAXTOUCH_DEBUG = yes` in `rules.mk`) and use the [maxtouch-debug](https://github.com/george-norton/maxtouch-debug) tool.

For more information about the trackpad hardware and tuning, see the [Procyon documentation](https://github.com/george-norton/procyon).
