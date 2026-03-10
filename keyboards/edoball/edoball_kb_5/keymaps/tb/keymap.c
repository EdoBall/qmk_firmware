#include QMK_KEYBOARD_H
#include <print.h>
#include "keymap_japanese.h"
#include "quantum_keycodes.h"
#include "print.h"
#include "debug.h"

#include "keymap.h"
#include "iqs5xx.h"
#include "key_overrides.h"

#define DEBUG

// Todo:  hogehoge.hでなるべくextern宣言をしないですむように関数を変更。
// TPS43 が接続されていなくても動くようにする。

static uint16_t kb_mode = 1 << M_USUS;
static char debug_str[256];

uint8_t readMatrixCol(uint8_t col_index);

/*
static uint8_t cursor_kc[][2] =
    {{KC_UP, KC_P},
     {KC_DOWN, KC_N},
     {KC_LEFT, KC_B},
     {KC_RIGHT, KC_F},
     {KC_PGUP, KC_NO},
     {KC_PGDN, KC_V}};
*/

// My Key Modifiers:
// 0: Pseudo Control, 1: Pseudo Alt, 2: Pseudo GUI, 3: Palm Rest, 4: PCB1, 5: Touch wall 1,
#define MM_PC 0
#define MM_PA 1
#define MM_PG 2
#define MM_PR 3
#define MM_PCB 4
#define MM_TW 5


// scroll
#define SCRL_MAG 0.02
#define SCRL_MAG_TWO_FINGERS 0.02
static uint8_t my_modifier = 0;
#ifdef AZOTEQ_IQS5XX_TPS43
static bool scroll_flag = false;
#endif


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
     * _______
     *   KC_TILDE,  KC_EXLM, KC_AT, KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_A, KC_B, KC_C, \
     */
                                                              /*   KC_LCTL, MO(_FN), KC_LGUI, KC_LALT,   CC_SPLW,LT(_TENKEY,KC_INT4),LT(_RAISE,KC_ENT),CC_SPLW,KC_BSPC, KC_RALT,KC_LEFT,CC_ADJUST,KC_DEL,KC_NO,KC_NO, KC_NO \   
                                                               */                                                           
    [_QWERTY] = LAYOUT_ortho_16x5(\
   CC_T3, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   CC_PCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, MO(_FN), KC_LGUI, KC_LALT,   CC_SPLW,LT(_TENKEY,KC_INT4),LT(_RAISE,KC_ENT),CC_SPLW,KC_BSPC, KC_RALT,CC_T1,CC_ADJUST,CC_T2,KC_NO,KC_NO, KC_NO \
                                  ),
    [_LOWER] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  CC_PR1,CC_PCB1,CC_TW1, \
   JP_TILD,S(KC_1),S(KC_2),  S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),S(KC_MINS),S(KC_EQL),KC_NO,KC_NO,KC_NO, \
   KC_ESC,  KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_A, KC_B, KC_C, \
   KC_LSFT, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   S(KC_LEFT),KC_UP,S(KC_RIGHT),KC_A,KC_B,KC_C, \
   KC_LCTL, _______, KC_LGUI,KC_LALT, KC_SPC,  _______, _______, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_RAISE] = LAYOUT_ortho_16x5(                                      \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                       ),
    [_LOWERJ] = LAYOUT_ortho_16x5(         \
   JP_GRV, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  CC_PR1,CC_PCB1,CC_TW1, \
   JP_TILD,JP_EXLM, JP_AT,   JP_HASH, JP_DLR,  JP_PERC, JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, JP_UNDS, JP_PLUS, KC_NO, KC_NO, KC_NO, \
   KC_ESC,  KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   S(KC_LEFT),KC_UP,S(KC_RIGHT),KC_NO,KC_NO,KC_NO, \
   KC_LCTL, _______, KC_LGUI,KC_LALT, KC_SPC,  _______, _______, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO  \
                                                                        ), \
    [_CURSOR] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  _______, KC_UP,   _______, _______, _______, _______, _______, TG(_CURSOR),_______,_______,_______,_______, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_LEFT,KC_DOWN,KC_RIGHT,  _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,_______, _______, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, S(KC_LEFT),KC_UP,S(KC_RIGHT), KC_NO,KC_NO,KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO  \
                                                                        ), \
    [_TCURSOR] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_NO, KC_NO, KC_NO, \
   KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, S(KC_LEFT),KC_UP,S(KC_RIGHT),KC_NO,KC_NO,KC_NO, \
   KC_LCTL, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN,KC_RIGHT,   KC_NO, KC_NO, KC_NO  \
                                                                        ), \
    [_TENKEY] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_NUM,  KC_PSLS,KC_PAST,   KC_PMNS, KC_BSPC, KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_P7,   KC_P8,   KC_P9,    KC_PPLS, KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_P4,   KC_P5,   KC_P6,    KC_PPLS, KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_P1,   KC_P2,   KC_P3,    KC_PENT ,S(KC_LEFT),S(KC_UP),S(KC_RIGHT), KC_NO, KC_NO, KC_NO, \
   KC_LCTL, CC_ADJUST, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_P0,   KC_P0,   KC_PDOT,  KC_PENT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_MOUSE] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    CC_SCRL, MS_BTN1 ,MS_BTN3,MS_BTN2,CC_SCRL, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, KC_N,    MS_BTN4, KC_COMM, KC_DOT,S(KC_LEFT), KC_UP,S(KC_RIGHT), KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, MS_BTN2, CC_SCRL, MS_BTN1, KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT,  KC_DOWN, KC_RIGHT, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_FN] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, KC_AUDIO_MUTE,KC_AUDIO_VOL_DOWN,KC_AUDIO_VOL_UP,KC_4,KC_BRIGHTNESS_DOWN,KC_BRIGHTNESS_UP,KC_7,KC_8,KC_CONTROL_PANEL,KC_ASSISTANT,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT,  CC_ADJUST, KC_DEL, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_ADJUST] = LAYOUT_ortho_16x5(                                       \
   CC_CTL_INV,CC_USUS,CC_USJP,TG(_CURSOR),TG(_TENKEY),CC_TBUP,CC_TBDWN,KC_7,KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   CC_INFO,CC_ATTDN,CC_ATTUP,CC_TMSDN,CC_TMSUP,    KC_T,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   CC_DUMP,CC_GACDN,CC_GACUP,CC_TMCDN,CC_TMCUP,    KC_G,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT,CC_PRXDN,CC_PRXUP,_______,_______,      KC_B,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS,KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT,  CC_ADJUST, KC_DEL,KC_NO, KC_NO, KC_NO \
                                                                        ), \
};

