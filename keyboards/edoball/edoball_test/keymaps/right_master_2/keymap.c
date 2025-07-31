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
//#define KJ_UBAR S(KC_RO)
#define KJ_UBAR S(KC_INT1)
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
// #define KJ_JYEN KC_JYEN
#define KJ_JYEN KC_INT3
// |
//#define KJ_PIPE S(KC_JYEN)
#define KJ_PIPE S(KC_INT3)  
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

/*
Key	Aliases	Description
KC_INTERNATIONAL_1	KC_INT1	JIS \ and _
KC_INTERNATIONAL_2	KC_INT2	JIS Katakana/Hiragana
KC_INTERNATIONAL_3	KC_INT3	JIS ¥ and |
KC_INTERNATIONAL_4	KC_INT4	JIS Henkan
KC_INTERNATIONAL_5	KC_INT5	JIS Muhenkan
KC_INTERNATIONAL_6	KC_INT6	JIS Numpad ,
KC_INTERNATIONAL_7	KC_INT7	International 7
KC_INTERNATIONAL_8	KC_INT8	International 8
KC_INTERNATIONAL_9	KC_INT9	International 9
KC_LANGUAGE_1	KC_LNG1	Hangul/English
KC_LANGUAGE_2	KC_LNG2	Hanja
KC_LANGUAGE_3	KC_LNG3	JIS Katakana
KC_LANGUAGE_4	KC_LNG4	JIS Hiragana
KC_LANGUAGE_5	KC_LNG5	JIS Zenkaku/Hankaku
KC_LANGUAGE_6	KC_LNG6	Language 6
KC_LANGUAGE_7	KC_LNG7	Language 7
KC_LANGUAGE_8	KC_LNG8	Language 8
KC_LANGUAGE_9	KC_LNG9	Language 9
*/


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
[_EMACS] = LAYOUT( \
   QK_GESC,KJ_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_BSPC, \
   QWERTY, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, \
   KC_LCTL,MYLCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  \
   US2JP,  KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, \
   ADJUST, KC_LCTL, KC_LGUI, KC_LALT, KC_CAPS, LT_RS,   LT_LE,   LT_RE,   LT_LS,   KC_BSPC, KC_RALT, KC_DEL,  KC_MINS, KC_EQL,  \
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
),

// US keymap to JIS keymap
[_US2JP] = LAYOUT( \
   QK_GESC,KJ_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KJ_MINS, KJ_EQL,  \
   QWERTY, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KJ_LBRC, KJ_RBRC,	\
   KC_LCTL,MYLCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KJ_SCLN, KJ_QUOT, KC_ENT,  \
   KC_LSFT,US2JPS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KJ_COMM, KJ_DOT,  KJ_SLSH, US2JPS,  KJ_JYEN, \
   ADJUST, KC_LCTL, KC_LGUI, KC_LALT, KC_INT5, LT_RSJ,  LT_LEJ,  KC_INT4, LT_LSJ,  KC_BSPC, KC_ENT,  KC_CAPS, KJ_MINS, KC_BSPC,	\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
), 

// US keymap with shift key to JIS keymap
[_US2JPS] = LAYOUT( \
   QK_GESC,KJ_TILD, KJ_EXLM, KJ_AT,   KJ_HASH, KJ_DLR,  KJ_PERC, KJ_CIRC, KJ_AMPR, KJ_ASTR, KJ_LPRN, KJ_RPRN, KJ_UBAR, KJ_PLUS,	\
   QWERTY, KC_TAB,  S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T), S(KC_Y), S(KC_U), S(KC_I), S(KC_O), S(KC_P), KJ_LCBC, KJ_RCBC, \
   KC_LCTL,MYLCTL,  S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), KJ_CLN,  KJ_DQT,  KC_ENT,  \
   KC_LSFT,US2JPS,  S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), KJ_LT,   KJ_GT,   KJ_QUES, US2JPS,  KJ_PIPE, \
   ADJUST, _______, _______, _______, KC_INT4, _______, _______, KC_INT5, _______, _______, _______, _______, KJ_UBAR, KC_DEL,  \
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
),

