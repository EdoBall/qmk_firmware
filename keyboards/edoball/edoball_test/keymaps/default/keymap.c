// Copyright 2023 Edo Ball (@Edo Ball)
// SPDX-License-Identifier: GPL-2.0-or-later


#include QMK_KEYBOARD_H

// #include "report.h"
#include "print.h"
// #include "i2c_master.h"
// #include "i2c_slave.h"
#include "debug.h"
#include "sensors/adns5050.h"
#include "keymap_japanese.h"

// I2C_SALVE_2(mouse controler, 8bit address)
#define I2C_SLAVE_2_ADDR 0x20
#define TIMEOUT 100

// for mouse report
// static report_mouse_t mouseReport = {};
// report_mouse_t mouseReport = {};

#define _QWERTY 0
#define _EMACS  1
#define _US2JP  2
#define _LOWER  3
#define _RAISE  4
#define _US2JPS 5
#define _MOUSE  6
#define _MYLCTL 7
#define _LOWERJ 13
#define _RAISEJ 14
#define _ADJUST 15

#define LT_LS LT(_LOWER, KC_SPC)
#define LT_LE LT(_LOWER, KC_ENT)
#define LT_RS LT(_RAISE, KC_SPC)
#define LT_RE LT(_RAISE, KC_ENT)
#define LT_LSJ LT(_LOWERJ, KC_SPC)
#define LT_LEJ LT(_LOWERJ, KC_ENT)
#define LT_RSJ LT(_RAISEJ, KC_SPC)
#define LT_REJ LT(_RAISEJ, KC_ENT)

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  EMACS,
  US2JP,
  LOWER,
  RAISE,
  US2JPS,
  MOUSE,
  LOWERJ,
  RAISEJ,
  ADJUST,
  MYLCTL
};

// Keycodes for Kigou on US keyborad to JIS keyboard.
// `
#define KJ_GRV KC_GRV
// ~
#define KJ_TILD S(KC_GRV)
// !
#define KJ_EXLM S(KC_1)
// @
#define KJ_AT KC_LBRC
// #
#define KJ_HASH S(KC_3)
// $
#define KJ_DLR S(KC_4)
// %
#define KJ_PERC S(KC_5)
// ^
#define KJ_CIRC KC_EQL
// &
#define KJ_AMPR S(KC_6)
// *
#define KJ_ASTR S(KC_QUOT)
// (
#define KJ_LPRN S(KC_8)
// )
#define KJ_RPRN S(KC_9)
// -
#define KJ_MINS KC_MINS
// _
#define KJ_UBAR S(KC_RO)
// =
#define KJ_EQL S(KC_MINS)
// +
#define KJ_PLUS S(KC_SCLN)
// [
#define KJ_LBRC KC_RBRC
// {
#define KJ_LCBC S(KC_RBRC)
// ]
#define KJ_RBRC KC_BSLS
// }
#define KJ_RCBC S(KC_BSLS) 
// ;
#define KJ_SCLN KC_SCLN
// :
#define KJ_CLN KC_QUOT
// '
#define KJ_QUOT S(KC_7)
// "
#define KJ_DQT S(KC_2)
// Yen
#define KJ_JYEN KC_JYEN
// |
#define KJ_PIPE S(KC_JYEN)
// ,
#define KJ_COMM KC_COMM
// <
#define KJ_LT S(KC_COMM)
// .
#define KJ_DOT KC_DOT
// >
#define KJ_GT S(KC_DOT)
// /
#define KJ_SLSH KC_SLSH
// ?
#define KJ_QUES S(KC_SLSH)


void write_pin_test(void);
void adns5050_test(void);


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
  {
   [_QWERTY] = LAYOUT( \
   QK_GESC,KJ_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_BSPC, \
   QWERTY, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, \
   MYLCTL, KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  \
   US2JP,  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, \
   ADJUST, KC_LCTL, KC_LGUI, KC_LALT, KC_CAPS, LT_RS,   LT_LE,   LT_RE,   LT_LS,   KC_BSPC, KC_RALT, KC_DEL,  KC_MINS, KC_EQL,  \
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
					  ),
   };


#ifdef AUDIO_ENABLE
float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
#endif

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=false;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
  if (!is_keyboard_master()) {
	setPinOutput(GP25);
	writePinHigh(GP25);
  }
}


