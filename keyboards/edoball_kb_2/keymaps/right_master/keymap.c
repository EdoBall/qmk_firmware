// Copyright 2022 Edo Ball (@Edo Ball)
// SPDX-License-Identifier: GPL-2.0-or-later

// Edoball KB-II default keymap
//  for right hand (master) and left hand (slave).

#include QMK_KEYBOARD_H

#include "report.h"
#include "print.h"
// #include "pointing_device.h"
#include "i2c_master.h"
#include "i2c_slave.h"
//#include "debug.h"
#include "keymap_japanese.h"

// I2C_SALVE_2(mouse controler, 8bit address)
#define I2C_SLAVE_2_ADDR 0x38
//#define TIMEOUT 100
#define TIMEOUT 50

// for mouse report
static report_mouse_t mouseReport = {};
// report_mouse_t mouseReport = {};

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
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
// #define KJ_UBAR S(KC_RO)
#define KJ_UBAR KC_UNDS
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
#define KC_JYEN KC_NUBS
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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

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
   ADJUST, KC_LCTL, KC_LGUI, KC_LALT, JP_MHEN, LT_RSJ,  LT_LEJ,  JP_HENK, LT_LSJ,  KC_BSPC, KC_ENT,  KC_CAPS, KJ_MINS, KC_BSPC,	\
   KC_A,   KC_B,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_A,    KC_B     \
), 

// US keymap with shift key to JIS keymap
[_US2JPS] = LAYOUT( \
   QK_GESC,KJ_TILD, KJ_EXLM, KJ_AT,   KJ_HASH, KJ_DLR,  KJ_PERC, KJ_CIRC, KJ_AMPR, KJ_ASTR, KJ_LPRN, KJ_RPRN, KJ_UBAR, KJ_PLUS,	\
   QWERTY, KC_TAB,  S(KC_Q), S(KC_W), S(KC_E), S(KC_R), S(KC_T), S(KC_Y), S(KC_U), S(KC_I), S(KC_O), S(KC_P), KJ_LCBC, KJ_RCBC, \
   KC_LCTL,MYLCTL,  S(KC_A), S(KC_S), S(KC_D), S(KC_F), S(KC_G), S(KC_H), S(KC_J), S(KC_K), S(KC_L), KJ_CLN,  KJ_DQT,  KC_ENT,  \
   KC_LSFT,US2JPS,  S(KC_Z), S(KC_X), S(KC_C), S(KC_V), S(KC_B), S(KC_N), S(KC_M), KJ_LT,   KJ_GT,   KJ_QUES, US2JPS,  KJ_PIPE, \
   ADJUST, _______, _______, _______, JP_MHEN, _______, _______, JP_HENK, _______, _______, _______, _______, KJ_UBAR, KC_DEL,  \
   KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
),

// MYLCTL (with Emacs like cursor key support)
[_MYLCTL] = LAYOUT( \
   QK_GESC, KC_GRV, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  \
   QWERTY,  KC_TAB, C(KC_Q), C(KC_W), KC_END,  C(KC_R), C(KC_T), C(KC_Y), C(KC_U), C(KC_I), C(KC_O), KC_UP,   KC_LBRC, KC_BSPC, \
   _______, _______,KC_HOME, C(KC_S), KC_DEL,  KC_RIGHT,C(KC_G), KC_BSPC, KC_ENT,  C(KC_K), C(KC_L), KC_SCLN, KC_QUOT, KC_ENT,  \
   RAISE,  KC_LSFT, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), KC_LEFT, KC_DOWN, C(KC_M), KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, \
   ADJUST, KC_LCTL, KC_LGUI,KC_LALT,C(KC_CAPS),C(KC_SPC),C(KC_ENT),C(KC_ENT),C(KC_SPC),C(KC_BSPC),KC_RALT,KC_DEL,KC_MINS,KC_EQL,\
   KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
),

[_LOWER] = LAYOUT( \
  _______, KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11 , KC_F12,  \
  EMACS, S(KC_NUHS),KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  \
  _______,S(KC_NUBS),_______,KC_LEFT, KC_UP,   KC_RGHT, _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, KC_LBRC, KC_RBRC, \
  _______, _______, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_PGUP, KC_PGDN, KC_COMM, KC_DOT,  KC_SCLN, KC_QUOT, KC_ENT,  \
  _______, _______, _______ ,_______, _______, _______, _______, _______, _______, KC_DEL,  _______, KC_SLSH, KC_RSFT, KC_BSLS, \
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
 ),

[_LOWERJ] = LAYOUT( \
  _______, KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11 , KC_F12,  \
  EMACS,S(KC_NUHS), KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KJ_MINS, KJ_EQL,  \
  _______,S(KC_NUBS),_______,KC_LEFT, KC_UP,   KC_RGHT, _______, KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, KJ_LBRC, KJ_RBRC, \
  _______, _______, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_PGUP, KC_PGDN, _______, _______, KJ_SCLN, KJ_QUOT, KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_DEL,  _______, JP_HENK, KC_RSFT, KC_BSLS,	\
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
 ),

[_RAISE] = LAYOUT( \
  _______, KC_ESC,  KC_MUTE, KC_VOLU, KC_VOLD, KC_F4,   KC_BRID, KC_BRIU, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  US2JP,   KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, \
  _______, KC_LCTL, _______, KC_LEFT, KC_UP  , KC_RIGHT,_______, KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, \
  _______, KC_LSFT, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_BTN4, KC_WH_D, KC_BTN5, _______, KC_COLN, KC_DQT,  KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TAB,  _______, JP_HENK, KC_RSFT, KC_BSLS,	\
   KC_NO,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
  ),