// for pseudo control (Emacs like cursor movement) functions
bool process_pseudo_control(uint16_t keycode, keyrecord_t *record) {
    int8_t flag;
    int8_t rows;
    uint32_t c_time, l_time;

    if (keycode != CC_PCTL) { // not pseudo control - do nothing.
        return(true); 
    }

    if (record->event.pressed) { // pseudo control is pressed.
        // check the touch sensor (the touch wall)
        l_time = timer_read();
        while ((c_time = (abs(timer_read() - l_time))) < 60) {
            rows =readMatrixCol(15); // read the touch wall status
            if (rows == 0) { // the touch wall is on.
                sprintf(debug_str, "%ld", c_time);
                dprintf(debug_str);
                my_modifier |= 1 << MM_TW; // The touch wall modifier is on
                break;
            }
        }
        
        // My Modifier: Touch Wall on, Keyboard mode: Psuedo Control: Invert mode.
        flag = (((my_modifier & 1 << MM_TW) >= 1) ^ ((kb_mode & 1 << M_PCTL_INV) >= 1));
        // have checked that the pseudo control is pressed.
        if (flag == 1) { // The touch wall is on, register ctrl key, not activate cursor orverrides.
            register_code(KC_LCTL);
            // My Modifier: Psuedo Controll off
            my_modifier = my_modifier & ~(1 << MM_PC);
            sprintf(debug_str, "normal");
            dprint(debug_str);
            //
        } else { // register cursor key overrides
            // sprintf(debug_str, "emacs");
           // dprint(debug_str);
            cursor_overrides_on();
            register_code(KC_LCTL);
        }
        my_modifier |= 1 << MM_PC; // Pseudo Control is on.
        return false; // handled.
    } else { // CC_PCTL is not pressed.
        //clear_overrides();
        cursor_overrides_off();
        unregister_code(KC_LCTL);
        my_modifier = my_modifier & ~(1 << MM_PC); // Pseudo Contrl is off.
        return false; // handled.
    }
    return(true);
}