void write_pin_test() {
  uint32_t i;
  uint8_t j;
  uint16_t pins[8] = {GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13};

#ifdef CONSOLE_ENABLE
 
  for (i = 0; i < 8; i++) {
	setPinOutput(pins[i]);
  }
  uprintf("\ninit.\n");

  
  for (i = 0; i <= 0xffff; i++) {
  // for (i = 0; i < 0xff; i++) {
	for (j = 0; j < 8; j++) {
	  if ((i & (1 << ((j + (i / 256)) % 8))) >= 1) {
		  writePinHigh(pins[j]);
		  // uprintf("1");
		} else {
		  writePinLow(pins[j]);
		  // uprintf("0");3
		}
	}
	wait_us(1);
	// uprintf("\n");
  }
#endif
  
  return;
}


void adns5050_test() {
  int i;
  uint8_t ui, cpival;
  uint8_t reg_addr[18] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0d, 0x19, 0x22, 0x3a, 0x3e, 0x3f};

  adns5050_init();
  uprintf("read_reg test: ");
  for (i = 0; i < 18; i++) {
	ui = adns5050_read_reg(reg_addr[i]);
	uprintf("%x:%x; ", reg_addr[i], ui);
  }
  uprintf("\n");
  cpival = adns5050_get_cpi();
  uprintf("r: %d", cpival);
  adns5050_set_cpi(750);
  cpival = adns5050_get_cpi();
  uprintf("; 750?: %d", cpival);
  adns5050_set_cpi(cpival);
  cpival = adns5050_get_cpi();
  uprintf("; r: %d\n", cpival);
  return;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // static uint16_t c = 0;
  //  static int interval = 8;
  //   uint32_t i;
  //  uint8_t j;
  // uint16_t pins[8] = {GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13};

#ifdef CONSOLE_ENABLE
  //  uprintf("uprint: %u\n", keycode);
  uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
  adns5050_test();
#endif

  /*
  for (i = 0; i < 8; i++) {
	setPinOutput(pins[i]);
  }
  uprintf("\ninit.\n");

  
  for (i = 0; i <= 0xffff; i++) {
  // for (i = 0; i < 0xff; i++) {
	for (j = 0; j < 8; j++) {
	  if ((i & (1 << ((j + (i / 256)) % 8))) >= 1) {
		  writePinHigh(pins[j]);
		  // uprintf("1");
		} else {
		  writePinLow(pins[j]);
		  // uprintf("0");3
		}
	}
	wait_us(1);
	// uprintf("\n");
  }
  c++;
  uprintf("c: %d\n", c);
  */
  return true;
}

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init(void){
  /*  mouseReport.x = 0;
  mouseReport.y = 0;
  mouseReport.v = 0;
  mouseReport.h = 0;
  mouseReport.buttons = 0;
  */
  // reset adns5050
  adns5050_write_reg(0x3a, 0x5a);
  wait_ms(1);
  adns5050_init();
  // adns5050_set_cpi(500);
#ifdef CONSOLE_ENABLE
  //  uprintf("sig:%u, cpi:%u\n", adns5050_check_signature(),adns5050_get_cpi());
  //  uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif 
  
}

/*
// 
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  static int c = 0;
  int interval;
  uint8_t ui;

  interval = 1;
  uint8_t reg_addr[18] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0d, 0x19, 0x22, 0x3a, 0x3e, 0x3f};
  if ((c % interval) == 0) {
	if ((c / interval) == 18) { 
	  uprintf("\n");
	  c = -1;
	} else {
	  ui = adns5050_read_reg(reg_addr[c / interval]);
	  uprintf("%x:%x; ", reg_addr[c / interval], ui);
	}
  }
  c++;
	
  if ((mouse_report.y == -1) && (mouse_report.x == -1)) {
	mouse_report.x = 0;
	mouse_report.y = 0;
	// uprintf(".");
	//	adns5050_set_cpi(625);
	//	uprintf("sig:%u, cpi:%u\n", adns5050_check_signature(),adns5050_get_cpi());
  } else {
	// uprintf("%d,%d;", mouse_report.x, mouse_report.y);
  }
  return mouse_report;
}

*/

// 
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  // static uint16_t c = 0;
  // static int interval = 8;
  // uint16_t i, j;
  /*
  if ((mouse_report.y == -1) && (mouse_report.x == -1)) {
	mouse_report.x = 0;
	mouse_report.y = 0;
  } else {
  }
  */
  mouse_report.x = -mouse_report.x;
  mouse_report.y = -mouse_report.y;
  return mouse_report;
}



