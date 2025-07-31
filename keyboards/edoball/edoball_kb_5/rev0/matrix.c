/*
Copyright 2012-2018 Jun Wako, Jack Humbert, Yiancar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include QMK_KEYBOARD_H
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "atomic_util.h"

#include "print.h"
#include "debug.h"

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return readPin(pin);
    } else {
        return 1;
    }
}

// user-defined overridable functions
/*
__attribute__((weak)) void matrix_init_pins(void);
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter);
*/


// matrix code
/*
// override
void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
	  pin_t pin = read_col_pins(col_index);
	  current_row_value |= readMatrixPin(pin) ? 0 : row_shifter;
    }

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}
*/

/*
Column pin: GP21
Row pins: GP8: (row of '1') - GP12: (row of 'Space')
Column Address Pins Enable: GP20
Column Address Pins for 74HC4067: {GP26, GP27, GP28, GP29};
Row pins:  {GP8, GP9, GP10, GP11, GP12};
*/

static bool select_row(uint8_t row) {
  pin_t row_pins[] = {GP8, GP9, GP10, GP11, GP12};
  pin_t pin = row_pins[row];
  if (pin != NO_PIN) {
	// unselect: High
	setPinOutput_writeLow(pin);
  } else {
	return(false);
  }
  return(true);
}

static void unselect_row(uint8_t row) {
  pin_t row_pins[] = {GP8, GP9, GP10, GP11, GP12};
  pin_t pin = row_pins[row];
  if (pin != NO_PIN) {
	// unselect: High
	setPinOutput_writeHigh(pin);
  }
}


static void unselect_rows(void) {
    for (uint8_t x = 0; x < 5; x++) {
        unselect_row(x);
    }
}

/*
__attribute__((weak)) void matrix_init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}
*/

void matrix_init_pins(void) {
  //  uint8_t addrPins[4] = {GP26, GP27, GP28, GP29};
  //  uint8_t i;

  // static inline void setPinOutput_writeHogeを使うと
  // output はinitいらない。
  // inputのみinit.
  // Key matrixのcols inputは GP14(multiplexerで切りかえ), touch keyのinputは、Col13(palm rest),
  // Col14(thumb) , Col15(touch wall), 
  // Rows は、GP0-GP4
  // とりあえず、mouse sensor のことは考えない。
  unselect_rows();  
  // gpio_atomic_set_pin_input(GP14); // matrix input (Cols)
  setPinInput(GP14);
  setPinInputHigh_atomic(GP0);
  setPinInputHigh_atomic(GP1);
  setPinInputHigh_atomic(GP2);
  setPinInputHigh_atomic(GP3);
  setPinInputHigh_atomic(GP4);
}

uint8_t readMatrixCol(uint8_t col_index) {
  // for 74HC4067...
  // Address Pins: GP12: S0 - GP9: S3
  // Input Pin: GP14
  // Address Enable Pin: GP13: NE

  
  uint8_t addrPins[4] = {GP12, GP11, GP10, GP9};
  uint8_t i, pinState;

  setPinOutput_writeLow(GP13); // unselect the 74HC4067
  
  // set an address
  for (i = 0; i < 4; i++) {
	if ((col_index & (1 << i)) >= 1) {
	  setPinOutput_writeHigh(addrPins[i]);
	} else {
	  setPinOutput_writeLow(addrPins[i]);
	}
  }
  
  setPinOutput_writeHigh(GP13); // select the 74HC4067
  wait_us(1);
  pinState = readMatrixPin(GP14);
  setPinOutput_writeLow(GP13); // unselect the 74HC4067
  
  // do not clear address pins.
  return(pinState);
}


void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
  // Col: read via a multiplexer
  // Row: set pins directly
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
	  //uint8_t pin_state = readMatrixPin(col_pins[col_index]);
	  uint8_t pin_state = readMatrixCol(col_index);

        // Populate the matrix row with the state of the col pin
	  current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
	//	current_matrix[current_row] = 9;
}


bool matrix_scan_custom(matrix_row_t* current_matrix) {
  bool         matrix_has_changed      = false;
  matrix_row_t tmp_matrix[MATRIX_ROWS] = {0};
  {
	for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
	  matrix_read_cols_on_row(tmp_matrix, current_row);
	}
	
	matrix_has_changed = memcmp(current_matrix, tmp_matrix, sizeof(tmp_matrix)) != 0;
	//	if (matrix_has_changed) memcpy(current_matrix, tmp_matrix, sizeof(tmp_matrix));
	if (matrix_has_changed) {
	  memcpy(current_matrix, tmp_matrix, sizeof(tmp_matrix));
	} else {
	  uprintf("nc");
	  // send_string("nc");
	}
  }
  // send_string("m");
  return matrix_has_changed;
}


/*
static bool select_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        gpio_atomic_set_pin_output_low(pin);
        return true;
    }
    return false;
}

static void unselect_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        gpio_atomic_set_pin_output_high(pin);
#            else
        gpio_atomic_set_pin_input_high(pin);
#            endif
    }
}

static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}

__attribute__((weak)) void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        if (row_pins[x] != NO_PIN) {
            gpio_atomic_set_pin_input_high(row_pins[x]);
        }
    }
}

__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    bool key_pressed = false;

    // Select col
    if (!select_col(current_col)) { // select col
        return;                     // skip NO_PIN col
    }
    matrix_output_select_delay();

    // For each row...
    for (uint8_t row_index = 0; row_index < ROWS_PER_HAND; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed); // wait for all Row signals to go HIGH
}


void matrix_init(void) {
    // initialize key pins
    matrix_init_pins();

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));

    debounce_init(ROWS_PER_HAND);

    matrix_init_kb();
}
*/