// MYLCTL (with Emacs like cursor key support)
[_MYLCTL] = LAYOUT( \
   QK_GESC, KC_GRV, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  \
   QWERTY,  KC_TAB, C(KC_Q), C(KC_W), KC_END,  C(KC_R), C(KC_T), C(KC_Y), C(KC_U), C(KC_I), C(KC_O), KC_UP,   KC_LBRC, KC_BSPC, \
   _______, _______,KC_HOME, C(KC_S), KC_DEL,  KC_RIGHT,C(KC_G), KC_BSPC, KC_ENT,  C(KC_K), C(KC_L), KC_SCLN, KC_QUOT, KC_ENT,  \
   RAISE,  KC_LSFT, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_LEFT, KC_DOWN, C(KC_M), KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, \
   ADJUST, KC_LCTL, KC_LGUI,KC_LALT,C(KC_CAPS),C(KC_SPC),C(KC_ENT),C(KC_ENT),C(KC_SPC),C(KC_BSPC),KC_RALT,KC_DEL,KC_MINS,KC_EQL,\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
					),

[_LOWER] = LAYOUT( \
  _______, KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11 , KC_F12,  \
  EMACS, S(KC_NUHS),KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  \
  _______,S(KC_NUBS),_______,KC_LEFT, KC_UP,   KC_RGHT, _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, KC_LBRC, KC_RBRC, \
  _______, _______, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_PGUP, KC_PGDN, KC_COMM, KC_DOT,  KC_SCLN, KC_QUOT, KC_ENT,  \
  _______, _______, _______ ,_______, _______, _______, _______, _______, _______, KC_DEL,  _______, KC_SLSH, KC_RSFT, KC_BSLS, \
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
				   ),

[_LOWERJ] = LAYOUT( \
  _______, KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11 , KC_F12,  \
  EMACS,S(KC_NUHS), KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KJ_MINS, KJ_EQL,  \
  _______,S(KC_NUBS),_______,KC_LEFT, KC_UP,   KC_RGHT, _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, KJ_LBRC, KJ_RBRC, \
  _______, _______, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_PGUP, KC_PGDN, _______, _______, KJ_SCLN, KJ_QUOT, KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  _______, KC_INT5, KC_RSFT, KC_BSLS,	\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
 ),

[_RAISE] = LAYOUT( \
  _______, KC_ESC,  KC_MUTE, KC_VOLU, KC_VOLD, KC_F4,   KC_BRID, KC_BRIU, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  US2JP,   KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, \
  _______, KC_LCTL, _______, KC_LEFT, KC_UP  , KC_RIGHT,_______, KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, \
  _______, KC_LSFT, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_BTN4, KC_WH_D, KC_BTN5, _______, KC_COLN, KC_DQT,  KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TAB,  _______, KC_INT5, KC_RSFT, KC_BSLS,	\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
  ),

[_RAISEJ] = LAYOUT( \
  _______, KC_ESC,  KC_MUTE, KC_VOLU, KC_VOLD, KC_F4,   KC_BRID, KC_BRIU, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  US2JP,   KJ_TILD, KJ_EXLM, KJ_AT,   KJ_HASH, KJ_DLR,  KJ_PERC, KJ_CIRC, KJ_AMPR, KJ_ASTR, KJ_LPRN, KJ_RPRN, KJ_UBAR, KJ_PLUS,	\
  _______, KC_LCTL, _______, KC_LEFT, KC_UP  , KC_RIGHT,_______, KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KJ_LCBC, KJ_RCBC, \
  _______, KC_LSFT, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_BTN4, KC_WH_D, KC_BTN5, _______, KJ_CLN,  KJ_DQT,  KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TAB,  _______, KJ_QUES, KC_RSFT, KC_BSLS,	\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
  ),

[_MOUSE] = LAYOUT( \
   QK_GESC,KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
   QWERTY, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______, _______, _______, _______, _______, KC_DEL,  KC_BSPC, \
   MYLCTL, KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KC_BSLS, _______, \
  _______, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_BTN4, KC_WH_D, KC_BTN5, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN2, KC_MNXT, _______, _______, _______, _______, \
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
  ),

[_ADJUST] =  LAYOUT( \
  QK_RBT,  AG_NORM, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  QWERTY,  _______, QK_RBT , RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, _______, KC_PSCR, \
  EMACS,   _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  EMACS,   US2JP,   _______, _______, KC_INS,  \
  US2JP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SCRL, \
  ADJUST,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_5,    \
  KC_A,    KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
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

#ifdef POINTING_DEVICE_ENABLE
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
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // static uint16_t c = 0;
  //  static int interval = 8;
  //   uint32_t i;
  //  uint8_t j;
  // uint16_t pins[8] = {GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13};

#ifdef CONSOLE_ENABLE
#ifdef POINTING_DEVICE_ENABLE
  //  uprintf("uprint: %u\n", keycode);
  uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
  adns5050_test();
#endif
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
