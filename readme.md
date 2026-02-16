# Sofle Custom Trackpad

A Sofle-based split keyboard with integrated trackpad support, RGB matrix lighting, and rotary encoder.

* Keyboard Maintainer: [foodforarabbit](https://github.com/foodforarabbit)
* Hardware Supported: RP2040-based Sofle split PCB with Procyon trackpad
* Hardware Availability: [foodforarabbit/sofle_custom_trackpad](https://github.com/foodforarabbit/sofle_custom_trackpad)

## Features

* Split ergonomic layout (4x6 + 6 thumb keys per side)
* RP2040 processor
* RGB matrix with full animation support
* Rotary encoder
* Integrated trackpad (maxtouch digitizer) on the right half
* Full-duplex serial communication between halves
* VIA support for easy keymap configuration

## Building

Make example for this keyboard (after setting up your build environment):

    make foodforarabbit/sofle_custom_trackpad:default

Flashing example for this keyboard:

    make foodforarabbit/sofle_custom_trackpad:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Double-tap reset**: Double-tap the reset button within 500ms to enter the RP2040 bootloader
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
