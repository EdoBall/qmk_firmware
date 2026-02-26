// Copyright 2025 Edo Ball (@Edo Ball)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

// LED pin
// Internal: GP16, PCB: GP15
// #define WS2812_DI_PIN GP16

// Numbers of LED
// Internal: 1
// #define RGBLED_NUM 1

#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD

// for AZOTEQ IQS5xx
// originaly assigned for MISO
#define I2C1_SCL_PIN GP7
// originaly assigned for for MOSI
#define I2C1_SDA_PIN GP6
#define I2C_DRIVER I2CD1
#define F_SCL 100000

// for SSCI-080798(IQS525)
// #define AZOTEQ_IQS5XX_WIDTH_MM 30
// #define AZOTEQ_IQS5XX_HEIGHT_MM 30
// #define AZOTEQ_IQS5XX_RESOLUTION_X 1024
// #define AZOTEQ_IQS5XX_RESOLUTION_Y 1024

// SSCI-080798 is located upside down.
// #define AZOTEQ_IQS5XX_ROTATION_180 TRUE

// TPS443 is located normaly.
#define AZOTEQ_IQS5XX_TPS43 1
#undef AZOTEQ_IQS5XX_ROTATION_180
