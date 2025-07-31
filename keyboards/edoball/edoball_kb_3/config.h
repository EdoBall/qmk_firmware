// Copyright 2022 Stefan Kerkmann
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "config_common.h"

// moved to json.info
//#define VENDOR_ID    0xFEED
//#define PRODUCT_ID   0x0004
//#define DEVICE_VER   0x0004
//#define MANUFACTURER EdoBall
//#define PRODUCT Test

// for getting a macro's value.
#define XXSTR(x) TTSTR(x)
#define TTSTR(x) #x

#define DEBUG_MATRIX_SCAN_RATE

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP25
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#pragma once

#define MATRIX_COL_PINS { GP27, GP26, GP22, GP21, GP20, GP19, GP18, GP17 }
//                          32    31    29    27    26    25    24    22 (Left or Right edge to Center)
#define MATRIX_ROW_PINS {  GP8,  GP9, GP10, GP11, GP12, NO_PIN}
//                          11    12    14    15    16   (17) (The other side to this side (Body))

// Matrix Rows: 6 * 2 (split keyboard)
#define MATRIX_ROWS 12
#define MATRIX_COLS 8

#define DIODE_DIRECTION COL2ROW

#define DEBOUNCE 5

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

#define SERIAL_DEBUG

#define WS2812_PIO_USE_PIO1

// touch keys
#define TK0OUT GP14
#define TK0IN GP15

// #define RGB_DI_PIN GP6
#ifdef RGB_DI_PIN
  #define RGBLED_NUM 12
  #define RGBLIGHT_SPLIT
  #define RGBLED_SPLIT { 6, 6 }
  #define RGBLIGHT_LED_MAP { 0, 1, 2, 3, 4, 5, \
                             11, 10, 9, 8, 7, 6 }
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
  #define RGBLIGHT_LIMIT_VAL 255
  #define RGBLIGHT_SLEEP
  #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#endif

#define TAPPING_TERM 500
#define PERMISSIVE_HOLD

// ADNS5050
#define ADNS5050_SCLK_PIN GP4
// 6 (Green)
// default: POINTING_DEVICE_SCLK_PIN
#define ADNS5050_SDIO_PIN GP5
// 7 (Blue)
// default: POINTING_DEVICE_SDIO_PIN
// 8 GND, Black
#define ADNS5050_CS_PIN GP6
// 9 (Yellow)
// default: POINTING_DEVICE_CS_PIN


// in rules.mk: SPLIT_KEYBOARD = yes -> SPLIT_KEYBOARD == 1
//                             = no  -> SPLIT_KEYBOARD is not defined.
#if SPLIT_KEYBOARD == 1
// #pragma message XXSTR(SPLIT_KEYBOARD)
#define MASTER_RIGHT
// #pragma message XXSTR(MASTER_RIGHT)
// #pragma message XXSTR(SPLIT_USB_DETECT)
#define SPLIT_USB_DETECT
#endif
