#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <print.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "quantum.h"
#include "ws2812.h"

// for debuging macros.
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define PRINT_MACRO(x) #x "=" TOSTRING(x)

// #pragma message(PRINT_MACRO(MATRIX_ROW_PINS))
#pragma message(PRINT_MACRO(MATRIX_ROWS))
#pragma message(PRINT_MACRO(MATRIX_COLS))

/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values
extern matrix_row_t matrix[MATRIX_ROWS];     // debounced values

/* Hardware Settings (defined in info.json)
Column pin: GP14
Row pins: GP0: (row of '1') - GP4: (row of 'Space')
Column Address Pins Enable: GP13
Column Address Pins for 74HC4067: {GP12, GP11, G10, GP9};
*/

// Column Address Pins Enable
#define CAPINE GP13
// Column Address Pins
#define CAPINS {GP12, GP11, GP10, GP9}
// Column Signal Pin
#define CSPIN GP14
// Row pins are defined by info.json (not here)
#define RPINS {GP0, GP1, GP2, GP3, GP4};

#define TKP3 GP25
#define LEDP1 GP15

static uint8_t addrPins[4] = CAPINS;

// MATRIX_*_PINS maybe come form info.json...?
pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;


// derived from quantum/matrix.c
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

static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}

static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinInputHigh_atomic(pin);
    }
}

static void unselect_rows(void) {
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        unselect_row(x);
    }
}

void matrix_init_pins_user(void) {
    // static inline void setPinOutput_writeHogeを使うと
    // output はinitいらない。
    // inputのみinit.
    // Key matrixのcols inputは P21(multiplexerで切りかえ), touch keyのinputは、P20-P16
    // Rows は、P8-P13
    // とりあえず、mouse sensor のことは考えない。

    unselect_rows();  // initialize Row pins.

    setPinOutput_writeLow(CAPINE); //Column Address Pins Enable:
    setPinInputHigh_atomic(CSPIN); // Col Signal Input
    setPinOutput_writeLow(addrPins[0]); 
    setPinOutput_writeLow(addrPins[1]); 
    setPinOutput_writeLow(addrPins[2]); 
    setPinOutput_writeLow(addrPins[3]); 

}

uint8_t readMatrixCol(uint8_t col_index) {
    // for 74HC4067...

    uint8_t i, pinState;

    setPinOutput_writeHigh(CAPINE); // unselect the 74HC4067
  
    // set an address
    for (i = 0; i < 4; i++) {
        if ((col_index & (1 << i)) >= 1) {
            setPinOutput_writeHigh(addrPins[i]);
        } else {
            setPinOutput_writeLow(addrPins[i]);
        }
    }
    // wait_us(1);
    setPinOutput_writeLow(CAPINE); // select the 74HC4067
    wait_us(10); // wait 10 us (not optimized)
    pinState = readMatrixPin(CSPIN); // Col Signal Input
    // pinState = 0;
    setPinOutput_writeHigh(CAPINE); // unselect the 74HC4067
  
    // do not clear address pins.
    return(pinState);
}

void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        uint8_t pin_state = readMatrixCol(col_index);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}

void matrix_init_user(void) {
    //    ws2812_init();  // WS2812ドライバーの初期化
    //    ws2812_setrgb_at(255, 0, 0, 0);  // 最初のLEDを赤色に設定
    //    ws2812_setrgb_at(0, 255, 0, 1);  // 2番目のLEDを緑色に設定
    //    ws2812_setrgb_at(0, 0, 255, 2);  // 3番目のLEDを青色に設定
    //    ws2812_flush();  // 変更を反映

    rgblight_sethsv_at(127, 255, 100, 0);
    /*
    rgblight_sethsv_at(0, 255, 100, 1);
    rgblight_sethsv_at(255, 127, 0, 2);
    rgblight_sethsv_at(0, 0, 255, 3);    
    */
}

void matrix_init_custom(void) {
    // TODO: initialize hardware here
      // initialize key pins
    matrix_init_pins_user();

    // initialize matrix state: all keys off
    memset(matrix, 0, sizeof(matrix));
    memset(raw_matrix, 0, sizeof(raw_matrix));

    debounce_init(MATRIX_ROWS);

    //    matrix_init_quantum();
}

bool matrix_scan_custom(matrix_row_t* current_matrix) {
    matrix_row_t tmp_matrix[MATRIX_ROWS] = {0};

    // Set row, read cols
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        matrix_read_cols_on_row(tmp_matrix, current_row);
    }

    bool changed = memcmp(current_matrix, tmp_matrix, sizeof(tmp_matrix)) != 0;
    if (changed) memcpy(current_matrix, tmp_matrix, sizeof(tmp_matrix));
    // process_touch_keys();
    return changed;
}
