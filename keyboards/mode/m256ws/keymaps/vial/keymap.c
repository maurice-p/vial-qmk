/* Copyright 2022 Gondolindrim
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "timer.h"


enum my_keycodes {
    LOL_01 = QK_KB_0,
    MOVE,
    CAPSLOCK
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LOL_01:
      if (record->event.pressed) {
        register_code(KC_B);
        register_code(KC_F11);
        //_delay_ms(20);
        register_mods(MOD_MASK_SHIFT); 
      } else {
        unregister_mods(MOD_MASK_SHIFT);
        unregister_code(KC_B);
        unregister_code(KC_F11);
      }
      return false; // Skip all further processing of this key
    case MOVE:
      if (record->event.pressed) {
        register_mods(MOD_MASK_CG);
      } else {
        unregister_mods(MOD_MASK_CG);
      }
      return true; // Skip all further processing of this key
    case CAPSLOCK:
      if (record->event.pressed) {
        if (get_mods() == MOD_MASK_CG) {
          unregister_mods(MOD_MASK_CG);
          register_mods(MOD_MASK_ALT);
        }
        else {
          layer_on(1);
        }
      } else {
        if (get_mods() == MOD_MASK_ALT) {
          unregister_mods(MOD_MASK_ALT);
          register_mods(MOD_MASK_CG);
        }
        layer_off(1);
      }
      return true; // Skip all further processing of this key
    default:
      return true; // Process all other keycodes normally
  }
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT_all( /* Base */
    KC_ESC , KC_1   , KC_2   , KC_3   , KC_4  , KC_5    , KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_MINS, KC_EQL , KC_BSPC, KC_BSPC,
    KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R  , KC_T    , KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL ,
    KC_A   , KC_S   , KC_D   , KC_F   , KC_G  , KC_H    , KC_J   , KC_K   , KC_L   , KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT , KC_PGDN,
    KC_LSFT, KC_NUBS, KC_Z   , KC_X   , KC_C  , KC_V    , KC_B   , KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT, KC_UP  , KC_END ,
    KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC ,                   KC_RALT, MO(1)  , KC_LEFT, KC_DOWN, KC_RGHT, KC_PGUP, KC_CAPS
),
[1] = LAYOUT_all(
    QK_BOOT, KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 , KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MPLY,
    KC_TRNS, KC_TRNS, KC_TRNS,                            KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_TRNS
)
};
