#include QMK_KEYBOARD_H
#include <print.h>
#include "keymap_japanese.h"
#include "quantum_keycodes.h"
#include "print.h"
#include "debug.h"

#define DEBUG

#define TIME_OUT 1000
#define TH_ON 10
#define TH_OFF 8

// layers:
#define _QWERTY 0
#define _LOWER  1
#define _RAISE  2
#define _LOWERJ 3
#define _CURSOR 4
#define _TENKEY 5
#define _MOUSE  6
#define _FN     14
#define _ADJUST 15

#define MAX_KEY_OVERRIDES 35

// Keyboard Control Mode:
// 0: normal(usus), 1: us key on jp os, 2: emacs, 3: vi, 4: wasd, 5: Touch wall invert mode, 
#define M_USUS 0
#define M_USJP 1
#define M_EMACS 2
#define M_VI 3
#define M_WASD 4
#define M_TENKEY 5
#define M_PCTL_INV 6
#define M_CURSOR_ON 7

static uint16_t kb_mode = 1 << M_USUS;
static char debug_str[256];

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

enum custom_keycodes {
    CC_TW1 = SAFE_RANGE,  // touch wall 1
    CC_PCB1, // touch key PCB 1 (on the main PCB)
    CC_PR1,  // touch key 2 (palm rest)
    CC_PCTL, // Pseudo-Control
    CC_FN,
    CC_LOWER,
    CC_RAISE,
    CC_ADJUST,
    CC_INFO,
    CC_TBUP,
    CC_TBDWN,
    CC_USUS,
    CC_USJP,
    CC_EMACS,
    CC_CTL_INV, // ctrl key invert
    CC_CSRN, // cursor on
    CC_CSRF, // cursor off
    CC_TENKEY,
    CC_DUMMY
};

static uint8_t my_modifier = 0;

// Key Override
// Mode: NoOverride, US(Key)2JIS(OS), JIS2US,
// Cursor Mode: No, Ctrl + Emacs (the touch key cancellation), Ctrl + the touch key + Emacs, vi(with the touch key),
const key_override_t **key_overrides;
static const key_override_t *working_overrides[MAX_KEY_OVERRIDES];
static const key_override_t *saved_overrides[MAX_KEY_OVERRIDES];
static int8_t n_overrides;
static int8_t pos_dummy_override; // for on/off cursor overrides

// Perform as a US keyboard on JP OS systems
const key_override_t *us_key_on_jp_os_overrides[] =
    {
     &ko_make_with_layers(MOD_MASK_SHIFT, QK_GESC, JP_TILD, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_2, JP_AT, 1 << _QWERTY | 1 << _LOWER),
     // &ko_make_with_layers_negmods_and_options(MOD_MASK_SHIFT, KC_2, JP_AT, 1 << _QWERTY | 1 << _LOWER, 0, ko_option_activation_trigger_down),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_6, JP_CIRC, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_7, JP_AMPR, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_8, JP_ASTR, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_9, JP_LPRN, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_0, JP_RPRN, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_MINS, JP_UNDS, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_EQL, JP_PLUS, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(0, KC_EQL, JP_EQL, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(0, KC_LBRC, JP_LBRC, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_RBRC, JP_RCBR, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(0, KC_RBRC, JP_RBRC, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_BSLS, JP_PIPE, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(0, KC_BSLS, JP_YEN, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_SCLN, JP_COLN, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_QUOT, JP_DQUO, 1 << _QWERTY | 1 << _LOWER),
     &ko_make_with_layers(0, KC_QUOT, JP_QUOT, 1 << _QWERTY | 1 << _LOWER),
     NULL
    };

const key_override_t *emacs_overrides[] =
    {
     &ko_make_with_layers(MOD_MASK_CTRL, KC_F, KC_RIGHT, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_B, KC_LEFT, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P, KC_UP, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_N, KC_DOWN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_V, KC_PGDN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_A, KC_HOME, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_E, KC_END, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_D, KC_DEL, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_ALT,  KC_V, KC_PGUP, 1 << _QWERTY),
     NULL
    };

