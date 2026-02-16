
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

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#define I2C1_CLOCK_SPEED 1000000
#define DIGITIZER_MOTION_PIN GP11
#define DIGITIZER_MOTION_PIN_ACTIVE_LOW yes
#define PROCYON_57_80


// Reduce soft serial speed: Work around rp2040 issues
#define SELECT_SOFT_SERIAL_SPEED 4