// for space - lower key
bool process_cc_spc_lower(uint16_t keycode, keyrecord_t *record) {
    //    static int32_t pressed_time = 0;
    static int8_t flag = 0; // LSB: Space Flag, 1B: Lower Flag

    if (keycode != CC_SPLW) { // not space - lower
        if(record->event.pressed) { // key is just pressed. (exclude released.)
            if (flag & (1 << 0)) { // Space Flag is on
                flag = 1 << 1; // Lower Flag is on
            }
        }
        return(true); // not handled.
    }
    
    if (record->event.pressed) { // space - lower is pressed.
        //        pressed_time = timer_read();
        flag = 1 << 0; // Space Flag is on.
        if (kb_mode & (1 << M_USJP)) { // US-JP mode
            layer_on(_LOWERJ); // _LOWERJ layer is on.
        } else {
            layer_on(_LOWER); // _LOWER layer is on.
        }
    } else { // released.
        layer_off(_LOWER); // _LOWER layer is on.
        layer_off(_LOWERJ); // _LOWER layer is on.
        if (flag & (1 << 0)) { // Space Flag is on
            //register_code(KC_SPC); // resgister space key.
            //unregister_code(KC_SPC); // resgister space key.
            tap_code(KC_SPC);
        }
        flag = 0; // space - lower is not pressed.
    }
    return(false); // handled.
}


void print_keymap(void){
    sprintf(debug_str, "TG(_CURSOR),CC_USUS,CC_USJP,CC_CTL_INV,TG(_TENKEY),CC_TBUP,CC_TBDWN,KC_7,KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1\n");
    SEND_STRING(debug_str);
    wait_ms(10);
    sprintf(debug_str, "CC_INFO,CC_ATTDN,CC_ATTUP,CC_TMSDN,CC_TMSUP,    KC_T,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO\n");
    SEND_STRING(debug_str);
    wait_ms(10);
    sprintf(debug_str, "CC_DUMP,CC_GACDN,CC_GACUP,CC_TMCDN,CC_TMCUP,    KC_G,  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO\n");
    SEND_STRING(debug_str);
    wait_ms(10);
    sprintf(debug_str, "KC_LSFT,CC_PRXDN,CC_PRXUP,_______,_______,      KC_B,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS,KC_NO, KC_NO, KC_NO\n");
    SEND_STRING(debug_str);
    wait_ms(10);
    return;
}

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=false;
  debug_keyboard=false;
  //  debug_mouse=false;

  // key overrides
  init_overrides();
  // default: US-JP mode
  set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);
  kb_mode |= 1 << M_USJP;
  kb_mode &= ~(1 << M_USUS);

  // RGB Lighting
  rgblight_enable();
  rgblight_mode((uint8_t)(RGB_MODE_BREATHE));
  rgblight_sethsv(HSV_CYAN);

#ifdef AZOTEQ_IQS5XX_TPS43
  // TPS43
  // default:
  // Global ATI C 1;Global ATI Target 700;Prox Th 23;Global Touch Multiplier - Set 16;Global Touch Multiplier - clear 12;t:0;df:7,6,250;

  // Global Touch Multiplier - Set: 16 -> 12
  set_global_touch_multiplier_set(12);
  //Global Touch Multiplier - clear: 12 -> 8
  set_global_touch_multiplier_clear(8);
#endif
}

