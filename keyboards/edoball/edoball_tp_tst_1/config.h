// Copyright 2025 Edo Ball (@Edo Ball)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* memo

keyboard:
    "matrix_pins": {
        "cols": ["GP8", "GP9", "GP10"],
        "rows": ["GP11", "GP12", "GP13"]
    },

NCS 20
MISO 21
MOSI 26
SCLK 27
MTN 28
N/C 29
VCC
GND

*/

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

#define DEBUG_ENABLE

// for AZOTEQ IQS5xx
#define I2C1_SCL_PIN GP17
#define I2C1_SDA_PIN GP16
#define I2C_DRIVER I2CD0
#define F_SCL 100000
#define AZOTEQ_IQS5XX_TPS43 1