const key_override_t *vi_overrides[] =
    {
     // KC_GRV override depends on OS
     &ko_make_basic(0, KC_H, KC_LEFT),
     &ko_make_basic(0, KC_J, KC_DOWN),
     &ko_make_basic(0, KC_K, KC_UP),
     &ko_make_basic(0, KC_L, KC_RIGHT),
     NULL
    };

const key_override_t *game_overrides[] =
    {
     // KC_GRV override depends on OS
     &ko_make_basic(MOD_MASK_CTRL, KC_W, KC_UP),
     &ko_make_basic(MOD_MASK_CTRL, KC_A, KC_LEFT),
     &ko_make_basic(MOD_MASK_CTRL, KC_S, KC_DOWN),
     &ko_make_basic(MOD_MASK_CTRL, KC_D, KC_RIGHT),
     NULL
    };

const key_override_t *tenkey_cursor_overrides[] =
    {
     // KC_GRV override depends on OS
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P8, KC_UP, 1 << _TENKEY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P4, KC_LEFT, 1 << _TENKEY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P2, KC_DOWN, 1 << _TENKEY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P6, KC_RIGHT, 1 << _TENKEY),
     NULL
    };

/*
static const key_override_t *no_override[] =
    {
     NULL
    };
*/

const key_override_t dummy_override = ko_make_basic(0, CC_DUMMY, CC_DUMMY); // no effect

const key_override_t *dummy_overrides[] =
    {
     // KC_GRV override depends on OS
     &dummy_override,
     NULL
    };


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
    [_QWERTY] = LAYOUT_ortho_16x5(\
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   CC_PCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, CC_FN, KC_LGUI, KC_LALT, KC_SPC,LT(_LOWER,KC_ENT),LT(_LOWER,KC_INT4), KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_INT5, CC_ADJUST,KC_NO,KC_NO, KC_NO \
                                  ),
    [_LOWER] = LAYOUT_ortho_16x5(                                       \
   CC_CSRN, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  CC_PR1,CC_PCB1,CC_TW1, \
   JP_TILD,S(KC_1),S(KC_2),  S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),S(KC_MINS),S(KC_EQL),KC_NO,KC_NO,KC_NO, \
   KC_ESC,  KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_A, KC_B, KC_C, \
   KC_LSFT, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_UP,   KC_F12,  KC_A, KC_B, KC_C, \
   KC_LCTL, _______, KC_LGUI,KC_LALT, KC_SPC,  _______, _______, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_RAISE] = LAYOUT_ortho_16x5(                                      \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                       ),
    [_LOWERJ] = LAYOUT_ortho_16x5(                                       \
   JP_GRV, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  CC_PR1,CC_PCB1,CC_TW1, \
   JP_TILD,JP_EXLM, JP_AT,   JP_HASH, JP_DLR,  JP_PERC, JP_CIRC, JP_AMPR, JP_ASTR, JP_LPRN, JP_RPRN, JP_UNDS, JP_PLUS, KC_NO, KC_NO, KC_NO, \
   KC_ESC,  KC_1,    KC_2,   KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_UP,   KC_F12,  KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI,KC_LALT, KC_SPC,  _______, _______, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO  \
                                                                        ), \
    [_CURSOR] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  _______, KC_UP,   _______, _______, _______, _______, _______, TG(_CURSOR),_______,_______,_______,_______, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_LEFT,KC_DOWN,KC_RIGHT,  _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT,_______, _______, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_UP,   _______, KC_NO,KC_NO,KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO  \
                                                                        ), \
    [_TENKEY] = LAYOUT_ortho_16x5(                                       \
   QK_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_NUM,  KC_PSLS,KC_PAST,   KC_PMNS, KC_BSPC, KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_P7,   KC_P8,   KC_P9,    KC_PPLS, KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_P4,   KC_P5,   KC_P6,    KC_PPLS, KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_P1,   KC_P2,   KC_P3,    KC_PENT ,KC_SLSH, KC_UP, CC_ADJUST, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, CC_ADJUST, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_P0,   KC_P0,   KC_PDOT,  KC_PENT, KC_LEFT, KC_DOWN, KC_RIGHT,KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_MOUSE] = LAYOUT_ortho_16x5(                                       \
   QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_FN] = LAYOUT_ortho_16x5(                                       \
   QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_ADJUST] = LAYOUT_ortho_16x5(                                       \
   TG(_CURSOR), CC_USUS, CC_USJP, CC_CTL_INV,CC_TBUP,CC_TBDWN,TG(_TENKEY),KC_7,KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   CC_INFO, _______, _______, _______, CC_TENKEY, KC_T,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, _______, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, CC_ADJUST,KC_NO, KC_NO, KC_NO \
                                                                        ), \
};