// Scrolling
report_mouse_t tp_scroll(report_mouse_t mouse_report) {
  static float carry_over_x = 0;
  static float carry_over_y = 0;
  float scrl_mag = SCRL_MAG;

  // mouse off: scrolling
  carry_over_x = carry_over_x + mouse_report.x * scrl_mag;
  // ceil carry_over values.
  if (carry_over_x > 127.0) {
	  carry_over_x = 127.0;
  }
  if (carry_over_x < -127.0) {
	  carry_over_x = -127.0;
  }
  mouse_report.h = (int8_t)carry_over_x;
  carry_over_x = carry_over_x - mouse_report.h;
  
  carry_over_y = carry_over_y + mouse_report.y * scrl_mag;
  if (carry_over_y > 127.0) {
	  carry_over_y = 127.0;
  }
  if (carry_over_y < -127.0) {
	  carry_over_y = -127.0;
  }
  mouse_report.v = (int8_t)carry_over_y;
  carry_over_y = carry_over_y - mouse_report.v;
  // mouse_report.v = -mouse_report.v;
#ifdef CONSOLE_ENABLE
	if (mouse_report.y != 0) {
        sprintf(debug_str, "%d,%d,%d:", mouse_report.y, mouse_report.v, (uint16_t)(carry_over_y*1000));
        dprint(debug_str);
	}
#endif
    // fix mouse pointer.
	mouse_report.x = 0;
	mouse_report.y = 0;
    dprint(".");
    return (mouse_report);
}

