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
#define RGB_DI_PIN GP16

// Numbers of LED
// Internal: 1
#define RGBLED_NUM 1

#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD

