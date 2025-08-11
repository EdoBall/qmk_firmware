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

// #define TIME_OUT 512
// #define TH_ON 10
// #define TH_OFF 8

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

// TPP 223 についてのメモ
// default では、5mmほど基板から離れても接触検知。
// 完全接触のためには、30pFのコンデンサの追加が良さそう。
// 20pFで1mmほど。

// #define TKP0 GP26
// #define TKP1 GP27
#define TKP3 GP25
#define LEDP1 GP15

static uint8_t addrPins[4] = CAPINS;

// MATRIX_*_PINS maybe come form info.json...?
pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

/*
// Touch keys
static struct touch_key_strc {
    uint8_t key_stts;
    uint32_t f_time;
    uint8_t f_stts;
} touch_keys[4];

uint16_t touch_key_pins [][2] = {{GP11, GP12}, {GP11, GP13}, {GP11, GP14}, {GP11, GP15}};
*/
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
/*
void matrix_init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}
*/

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

    // for touch keys
    //    setPinOutput_writeLow(GP11); // For charging touch keys.
    // setPinInput(TKP0); // Touch Key 0
    // setPinInput(TKP1); // Touch Key 1
    // setPinInput(TKP3); // Touch Key 2

    // for LEDs
    // LED0 -- none
    // setPinOutput_writeHigh(LEDP1); // LED1
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
    wait_us(10);
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

/*
bool process_touch_key(int16_t* on_delay, int16_t* off_delay, uint16_t output_pin, uint16_t input_pin) {
    uint16_t j;
    uint16_t on_delay_c, off_delay_c;

    writePinHigh(output_pin);
    on_delay_c = TIME_OUT;
    for (j = 0; j < TIME_OUT; j++) {
        if (readPin(input_pin) == 1) {
            on_delay_c = j;
            break;
        }
        //        wait_us(1);
    }
    wait_us(1);
	writePinLow(output_pin);
	off_delay_c = TIME_OUT;
	for (j = 0; j < TIME_OUT; j++) {
	  if (readPin(input_pin) == 0) {
		off_delay_c = j;
		break;
	  }
	  wait_us(1);
	}

    *on_delay = on_delay_c;
    *off_delay = off_delay_c;

    // *on_delay = 0;
    // *off_delay = 0;
    //return(true);

    if ((on_delay_c == TIME_OUT) || off_delay_c == TIME_OUT) {
        return(false);
    }
    return(true);
}

//
// touch_keys
bool process_touch_keys(void) {
    //    uint16_t i, k;
    uint16_t k;
    //    uint16_t on_delay_c;
    int16_t delay[4][2];
    // static uint32_t st_time = 0;
    // char str[1000];
    // float count;

    // initialize
    for (k = 0; k < 4; k++) {
        writePinLow(touch_key_pins[k][0]);
    }
        
    for (k = 0; k < 4; k++) {
        process_touch_key(&delay[k][0], &delay[k][1], touch_key_pins[k][0], touch_key_pins[k][1]);
    }
    for (k = 1; k < 4; k++) {
        if (((delay[k][0] - delay[0][0]) > 0) && ((delay[k][1] - delay[0][1]) > 0)) {
            touch_keys[k].key_stts = 1;
            //            uprintf("%d,%d:", delay[k][0] - delay[0][0], delay[k][1] - delay[1][0]);
        } else {
            touch_keys[k].key_stts  = 0;
        }
        //        uprintf("%d,%d:", delay[k][0] - delay[0][0], delay[k][1] - delay[k][1]);        
    }
    if (touch_keys[1].key_stts == 1) {
        writePinLow(GP16);
    } else {
        writePinHigh(GP16);
    }
        // setPinOutput_writeHigh(GP16); // LED1
        // setPinOutput_writeLow(GP17); // LED2
        // setPinOutput_writeLow(GP18); // LED3
    return(true);
}
*/


void matrix_init_user(void) {
    //    ws2812_init();  // WS2812ドライバーの初期化
    //    ws2812_setrgb_at(255, 0, 0, 0);  // 最初のLEDを赤色に設定
    //    ws2812_setrgb_at(0, 255, 0, 1);  // 2番目のLEDを緑色に設定
    //    ws2812_setrgb_at(0, 0, 255, 2);  // 3番目のLEDを青色に設定
    //    ws2812_flush();  // 変更を反映
    rgblight_sethsv_at(127, 255, 100, 0);
    rgblight_sethsv_at(0, 255, 100, 1);
    rgblight_sethsv_at(255, 127, 0, 2);
    rgblight_sethsv_at(0, 0, 255, 3);    
    
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