report_mouse_t two_fingers_scroll(report_mouse_t mouse_report) {
  static float carry_over_h = 0;
  static float carry_over_v = 0;
  float scrl_mag = SCRL_MAG_TWO_FINGERS;

  // mouse off: scrolling
  carry_over_h = carry_over_h + mouse_report.h * scrl_mag;
  // ceil carry_over values.
  if (carry_over_h > 127.0) {
	  carry_over_h = 127.0;
  }
  if (carry_over_h < -127.0) {
	  carry_over_h = -127.0;
  }
  mouse_report.h = (int8_t)carry_over_h;
  carry_over_h = carry_over_h - mouse_report.h;
  
  carry_over_v = carry_over_v + mouse_report.v * scrl_mag;
  if (carry_over_v > 127.0) {
	  carry_over_v = 127.0;
  }
  if (carry_over_v < -127.0) {
	  carry_over_v = -127.0;
  }
  mouse_report.v = (int8_t)carry_over_v;
  carry_over_v = carry_over_v - mouse_report.v;

#ifdef CONSOLE_ENABLE
	if (mouse_report.v != 0) {
        sprintf(debug_str, "%d,%d:", mouse_report.v, (uint16_t)(carry_over_v*1000));
        dprint(debug_str);
	}
#endif
    // no need: fix mouse pointer.
    // mouse_report.x = 0;
	// mouse_report.y = 0;
    return (mouse_report);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t rows;
    static uint8_t rc2, rc3;
    uint32_t c_time, l_time;

#ifdef AZOTEQ_IQS5XX_TPS43
    uint16_t ati_target;
    uint8_t global_ati_c;
    uint8_t prox_th;
    uint8_t global_touch_multiplier_set;
    uint8_t global_touch_multiplier_clear;
#endif
    
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    if (debug_keyboard == true) {
        uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    }
#endif

    if (process_pseudo_control(keycode, record) == false) {
        return(false);
    }

    if (process_cc_spc_lower(keycode, record) == false) {
        return(false);
    }
    
    switch (keycode) {
    case CC_T1: // left key
        if (record->event.pressed) {
            l_time = timer_read();
            while ((c_time = (abs(timer_read() - l_time))) < 60) {
                    rows =readMatrixCol(14);
                    if (rows == 0) {
                        sprintf(debug_str, "%ld", c_time);
                        dprintf(debug_str);
                        break;
                    }
                }
            register_code(KC_LEFT);
        } else {
            unregister_code(KC_LEFT);
        }
        return(false);

    case CC_T2: // right or Del key
        if (record->event.pressed) {
            l_time = timer_read();
            rc2 = 0; // KC_DEL
            while ((c_time = (abs(timer_read() - l_time))) < 60) {
                    rows =readMatrixCol(14);
                    if (rows == 0) {
                        sprintf(debug_str, "%ld", c_time);
                        dprintf(debug_str);
                        rc2 = 1; // KC_RIGHT
                        break;
                    }
                }
            if (rc2 == 0) {
                register_code(KC_DEL);
            } else {
                register_code(KC_RIGHT);
            }
        } else {
            if (rc2 == 0) {
                unregister_code(KC_DEL);
            } else {
                unregister_code(KC_RIGHT);
            }
        }
        return(false);
        
    case CC_T3:
        if (record->event.pressed) {
            l_time = timer_read();
            rc3 = 0; // Grave ESC mode
            while ((c_time = (abs(timer_read() - l_time))) < 100) {
                // check the touch wall status.
                rows =readMatrixCol(15);
                if (rows == 0) {
                    sprintf(debug_str, "%ld", c_time);
                    dprintf(debug_str);
                    layer_on(_CURSOR);
                    rc3 = 1; // cursor mode
                    return(false);
                }
            }
            if (rc3 == 0) {
                if (kb_mode & (1 << M_USJP)) { // US-JP mode
                    if (get_mods() & MOD_MASK_ALT) {
                        // A shift key or a alt key is pressed.
                        register_code(KC_GRV); // Grave(JP_ZKHK) + ALT -> Kana / Kanji Toggle.
                    } else {
                        if (get_mods() & MOD_MASK_SHIFT) {
                            // A shift key or a alt key is pressed.
                            register_code(KC_EQL); // JP_CIRC (= KC_EQL) + shift -> JP_TILD
                        } else {
                            register_code(KC_ESC);
                        }
                    }
                } else { //US-US mode
                    if (get_mods() & (MOD_MASK_SHIFT | MOD_MASK_ALT)) {
                        // A shift key or a alt key is pressed.
                        register_code(KC_GRV);
                    } else {
                        register_code(KC_ESC);
                    }
                }
            }
        } else { // not pressed.
            if (rc3 == 0) { // Grave ESC mode
                if (kb_mode & (1 << M_USJP)) { // US-JP mode
                    if (get_mods() & MOD_MASK_ALT) {
                        // A shift key or a alt key is pressed.
                        unregister_code(KC_GRV);
                    } else {
                        if (get_mods() & MOD_MASK_SHIFT) {
                            // A shift key or a alt key is pressed.
                            unregister_code(KC_EQL); // JP_CIRC (= KC_EQL) + shift -> JP_TILD
                        } else {
                            unregister_code(KC_ESC);
                        }
                    }
                } else { // US-US mode
                    if (get_mods() & (MOD_MASK_SHIFT | MOD_MASK_ALT)) {
                        // A shift key or a alt key is pressed.
                        unregister_code(KC_GRV);
                    } else {
                        unregister_code(KC_ESC);
                    }
                }
            }
        }
        return(false);
        /*        
        if (record->event.pressed) {
            l_time = timer_read();
            rc3 = 0; // QK_GESC
            while ((c_time = (abs(timer_read() - l_time))) < 100) {
                    rows =readMatrixCol(15);
                    if (rows == 0) {
                        sprintf(debug_str, "%ld", c_time);
                        dprintf(debug_str);
                        layer_on(_CURSOR);
                        rc3 = 0;
                        return(false);
                    }
                }
            if (rc3 == 0) {
            register_code(KC_ESC);
            }
        } else {
            if (rc3 == 0) {
                unregister_code(KC_ESC);
            }
        }
        return(false);
        */
        
    case CC_TW1:
        // The left keys with touch sensors (or the touch wall) is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_TW;
        } else {
            my_modifier = my_modifier & ~(1 << MM_TW);
        }
        return false;
        
    case CC_PR1:
        // The palm rest touch sensor is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_PR;
        } else {
            my_modifier = my_modifier & ~(1 << MM_PR);
        }
        return false;

    case CC_PCB1:
        // The Touch Sensor on the PCB (for cursor keys control) is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_PCB;
            // register_code(KC_P);
            layer_on(_TCURSOR);
            rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
            rgblight_sethsv(HSV_RED);

        } else {
            my_modifier = my_modifier & ~(1 << MM_PCB);
            // unregister_code(KC_P);
            layer_off(_TCURSOR);
            rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
            rgblight_sethsv(HSV_CYAN);

        }
        return false;
        
    case CC_USUS:
        if (record->event.pressed) {
            init_overrides();
            set_overrides(dummy_overrides, dummy_overrides, emacs_overrides);
            kb_mode |= 1 << M_USUS;
            kb_mode &= ~(1 << M_USJP);
            //            rgblight_mode((uint8_t)(RGB_MODE_BREATHE));
            rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
            rgblight_sethsv(HSV_GREEN);
            dprint("USUS");
            return false; // Do not let QMK process the keycode further
        }
        return true;

    case CC_USJP:
        if (record->event.pressed) {
            init_overrides();
            set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);
            kb_mode |= 1 << M_USJP;
            kb_mode &= ~(1 << M_USUS);
            //            rgblight_mode((uint8_t)(RGB_MODE_BREATHE) + 1);
            rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
            rgblight_sethsv(HSV_CYAN);
            dprint("USJP");
            return false; // Do not let QMK process the keycode further
        }
        return true;

    case CC_CTL_INV:
        if (record->event.pressed) {
            kb_mode ^= 1 << M_PCTL_INV;
            sprintf(debug_str, "CTL_INV:%x", kb_mode);
            if ((kb_mode & (1 << M_PCTL_INV)) >= 1) {
                // rgblight_mode((uint8_t)(RGBLIGHT_MODE_RAINBOW_MOOD));
                rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
                rgblight_sethsv(HSV_RED);
                } else {
                // rgblight_mode((uint8_t)(RGBLIGHT_MODE_BREATHING));
                rgblight_mode((uint8_t)(RGBLIGHT_MODE_STATIC_LIGHT));
                rgblight_sethsv(HSV_YELLOW);
                }
            dprint(debug_str);
            return false; // Do not let QMK process the keycode further
        }
        return true;

    case CC_TENKEY:
        break;
    case CC_LOWER:
        break;
    case CC_RAISE:
        break;
    case CC_INFO:
        if (record->event.pressed) {
            // SEND_STRING("CC_INFO");
            if ((kb_mode & 1 << M_USUS) >= 1) {
                SEND_STRING("USUS;");
                rgblight_sethsv(HSV_GREEN);
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_USJP) >= 1) {
                SEND_STRING("USJP;");
                rgblight_sethsv(HSV_CYAN);
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_EMACS) >= 1) {
                SEND_STRING("EMACS;");
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_VI) >= 1) {
                SEND_STRING("VI;");
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_WASD) >= 1) {
                SEND_STRING("WASD;");
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_TENKEY) >= 1) {
                SEND_STRING("TENKEY;");
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_PCTL_INV) >= 1) {
                SEND_STRING("PCTL_INV;");
                rgblight_sethsv(HSV_YELLOW);
                wait_ms(10);
            }
            if ((kb_mode & 1 << M_CURSOR_ON) >= 1) {
                SEND_STRING("CURSOR_ON;");
                wait_ms(10);
            }
