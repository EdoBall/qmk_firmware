// Copyright 2025 Edo Ball (@Edo Ball)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

// LEDシリアル信号を送るピンを指定
#define RGB_DI_PIN GP15
// LEDの数を指定
#define RGBLED_NUM 4
// #define WS2812_RGBW
// #define WS2812_DI_PIN GP16
//#define WS2812_LED_COUNT 4

// 最大輝度（USB電流制限に注意）
#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_ANIMATIONS
// 他にもエフェクトを追加可能
