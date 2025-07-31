#include QMK_KEYBOARD_H
#include <print.h>

#define TIME_OUT 1000
#define TH_ON 10
#define TH_OFF 8

// Touch keys
static struct touch_key_strc {
    uint8_t key_stts;
    uint32_t f_time;
    uint8_t f_stts;
} touch_keys[4];

uint16_t touch_key_pins [][2] = {{GP11, GP27}, {GP11, GP28}, {GP11, GP29}, {GP11, GP26}};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┐
     * │ 7 │ 8 │ 9 │ / │
     * ├───┼───┼───┼───┤
     * │ 4 │ 5 │ 6 │ * │
     * ├───┼───┼───┼───┤
     * │ 1 │ 2 │ 3 │ - │
     * ├───┼───┼───┼───┤
     * │ 0 │ . │Ent│ + │
     * └───┴───┴───┴───┘
     */
    [0] = LAYOUT_ortho_4x4(
        KC_P7,   KC_P8,   KC_P9,   KC_PSLS,
        KC_P4,   KC_P5,   KC_P6,   KC_PAST,
        KC_P1,   KC_P2,   KC_P3,   KC_PMNS,
        KC_P0,   KC_PDOT, KC_PENT, KC_PPLS
    )
};


bool process_touch_key(int16_t* on_delay, int16_t* off_delay, uint16_t output_pin, uint16_t input_pin) {
    int16_t j;
    int16_t on_delay_c, off_delay_c;
    static uint16_t cnt = 0;
    // char str[256];

    writePinLow(output_pin);
    wait_us(1);
    writePinHigh(output_pin);
    on_delay_c = TIME_OUT;
    for (j = 0; j < TIME_OUT; j++) {
        if (readPin(input_pin) == 1) {
            on_delay_c = j;
            break;
        }
        wait_us(1);
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

    if (cnt == 0 || cnt >= 100) {
        // print("\n>\n");
        // sprintf(str, "%d,%d,%d,%d:", *on_delay, *off_delay, output_pin, input_pin);
        //        uprintf("(%d,%d),%d,%d:", output_pin, input_pin, on_delay_c, off_delay_c);
        //SEND_STRING(str);
        cnt = 1;
    }
    cnt++;
    
    // uprintf("%d,%d,%d,%d:", *on_delay, *off_delay, output_pin, input_pin);
    // uprintf("(%d,%d)", on_delay_c, off_delay_c);

    //*on_delay = 0;
    //*off_delay = 0;
    //return(true);

    if ((on_delay_c >= TIME_OUT) || off_delay_c >= TIME_OUT) {
        //
        return(false);
    }

    if ((on_delay_c >= 0) || off_delay_c >= 0) {
    }
    return(true);
}

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
    for (k = 0; k < 2; k++) {
        writePinLow(touch_key_pins[k][0]);
    }
        
    for (k = 0; k < 2; k++) {
        delay[k][0] = delay[k][1] = 0;
        if (process_touch_key(&delay[k][0], &delay[k][1], touch_key_pins[k][0], touch_key_pins[k][1]) == false) {
            // uprintf("f%d:", k);
        }
    }
    for (k = 0; k < 3; k++) {
        //        if (((delay[k][0] - delay[0][0]) > 0) && ((delay[k][1] - delay[0][1]) > 0)) {
        //        if (((delay[k][0] - delay[0][0]) > 0)&&(delay[k][0] != 512)) {
        if ((delay[k][0] > 5)&&(delay[k][0] != TIME_OUT)) {
            touch_keys[k].key_stts = 1;
            // uprintf("(%d(%d,%d))", k, delay[k][0] - delay[0][0], delay[k][1] - delay[1][0]);
        } else {
            touch_keys[k].key_stts  = 0;
            // print(".");
        }

        uprintf("%d,%d:", delay[k][0] - delay[0][0], delay[k][1] - delay[k][1]);
    }
    if (touch_keys[1].key_stts == 1) {
        writePinLow(GP1);
    } else {
        writePinHigh(GP1);
    }
        // setPinOutput_writeHigh(GP16); // LED1
        // setPinOutput_writeLow(GP17); // LED2
        // setPinOutput_writeLow(GP18); // LED3
    return(true);
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=false;
  debug_matrix=false;
  debug_keyboard=false;
  //  debug_mouse=false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
    #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
  return true;
}


void           pointing_device_driver_init(void) {
    // for touch keys
    /*
    setPinOutput(GP11); // For charging touch keys.
    writePinLow(GP11); // For charging touch keys.
    setPinInput(GP12); // Touch Key 0
    setPinInput(GP13); // Touch Key 1
    setPinInput(GP14); // Touch Key 2
    setPinInput(GP15); // Touch Key 3
    */
}
report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) { return mouse_report; }
uint16_t       pointing_device_driver_get_cpi(void) { return 0; }
void           pointing_device_driver_set_cpi(uint16_t cpi) {}


report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    //

    // uint32_t c_time;

    // c_time = timer_read();

    // uprintf("t%ld:", c_time);
        
    //process_touch_keys();
    
    return(mouse_report);
}