#ifdef AZOTEQ_IQS5XX_TPS43
            print_iqs5xx_config();
#endif
            // print_keymap();
            return false;
        }
        return true;
        
    case CC_ADJUST:
        if (record->event.pressed) {
            layer_on(_ADJUST);
        } else {
            layer_off(_ADJUST);
        }
        return false;
    
    case CC_FN:
        if ((my_modifier & (1 << MM_TW)) >= 1) {
            if (record->event.pressed) {
                // if adjust mode is on, pressing fn key goes adjust mode off.
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
        }
        return true;

    case QK_GESC:
        if ((my_modifier & (1 << MM_TW)) >= 1) {
            // for vi cursor mode
            if (record->event.pressed) {
                layer_on(_CURSOR);
                uprint("CURSOR");
            }
            return false;
        }
        return true;
        
    case KC_ESC:
        // Detect the activation of only Left Alt
        if (get_mods() == MOD_BIT(KC_LALT)) {
            if (record->event.pressed) {
                // No need to register KC_LALT because it's already active.
                // The Alt modifier will apply on this KC_TAB.
                register_code(KC_TAB);
            } else {
                unregister_code(KC_TAB);
            }
            // Do not let QMK process the keycode further
            return false;
        }
        // Else, let QMK process the KC_ESC keycode as usual
        return true;

#ifdef AZOTEQ_IQS5XX_TPS43
    case CC_ATTUP:
        // IQS5xx ATI Target Up.
        if (record->event.pressed) {
            ati_target = get_ati_target();
            ati_target = ati_target + 50;
            set_ati_target2(ati_target);
            return false;
        }
        return true;

    case CC_ATTDN:
        // IQS5xx ATI Target Down.
        if (record->event.pressed) {
            ati_target = get_ati_target();
            ati_target = ati_target - 50;
            set_ati_target2(ati_target);
            return false;
        }
        return true;

    case CC_GACUP:
        // IQS5xx ATI Target Up.
        if (record->event.pressed) {
            global_ati_c = get_global_ati_c();
            global_ati_c = global_ati_c + 1;
            set_global_ati_c(global_ati_c);
            return false;
        }
        return true;

    case CC_GACDN:
        // IQS5xx ATI Target Downp.
        if (record->event.pressed) {
            global_ati_c = get_global_ati_c();
            global_ati_c = global_ati_c - 1;
            set_global_ati_c(global_ati_c);
            return false;
        }
        return true;

    case CC_PRXUP:
        // IQS5xx Prox Threshold Up.
        if (record->event.pressed) {
            prox_th = get_prox_th();
            prox_th = prox_th + 1;
            set_prox_th(prox_th);
            return false;
        }
        return true;

    case CC_PRXDN:
        // IQS5xx Prox Threshold Down.
        if (record->event.pressed) {
            prox_th = get_prox_th();
            prox_th = prox_th - 1;
            set_prox_th(prox_th);
            return false;
        }
        return true;

    case CC_TMSUP:
        // IQS5xx Global Touch Multiplier - set Up.
        if (record->event.pressed) {
            global_touch_multiplier_set = get_global_touch_multiplier_set();
            global_touch_multiplier_set = global_touch_multiplier_set + 1;
            set_global_touch_multiplier_set(global_touch_multiplier_set);
            return false;
        }
        return true;

    case CC_TMSDN:
        // IQS5xx Global Touch Multiplier - set down.        
        if (record->event.pressed) {
            global_touch_multiplier_set = get_global_touch_multiplier_set();
            global_touch_multiplier_set = global_touch_multiplier_set - 1;
            set_global_touch_multiplier_set(global_touch_multiplier_set);
            return false;
        }
        return true;

    case CC_TMCUP:
        // IQS5xx Global Touch Multiplier - clear Up.
        if (record->event.pressed) {
            global_touch_multiplier_clear = get_global_touch_multiplier_clear();
            global_touch_multiplier_clear = global_touch_multiplier_clear + 1;
            set_global_touch_multiplier_clear(global_touch_multiplier_clear);
            return false;
        }
        return true;

    case CC_TMCDN:
        // IQS5xx Global Touch Multiplier - clear down.        
        if (record->event.pressed) {
            global_touch_multiplier_clear = get_global_touch_multiplier_clear();
            global_touch_multiplier_clear = global_touch_multiplier_clear - 1;
            set_global_touch_multiplier_clear(global_touch_multiplier_clear);
            return false;
        }
        return true;
        
    case CC_SCRL:
        // Track Pad scrolling
        if (record->event.pressed) {
            scroll_flag = true;
            dprint("-");
        } else {
            scroll_flag = false;
            dprint("_");
        }
        return true;

    case CC_DUMP:
        // dump register of ISQ5xx
        if (record->event.pressed) {
            dump_reg(0x0000, 0x0038);
            dump_reg(0x042f, 0x0434);
            dump_reg(0x056b, 0x0597);
            dump_reg(0x062e, 0x06cf);
        }
        return true;
#endif
    }
    return true;
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
#ifdef AZOTEQ_IQS5XX_TPS43
    static uint32_t c_time;
    uint8_t reg_value;

    reg_value = get_reg8(AZOTEQ_IQS5XX_REG_NUMBER_OF_FINGERS);

    if ((mouse_report.x != 0) || (mouse_report.y != 0) || (reg_value >= 1)) {
        c_time = timer_read();
        layer_on(_MOUSE);
    } else {
        if (abs((timer_read() - c_time)) > 200) { // over 200ms
            layer_off(_MOUSE);
        }
    }
    
    if ((mouse_report.h != 0) || (mouse_report.v != 0)) {
        mouse_report = two_fingers_scroll(mouse_report);
    }

    if (scroll_flag == true) {
        mouse_report = tp_scroll(mouse_report);
        mouse_report.buttons = 0; // suppress buttons when scrolling.
        return(mouse_report);
    }
#endif
    return(mouse_report);
}