[_RAISEJ] = LAYOUT( \
  _______, KC_ESC,  KC_MUTE, KC_VOLU, KC_VOLD, KC_F4,   KC_BRID, KC_BRIU, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  US2JP,   KJ_TILD, KJ_EXLM, KJ_AT,   KJ_HASH, KJ_DLR,  KJ_PERC, KJ_CIRC, KJ_AMPR, KJ_ASTR, KJ_LPRN, KJ_RPRN, KJ_UBAR, KJ_PLUS,	\
  _______, KC_LCTL, _______, KC_LEFT, KC_UP  , KC_RIGHT,_______, KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KJ_LCBC, KJ_RCBC, \
  _______, KC_LSFT, _______, KC_PGUP, KC_DOWN, KC_PGDN, _______, KC_BTN4, KC_WH_D, KC_BTN5, _______, KJ_CLN,  KJ_DQT,  KC_ENT,  \
  _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TAB,  _______, KJ_QUES, KC_RSFT, KC_BSLS,	\
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
  ),

[_MOUSE] = LAYOUT( \
   QK_GESC,KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
   QWERTY, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    _______, _______, _______, _______, _______, KC_DEL,  KC_BSPC, \
   MYLCTL, KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_BTN1, KC_BTN3, KC_BTN2, KC_LBRC, KC_RBRC, KC_BSLS, _______, \
  _______, KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_BTN4, KC_WH_D, KC_BTN5, _______, _______, _______, _______, \
  _______, _______, _______, _______, _______, _______, _______, _______, KC_BTN2, KC_MNXT, _______, _______, _______, _______, \
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
  ),

[_ADJUST] =  LAYOUT( \
  QK_RBT,   AG_NORM, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
  QWERTY,  _______, QK_RBT , RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, _______, KC_PSCR, \
  EMACS,   _______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  EMACS,   US2JP,   _______, _______, KC_INS,  \
  US2JP,   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_SCRL, \
  ADJUST,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LCAP,    \
  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    \
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
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  debug_mouse=true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
  uprintf("uprint: %u\n", keycode);
#endif
  switch (keycode) {
      // for mouse control
      // avoid conflicting mouse reports and mouse keys, hook button keycodes.
      // memo: mouseReport.buttons = 0x1F (decimal 31, binary 00011111) max (bitmask for mouse buttons 1-5, 1 is rightmost, 5 is leftmost) 0x00 min
    case KC_BTN1:
      if (record->event.pressed) {
	mouseReport.buttons |= 1;
      } else {
	mouseReport.buttons &= 0x1E; // 0001 1110
      }
      return false;
      break;
    case KC_BTN2:
      if (record->event.pressed) {
	mouseReport.buttons |= 2;
      } else {
	mouseReport.buttons &= 0x1D; // 0001 1101
      }
      return false;
      break;
    case KC_BTN3:
      if (record->event.pressed) {
	mouseReport.buttons |= 4;
      } else {
	mouseReport.buttons &= 0x1B; // 0001 1011
      }
      return false;
      break;
    case KC_BTN4:
      if (record->event.pressed) {
	mouseReport.buttons |= 8;
      } else {
	mouseReport.buttons &= 0x17; // 0001 0111
      }
      return false;
      break;
    case KC_BTN5:
      if (record->event.pressed) {
	mouseReport.buttons |= 16;
      } else {
	mouseReport.buttons &= 0x0F; // 0000 1111
      }
      return false;
      break;
      
      // change the default layer.
  case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        persistent_default_layer_set(1UL<<_QWERTY);
      }
      return false;
      break;
    case EMACS:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_EMACS);
      }
      return false;
      break;
    case US2JP:
      if (record->event.pressed) {
        persistent_default_layer_set(1UL<<_US2JP);
      }
      return false;
      break;
    case US2JPS:
      if (record->event.pressed) {
	layer_on(_US2JPS);
      } else {
	layer_off(_US2JPS);
      }
      return false;
      break;
    case MYLCTL:
      if (record->event.pressed) {
	layer_on(_MYLCTL);
      } else {
	layer_off(_MYLCTL);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
      // for SJIS
    case LOWERJ:
      if (record->event.pressed) {
        layer_on(_LOWERJ);
        update_tri_layer(_LOWERJ, _RAISEJ, _ADJUST);
      } else {
        layer_off(_LOWERJ);
        update_tri_layer(_LOWERJ, _RAISEJ, _ADJUST);
      }
      return false;
      break;
    case RAISEJ:
      if (record->event.pressed) {
        layer_on(_RAISEJ);
        update_tri_layer(_LOWERJ, _RAISEJ, _ADJUST);
      } else {
        layer_off(_RAISEJ);
        update_tri_layer(_LOWERJ, _RAISEJ, _ADJUST);
      }
      return false;
      break;
  }
  return true;
}

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init(void){
  mouseReport.x = 0;
  mouseReport.y = 0;
  mouseReport.v = 0;
  mouseReport.h = 0;
  mouseReport.buttons = 0;
}

void pointing_device_send(void){
  host_mouse_send(&mouseReport);
  mouseReport.x = 0;
  mouseReport.y = 0;
  mouseReport.v = 0;
  mouseReport.h = 0;
}

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
#endif

#ifdef OLED_ENABLE
bool oled_task_user(void) {
  // Host Keyboard Layer Status
  oled_write_P(PSTR("Layer: "), false);
  // The highest layer:
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
    oled_write_ln_P(PSTR("Undefined"), false);
  }

  // Host Keyboard LED Status
  led_t led_state = host_keyboard_led_state();
  oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
  oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
  oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
  return false;
}
#endif