// Key override functions
void clear_overrides(void) {
    working_overrides[0] = NULL;
    n_overrides = 0;
}

void init_overrides(void) {
    clear_overrides();
    key_overrides = working_overrides;
}

int8_t add_overrides(const key_override_t **add) {
    int8_t j;
    for (j = 0; n_overrides < MAX_KEY_OVERRIDES - 1; n_overrides++, j++) {
        working_overrides[n_overrides] = add[j];
        if (working_overrides[n_overrides] == NULL) {
            return(n_overrides);
        }
    }
    working_overrides[++n_overrides] = NULL;
    return(n_overrides);
}

int8_t save_overrides(void) {
    int8_t i;
    for (i = 0; i <= n_overrides; i++) {
        saved_overrides[i] = working_overrides[i];
        if (saved_overrides[i] == NULL) {
            n_overrides = i;
            return(n_overrides);
        }
    }
    working_overrides[n_overrides] = NULL;
    return(n_overrides);
}

int8_t load_overrides(void) {
    int8_t i;
    for (i = 0; i <= n_overrides; i++) {
        working_overrides[i] = saved_overrides[i];
        if (working_overrides[i] == NULL) {
            n_overrides = i;
            return(n_overrides);
        }
    }
    working_overrides[n_overrides] = NULL;
    return(n_overrides);
}

int8_t set_overrides(const key_override_t **ko1, const key_override_t **ko2, const key_override_t **ko3) {
    // ko1: basic (for layout) overrides, ko2: basic 2, ko3: cursor overrides
    init_overrides();
    add_overrides(ko1);
    pos_dummy_override = add_overrides(ko2);
    n_overrides++; // cursor overrides are off.
    add_overrides(ko3);
    return(n_overrides);
}

int8_t cursor_overrides_on(void) {
    working_overrides[pos_dummy_override] =&dummy_override;
    return(true);
}

int8_t cursor_overrides_off(void) {
    working_overrides[pos_dummy_override] = NULL;
    return(true);
}