/*
void pointing_device_send(void){
  host_mouse_send(&mouseReport);
  mouseReport.x = 0;
  mouseReport.y = 0;
  mouseReport.v = 0;
  mouseReport.h = 0;
}
*/
/*
void pointing_device_task(void){
  uint8_t buff[4];
  i2c_status_t status;
  int8_t dX, dY;
  static uint8_t prev_buff_state = 0;
  static uint8_t prev_buff_state_touch = 0;  
  static int8_t v_movement = 0;
  static float v_movement_mag = 0.05;
  
  mouseReport.x = 0;
  mouseReport.y = 0;
  mouseReport.v = 0;
  mouseReport.h = 0;
  
  status = i2c_readReg(I2C_SLAVE_2_ADDR, 0x1, buff, sizeof(buff), TIMEOUT);
  
  if (status == I2C_STATUS_SUCCESS) {
    dX = (int8_t) buff[0];
    dY = (int8_t) buff[1];

    // for changing layer of mouse buttons
    // Switches
    if (prev_buff_state != buff[2]) {
      if (buff[2] & 1) {
	layer_on(_RAISE);
	update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
	layer_off(_RAISE);
	update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
    }
    prev_buff_state = buff[2];
    // Touch buttons
    if (prev_buff_state_touch != buff[3]) {
      if (buff[3] & 1) {
	layer_on(_MOUSE);
      } else {
	layer_off(_MOUSE);
      }
    }
    prev_buff_state_touch = buff[3];
    
    //    if ((buff[3] & 1) && (((buff[3] & 1<<1) == 0) || (mouseReport.buttons != 0))) { // touch button is on.
    if ((buff[3] & 1) && ((buff[3] & 1<<1) == 0))  { // touch button is on.      
      // mouse x y movement
      mouseReport.x = dX;
      mouseReport.y = dY;
    } else {
      // vertical scroll
      v_movement += dY;
      if (abs(v_movement * v_movement_mag) >= 1) {
	mouseReport.v = -(v_movement * v_movement_mag);
	v_movement = v_movement + mouseReport.v / v_movement_mag;
      }
    }
    pointing_device_send();
  } else {
    // I2C is failed.
    // do nothing.
  }
  // do nothing.
}
*/
#endif

#ifdef OLED_ENABLE
//#ifndef OLED_DRIVER_ENABLE // OLD QMK version.
/*
bool oled_task_user(void) {

  // Host Keyboard Layer Status
  // The highest layer:
  oled_write_P(PSTR("Layer: "), false);

  switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _EMACS:
            oled_write_P(PSTR("EMACS\n"), false);
            break;
        case _US2JP:
            oled_write_P(PSTR("US2JP\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("LOWER\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("RAISE\n"), false);
            break;
        case _US2JPS:
            oled_write_P(PSTR("US2JPS\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("MOUSEP\n"), false);
            break;
        case _MYLCTL:
            oled_write_P(PSTR("MYCTL\n"), false);
            break;
        case _LOWERJ:
            oled_write_P(PSTR("LOWERJ\n"), false);
            break;
        case _RAISEJ:
            oled_write_P(PSTR("RAISEJ\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("ADJUST\n"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // The default layer:
  oled_write_P(PSTR("Default: "), false);
  switch (default_layer_state) {
  case 1 << _QWERTY:
    oled_write_P(PSTR("QWERTY\n"), false);
    break;
  case 1 << _EMACS:
    oled_write_P(PSTR("EMACS\n"), false);
    break;
  case 1 << _US2JP:
    oled_write_P(PSTR("US2JP\n"), false);
    break;
  case 1 << _LOWER:
    oled_write_P(PSTR("LOWER\n"), false);
    break;
  case 1 << _RAISE:
    oled_write_P(PSTR("RAISE\n"), false);
    break;
  case 1 << _US2JPS:
    oled_write_P(PSTR("US2JPS\n"), false);
    break;
  case 1 << _MOUSE:
    oled_write_P(PSTR("MOUSEP\n"), false);
    break;
  case 1 << _MYLCTL:
    oled_write_P(PSTR("MYCTL\n"), false);
    break;
  case 1 << _LOWERJ:
    oled_write_P(PSTR("LOWERJ\n"), false);
    break;
  case 1 << _RAISEJ:
    oled_write_P(PSTR("RAISEJ\n"), false);
    break;
  case 1 << _ADJUST:
    oled_write_P(PSTR("ADJUST\n"), false);
    break;
  default:
    // Or use the write_ln shortcut over adding '\n' to the end of your string
    oled_write_ln_P(PSTR("Undefined"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
  return false;
}

*/
#endif
