#include QMK_KEYBOARD_H
#include <print.h>
#include "keymap_japanese.h"
#include "quantum_keycodes.h"
#include "print.h"
#include "debug.h"

#define TIME_OUT 1000
#define TH_ON 10
#define TH_OFF 8

#define _QWERTY 0
#define _LOWER  1
#define _RAISE  2
#define _EMACS  3
#define _VI     4
#define _TENKEY 5
#define _MOUSE  6
#define _FN     14
#define _ADJUST 15

#define MAX_KEY_OVERRIDES 35

// Keyboard Mode: 0: normal(usus), 1: us key on jp os, 2: emacs, 3: vi, 4: wasd, 5: Touch wall invert mode, 
#define M_USUS 0
#define M_USJP 1
#define M_EMACS 2
#define M_VI 3
#define M_WASD 4
#define M_TW_INV 5
#define M_CURSOR_ON 6

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
    CC_USUS,
    CC_USJP,
    CC_FN,
    CC_LOWER,
    CC_RAISE,
    CC_INFO,
    CC_TBUP,
    CC_TBDWN
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
     &ko_make_basic(MOD_MASK_SHIFT, KC_GRV, JP_TILD),
     &ko_make_basic(MOD_MASK_SHIFT, KC_2, JP_AT),
     &ko_make_basic(MOD_MASK_SHIFT, KC_6, JP_CIRC),
     &ko_make_basic(MOD_MASK_SHIFT, KC_7, JP_AMPR),
     &ko_make_basic(MOD_MASK_SHIFT, KC_8, JP_ASTR),
     &ko_make_basic(MOD_MASK_SHIFT, KC_9, JP_LPRN),
     &ko_make_basic(MOD_MASK_SHIFT, KC_0, JP_RPRN),
     &ko_make_basic(MOD_MASK_SHIFT, KC_MINS, JP_UNDS),
     &ko_make_basic(0, KC_EQL, JP_EQL),
     &ko_make_basic(MOD_MASK_SHIFT, KC_EQL, JP_PLUS),
     &ko_make_basic(0, KC_LBRC, JP_LBRC),
     &ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, JP_RBRC),
     &ko_make_basic(0, KC_BSLS, JP_BSLS),
     &ko_make_basic(MOD_MASK_SHIFT, KC_BSLS, JP_PIPE),
     &ko_make_basic(MOD_MASK_SHIFT, KC_SCLN, JP_COLN),
     &ko_make_basic(0, KC_QUOT, JP_QUOT),
     &ko_make_basic(MOD_MASK_SHIFT, KC_QUOT, JP_DQUO),
     NULL
    };


