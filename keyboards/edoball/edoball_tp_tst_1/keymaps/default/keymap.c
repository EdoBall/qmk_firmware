#include QMK_KEYBOARD_H
#include <print.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ A │ B │ C │
     * ├───┼───┼───┤
     * │ D │ E │ F │
     * ├───┼───┼───┤
     * │ G │ H │ I │
     * └───┴───┴───┘
     */
    [0] = LAYOUT_ortho_3x3(
        KC_A,    KC_B,    KC_C,
        KC_D,    KC_E,    KC_F,
        KC_G,    KC_H,    KC_I
    )
};


void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
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



// in keyboard.c:
#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_kb(void) {
    pmw33xx_init(1);         // index 1 is the second device.
    pmw33xx_set_cpi(0, 800); // applies to first sensor
    pmw33xx_set_cpi(1, 800); // applies to second sensor
    pointing_device_init_user();
}

// Contains report from sensor #0 already, need to merge in from sensor #1
report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    pmw33xx_report_t report = pmw33xx_read_burst(1);
    //dprintf(".");
    if (!report.motion.b.is_lifted && report.motion.b.is_motion) {
// From quantum/pointing_device_drivers.c
#define constrain_hid(amt) ((amt) < -127 ? -127 : ((amt) > 127 ? 127 : (amt)))
        mouse_report.x = constrain_hid(mouse_report.x + report.delta_x);
        mouse_report.y = constrain_hid(mouse_report.y + report.delta_y);
        dprintf("%d,%d:", mouse_report.x, mouse_report.y);
    }
    return pointing_device_task_user(mouse_report);
}
#endif

