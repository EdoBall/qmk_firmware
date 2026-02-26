#include QMK_KEYBOARD_H
#include <print.h>
#include "keymap_japanese.h"
#include "quantum_keycodes.h"

#include "keymap.h"
#include "key_overrides.h"

const key_override_t *key_overrides[MAX_KEY_OVERRIDES];
static int8_t n_overrides;
static int8_t pos_dummy_override; // for on/off cursor overrides

// Perform as a US keyboard on JP OS systems
const key_override_t *us_key_on_jp_os_overrides[] =
    {
     &ko_make_with_layers(MOD_MASK_SHIFT, QK_GESC, JP_TILD, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_2, JP_AT, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_6, JP_CIRC, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_7, JP_AMPR, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_8, JP_ASTR, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_9, JP_LPRN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_0, JP_RPRN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_MINS, JP_UNDS, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_EQL, JP_PLUS, 1 << _QWERTY),
     &ko_make_with_layers(0, KC_EQL, JP_EQL, 1 << _QWERTY),
     &ko_make_with_layers(0, KC_LBRC, JP_LBRC, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_RBRC, JP_RCBR, 1 << _QWERTY),
     &ko_make_with_layers(0, KC_RBRC, JP_RBRC, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_BSLS, JP_PIPE, 1 << _QWERTY),
     &ko_make_with_layers(0, KC_BSLS, JP_YEN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_SCLN, JP_COLN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_SHIFT, KC_QUOT, JP_DQUO, 1 << _QWERTY),
     &ko_make_with_layers(0, KC_QUOT, JP_QUOT, 1 << _QWERTY),
     /*
     &ko_make_with_layers(MOD_MASK_SHIFT, QK_GESC, JP_TILD,  1 << _LOWER),
     &ko_make_with_layers(0, JP_KC_2, JP_AT, 1 << _LOWER),
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
     */
     NULL
    };

const key_override_t *emacs_overrides[] =
    {
     &ko_make_with_layers(MOD_MASK_CTRL, KC_F, KC_RIGHT, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_B, KC_LEFT, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_P, KC_UP, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_N, KC_DOWN, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_A, KC_HOME, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_E, KC_END, 1 << _QWERTY),
     &ko_make_with_layers(MOD_MASK_CTRL, KC_D, KC_DEL, 1 << _QWERTY),
     //     &ko_make_with_layers(MOD_MASK_CTRL, KC_V, KC_PGDN, 1 << _QWERTY),
     //     &ko_make_with_layers(MOD_MASK_ALT,  KC_V, KC_PGUP, 1 << _QWERTY),
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

const key_override_t dummy_override = ko_make_basic(0, CC_DUMMY, CC_DUMMY); // no effect

const key_override_t *dummy_overrides[] =
    {
     // KC_GRV override depends on OS
     &dummy_override,
     NULL
    };

// Key Override
// Mode: NoOverride, US(Key)2JIS(OS), JIS2US,
// Cursor Mode: No, Ctrl + Emacs (the touch key cancellation), Ctrl + the touch key + Emacs, vi(with the touch key),

// Key override functions
void init_overrides(void) {
    key_overrides[0] = NULL;
    n_overrides = 0;
}

int8_t add_overrides(const key_override_t **add) {
    int8_t j;
    for (j = 0; n_overrides < MAX_KEY_OVERRIDES - 1; n_overrides++, j++) {
        key_overrides[n_overrides] = add[j];
        if (key_overrides[n_overrides] == NULL) {
            return(n_overrides);
        }
    }
    key_overrides[n_overrides] = NULL; // n_overrides == MAX_KEY_OVERRIDES - 1
    return(n_overrides);
}

int8_t set_overrides(const key_override_t **ko1, const key_override_t **ko2, const key_override_t **ko3) {
    // ko1: basic (for layout) overrides, ko2: basic 2, ko3: cursor overrides
    // The cursor overrides are enabled or disabled.
    init_overrides();
    add_overrides(ko1);
    pos_dummy_override = add_overrides(ko2);
    n_overrides++; // cursor overrides are disabled.
    add_overrides(ko3);
    return(n_overrides);
}

int8_t cursor_overrides_on(void) {
    key_overrides[pos_dummy_override] =&dummy_override;
    return(true);
}

int8_t cursor_overrides_off(void) {
    key_overrides[pos_dummy_override] = NULL;
    return(true);
}