const key_override_t *emacs_overrides[] =
    {
     &ko_make_basic(MOD_MASK_CTRL, KC_F, KC_RIGHT),
     &ko_make_basic(MOD_MASK_CTRL, KC_B, KC_LEFT),
     &ko_make_basic(MOD_MASK_CTRL, KC_P, KC_UP),
     &ko_make_basic(MOD_MASK_CTRL, KC_N, KC_DOWN),
     &ko_make_basic(MOD_MASK_CTRL, KC_V, KC_PGDN),
     &ko_make_basic(MOD_MASK_CTRL, KC_A, KC_HOME),
     &ko_make_basic(MOD_MASK_CTRL, KC_E, KC_END),
     //     &ko_make_basic(MOD_MASK_ALT,  KC_V, KC_PGUP),
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

/*
static const key_override_t *no_override[] =
    {
     NULL
    };
*/

const key_override_t dummy_override = ko_make_basic(0, CC_INFO, CC_INFO); // no effect

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
     */
    [_QWERTY] = LAYOUT_ortho_16x5(\
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   CC_PCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                  ),
    [_LOWER] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_GRV,  KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS, KC_PLUS, KC_A, KC_B, KC_C, \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_A, KC_B, KC_C, \
   KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_A, KC_B, KC_C, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_RAISE] = LAYOUT_ortho_16x5(                                      \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                       ),
    [_EMACS] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ),
    [_VI] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_TENKEY] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_MOUSE] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_FN] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
                                                                        ), \
    [_ADJUST] = LAYOUT_ortho_16x5(                                       \
   KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  CC_PR1,CC_PCB1,CC_TW1, \
   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_NO, KC_NO, KC_NO, \
   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_BSLS, KC_NO, KC_NO, KC_NO, \
   KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC, KC_ENT,   KC_INT4, KC_SPC,  KC_BSPC, KC_RALT, KC_DEL,  KC_CAPS, KC_BSPC, KC_NO, KC_NO, KC_NO \
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
    clear_overrides();
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

/* todo
psuedo controlがpressedの場合、
cursor overridesをregister
control wo register
unpressed :
cursor overrides wo pop
control wo unregister


key override sets はどうか？
 */
bool process_pseudo_control(uint16_t keycode, keyrecord_t *record) {
    if (keycode != CC_PCTL) { // not pseudo control - do nothing.
        return(true); 
    }
    if (record->event.pressed) { // pseudo control is pressed.
        // have checked that the pseudo control is pressed.
        if ((my_modifier & 1 << MM_TW) >= 1) { // The touch wall is on, register ctrl key.
            register_code(KC_LCTL);
            return false; // handled.
        } else { // register cursor key overrides
            sprintf(debug_str, "emacs");
            dprint(debug_str);
            // clear_overrides();
            // add_overrides(emacs_overrides);
            cursor_overrides_on();
            register_code(KC_LCTL);
            return false; // handled.
        }
    } else { // CC_PCTL is not pressed.
        //clear_overrides();
        cursor_overrides_off();
        unregister_code(KC_LCTL);
        return false; // handled.
    }
    return(true);
}

/*
void register_us_key_on_jp_os_overrides(void) {
    remove_all_os_key_overrides();

    for (int idx = 0; idx < sizeof(us_key_on_jp_os_overrides) / sizeof(us_key_on_jp_os_overrides[0]); idx++) {
        register_os_key_override(us_key_on_jp_os_overrides[idx]);
    }

    if (keymap_config.swap_lalt_lgui) {
        register_os_key_override(grv_override_jp_mac);
    } else {
        register_os_key_override(grv_override_jp_win);
    }
}
*/

/*
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

    // *on_delay = 0;
    // *off_delay = 0;
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
*/

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  debug_matrix=true;
  debug_keyboard=true;
  //  debug_mouse=false;

  // key overrides
  set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // If console is enabled, it will print the matrix position and status of each key pressed
    #ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    if (process_pseudo_control(keycode, record) == false) {
        return(false);
    }
    switch (keycode) {
    case CC_TW1:
        // The touch wall is on.
        if (record->event.pressed) {
            my_modifier = my_modifier | 1 << MM_TW;
        } else {
            my_modifier = my_modifier & ~(1 << MM_TW);
        }
        return false;
        
        // case CC_PR1:
        
        // case CC_PCTL:
        
    case CC_USUS:
        set_overrides(dummy_overrides, dummy_overrides, emacs_overrides);
        kb_mode |= 1 << M_USUS;
        kb_mode &= ~(1 << M_USJP);
        set_overrides(dummy_overrides, dummy_overrides, emacs_overrides);
        return false; // Do not let QMK process the keycode further
        
    case CC_USJP:
        set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);
        kb_mode |= 1 << M_USJP;
        kb_mode &= ~(1 << M_USUS);
        set_overrides(us_key_on_jp_os_overrides, dummy_overrides, emacs_overrides);
        return false; // Do not let QMK process the keycode further
        
    case CC_FN:
    case CC_LOWER:
    case CC_RAISE:
    case CC_INFO:
        // case CC_TBUP:
        // case CC_TBDWN:

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
