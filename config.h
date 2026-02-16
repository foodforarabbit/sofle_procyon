
#pragma once

/* Double tap reset button to enter bootloader */

/* Handedness. */
#define SPLIT_HAND_PIN GP29
#define SPLIT_HAND_PIN_LOW_IS_RIGHT // High -> right, Low -> left.

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP25
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// Serial config
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_DRIVER SIOD0


// Enable use of pointing device on slave split.
#define SPLIT_POINTING_ENABLE

// Pointing device is on the right split.
#define POINTING_DEVICE_RIGHT

/* CRC. */
#define CRC8_USE_TABLE
#define CRC8_OPTIMIZE_SPEED

// Scroll sensitivity: higher = less sensitive. Default is 10.
// 35 gives an Apple-trackpad-like scroll feel.
#define DIGITIZER_SCROLL_DIVISOR 35

// Tap detection: how far the finger can move (in sensor units) during a tap
// and still be recognized as a click. Default is 25, which is too tight --
// slight finger slip during a tap exceeds it and the click is lost.
#define DIGITIZER_MOUSE_TAP_DISTANCE 50

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#define I2C1_CLOCK_SPEED 1000000
#define DIGITIZER_MOTION_PIN GP11
#define DIGITIZER_MOTION_PIN_ACTIVE_LOW yes
#define PROCYON_57_80