bool process_pseudo_control(uint16_t keycode, keyrecord_t *record) {
    if (keycode != CC_PCTL) { // not pseudo control - do nothing.
        return(true); 
    }
    int8_t flag;
    flag = (((my_modifier & 1 << MM_TW) >= 1) ^ ((kb_mode & 1 << M_PCTL_INV) >= 1));
    if (record->event.pressed) { // pseudo control is pressed.
        // have checked that the pseudo control is pressed.
        if (flag == 1) { // The touch wall is on, register ctrl key, not activate cursor orverrides.
        // if ((my_modifier & 1 << MM_TW) > 0xFF) { // Always off
            register_code(KC_LCTL);
            my_modifier = my_modifier & ~(1 << MM_PC);
            sprintf(debug_str, "normal");
            dprint(debug_str);
            //
        } else { // register cursor key overrides
            sprintf(debug_str, "emacs");
            dprint(debug_str);
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

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=false;
  debug_keyboard=false;
  //  debug_mouse=false;

  // key overrides
  init_overrides();
  set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);

  // RGB Lighting
  rgblight_enable();
  rgblight_mode((uint8_t)(RGB_MODE_BREATHE));
  rgblight_sethsv(HSV_CYAN);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    if (debug_keyboard == true) {
        uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    }
#endif

    if (process_pseudo_control(keycode, record) == false) {
        return(false);
    }
    switch (keycode) {
    case CC_TW1:
        // The touch wall 1 is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_TW;
        } else {
            my_modifier = my_modifier & ~(1 << MM_TW);
        }
        return false;
        
    case CC_PR1:
        // The Palm Rest Touch Sensor is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_PR;
        } else {
            my_modifier = my_modifier & ~(1 << MM_PR);
        }
        return false;

    case CC_PCB1:
        // The Touch Sensor on PCB is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_PCB;
        } else {
            my_modifier = my_modifier & ~(1 << MM_PCB);
        }
        return false;
        
    case CC_USUS:
        if (record->event.pressed) {
            init_overrides();
            set_overrides(dummy_overrides, dummy_overrides, emacs_overrides);
            kb_mode |= 1 << M_USUS;
            kb_mode &= ~(1 << M_USJP);
            rgblight_sethsv(HSV_GREEN);
            rgblight_mode((uint8_t)(RGB_MODE_BREATHE));
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
            rgblight_sethsv(HSV_CYAN);
            rgblight_mode((uint8_t)(RGB_MODE_BREATHE) + 1);
            dprint("USJP");
            return false; // Do not let QMK process the keycode further
        }
        return true;

    case CC_CTL_INV:
        if (record->event.pressed) {
            kb_mode ^= 1 << M_PCTL_INV;
            sprintf(debug_str, "CTL_INV:%x", kb_mode);
            if ((kb_mode & (1 << M_PCTL_INV)) >= 1) {
                    rgblight_mode((uint8_t)(RGBLIGHT_MODE_RAINBOW_MOOD));
                } else {
                    rgblight_mode((uint8_t)(RGBLIGHT_MODE_BREATHING));
                }
            dprint(debug_str);
            return false; // Do not let QMK process the keycode further
        }
        return true;

    case CC_TENKEY:
        break;

        //    case CC_FN:
        //        break;
    case CC_LOWER:
        break;
    case CC_RAISE:
        break;
    case CC_INFO:
        if (record->event.pressed) {
            // SEND_STRING("CC_INFO");
            if ((kb_mode & 1 << M_USUS) >= 1) {
                SEND_STRING("USUS;");
            }
            if ((kb_mode & 1 << M_USJP) >= 1) {
                SEND_STRING("USJP;");
            }
            if ((kb_mode & 1 << M_EMACS) >= 1) {
                SEND_STRING("EMACS;");
            }
            if ((kb_mode & 1 << M_VI) >= 1) {
                SEND_STRING("VI;");
            }
            if ((kb_mode & 1 << M_WASD) >= 1) {
                SEND_STRING("WASD;");
            }
            if ((kb_mode & 1 << M_TENKEY) >= 1) {
                SEND_STRING("TENKEY;");
            }
            if ((kb_mode & 1 << M_PCTL_INV) >= 1) {
                SEND_STRING("PCTL_INV;");
            }
            if ((kb_mode & 1 << M_CURSOR_ON) >= 1) {
                SEND_STRING("CURSOR_ON;");
            }
            return false;
        }
        return true;
        
        
    case CC_ADJUST:
        if (record->event.pressed) {
            layer_on(_ADJUST);
            uprint("ADJUST");
        } else {
            layer_off(_ADJUST);
            uprint("off");

        }
        return false;
    
        // case CC_TBUP:
        // case CC_TBDWN:

    case CC_FN:
        if ((my_modifier & (1 << MM_TW)) >= 1) {
            if (record->event.pressed) {
                layer_on(_ADJUST);
                uprint("ADJUST");
            } else {
                layer_off(_ADJUST);
                uprint("off");
            }
            return false;
        }
        return true;

    case QK_GESC:
        if ((my_modifier & (1 << MM_TW)) >= 1) {
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

    }
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
