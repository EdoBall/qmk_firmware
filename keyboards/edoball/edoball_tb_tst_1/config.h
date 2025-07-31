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

// for PMW3389

#define SPI_DRIVER SPID1
#define SPI_SCK_PIN GP26
#define SPI_MOSI_PIN GP27
#define SPI_MISO_PIN GP28
#define PMW33XX_CS_PIN GP29
// #define POINTING_DEVICE_CS_PIN GP9

/*
#    define SPI_DRIVER SPID1
#    define SPI_SCK_PIN GP26
#    define SPI_MISO_PIN GP8
#    define SPI_MOSI_PIN GP27
#    define POINTING_DEVICE_CS_PIN GP9
*/
#    define PMW33XX_CPI 500
#    define PMW33XX_CS_DIVISOR 64

// (Required) Sets the Chip Select pin connected to the sensor.	POINTING_DEVICE_CS_PIN

// PMW33XX_CS_PINS (Alternative) Sets the Chip Select pins connected to multiple sensors.	{PMW33XX_CS_PIN}

// PMW33XX_CS_PIN_RIGHT (Optional) Sets the Chip Select pin connected to the sensor on the right half.	PMW33XX_CS_PIN
// PMW33XX_CS_PINS_RIGHT	(Optional) Sets the Chip Select pins connected to multiple sensors on the right half.	{PMW33XX_CS_PIN_RIGHT}

// #define PMW33XX_CPI 1500
// (Optional) Sets counts per inch sensitivity of the sensor.	varies
// PMW33XX_CLOCK_SPEED	(Optional) Sets the clock speed that the sensor runs at.	2000000
// PMW33XX_SPI_DIVISOR	(Optional) Sets the SPI Divisor used for SPI communication.	varies
// PMW33XX_LIFTOFF_DISTANCE	(Optional) Sets the lift off distance at run time	0x02
// ROTATIONAL_TRANSFORM_ANGLE	(Optional) Allows for the sensor data to be rotated +/- 127 degrees directly in the sensor.	0


// MOUSE_EXTENDED_REPORT	(Optional) Enables support for extended mouse reports. (-32767 to 32767, instead of just -127 to 127).	not defined
// WHEEL_EXTENDED_REPORT	(Optional) Enables support for extended wheel reports. (-32767 to 32767, instead of just -127 to 127).	not defined
// POINTING_DEVICE_ROTATION_90	(Optional) Rotates the X and Y data by 90 degrees.	not defined
// POINTING_DEVICE_ROTATION_180	(Optional) Rotates the X and Y data by 180 degrees.	not defined
// POINTING_DEVICE_ROTATION_270	(Optional) Rotates the X and Y data by 270 degrees.	not defined
// POINTING_DEVICE_INVERT_X	(Optional) Inverts the X axis report.	not defined
// POINTING_DEVICE_INVERT_Y	(Optional) Inverts the Y axis report.	not defined
// POINTING_DEVICE_MOTION_PIN	(Optional) If supported, will only read from sensor if pin is active.	not defined
// POINTING_DEVICE_MOTION_PIN_ACTIVE_LOW	(Optional) If defined then the motion pin is active-low.	varies
// POINTING_DEVICE_TASK_THROTTLE_MS	(Optional) Limits the frequency that the sensor is polled for motion.	not defined
// POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE	(Optional) Enable inertial cursor. Cursor continues moving after a flick gesture and slows down by kinetic friction.	not defined
// POINTING_DEVICE_GESTURES_SCROLL_ENABLE	(Optional) Enable scroll gesture. The gesture that activates the scroll is device dependent.	not defined
// #define POINTING_DEVICE_CS_PIN 25
// (Optional) Provides a default CS pin, useful for supporting multiple sensor configs.	not defined
// #define POINTING_DEVICE_SDIO_PIN 
// (Optional) Provides a default SDIO pin, useful for supporting multiple sensor configs.	not defined
// #define POINTING_DEVICE_SCLK_PIN 27
// (Optional) Provides a default SCLK pin, useful for supporting multiple sensor configs.

