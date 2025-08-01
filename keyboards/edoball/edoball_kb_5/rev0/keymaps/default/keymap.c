#include QMK_KEYBOARD_H

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
    [0] = LAYOUT_ortho_16x5(
        KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PSLS, KC_P7,   KC_P8,   KC_P9,   KC_PSLS,
        KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PAST, KC_P4,   KC_P5,   KC_P6,   KC_PAST, 
        KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, KC_P1,   KC_P2,   KC_P3,   KC_PMNS, 
        KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS,
        KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS, KC_P0,   KC_PDOT, KC_PENT, KC_PPLS
    )
};
