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
#include "keymap_german.h"
//#include "timer.h"
//#include "wait.h"

enum layers {
    L0_BASE,
    L1_GAME,
    L2_SYM,
    L3_NAV
};

enum my_keycodes {
    LOL_01 = QK_KB_0,
    MOVE,
    CAPSLOCK,
    GAME_TOG
};

static uint32_t rng_state = 0xA3C59AC3u;


// mix entropy into the rng_state using a simple hash function
static void rng_add_entropy(uint32_t value) {

    rng_state ^= value
               + 0x9E3779B9u
               + (rng_state << 6)
               + (rng_state >> 2);

    // Xorshift32
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;

    // Xorshift darf nicht im Zustand 0 hängen bleiben
    if (rng_state == 0) {
        rng_state = 0xA3C59AC3u;
    }
}


// create 32-bit-pseudo number
static uint32_t rng32(void) {

    uint32_t x = rng_state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    if (x == 0) {
        x = 0xA3C59AC3u;
    }

    rng_state = x;

    return x;
}


/* random range between min and max */
static uint16_t random_range(uint16_t min, uint16_t max) {

    if (max <= min) {
        return min;
    }

    uint32_t range = (uint32_t)max - min + 1;

    // Verhindert Modulo-Bias
    uint32_t threshold = (0u - range) % range;

    uint32_t r;

    do {
        r = rng32();
    } while (r < threshold);

    return min + (r % range);
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  /* collect entropy from every keypress */
  if (record->event.pressed) {

      uint32_t entropy =
          timer_read32()
          ^ ((uint32_t)keycode << 16)
          ^ ((uint32_t)record->event.key.row << 8)
          ^ ((uint32_t)record->event.key.col);

      rng_add_entropy(entropy);
  }

  switch (keycode) {
    case LOL_01:
      if (record->event.pressed) {
        register_code(KC_F10); // Attack only champions
        wait_ms(random_range(5, 20));
        register_code(KC_F11); // Advance player stats for circle
        wait_ms(random_range(5, 20));
        register_code(KC_LSFT);
      } else {
        unregister_code(KC_LSFT);
        wait_ms(random_range(5, 20));
        unregister_code(KC_F10); // Attack only champions
        wait_ms(random_range(5, 20));
        unregister_code(KC_F11); // Advance player stats for circle
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
          layer_on(L2_SYM);
        }
      } else {
        if (get_mods() == MOD_MASK_ALT) {
          unregister_mods(MOD_MASK_ALT);
          register_mods(MOD_MASK_CG);
        }
        layer_off(L2_SYM);
      }
      return true; // Skip all further processing of this key
    case GAME_TOG:
      if (record->event.pressed) {
        if (get_highest_layer(default_layer_state) == L1_GAME) {
          set_single_default_layer(L0_BASE);
        }
        else {
          set_single_default_layer(L1_GAME);
        }
      }
      return false; // Skip all further processing of this key
    default:
      return true; // Process all other keycodes normally
  }
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [L0_BASE] = LAYOUT_65_iso_blocker(
        KC_ESC,              DE_1,                DE_2,                DE_3,                DE_4,                DE_5,                DE_6,                DE_7,                DE_8,                DE_9,                DE_0,                DE_SS,               DE_ACUT,             KC_BSPC,             KC_DEL,
        KC_TAB,              DE_Q,                DE_W,                DE_E,                DE_R,                DE_T,                DE_Z,                DE_U,                DE_I,                DE_O,                DE_P,                DE_UDIA,             DE_PLUS,                                  GAME_TOG,
        MO(L2_SYM),          DE_A,                DE_S,                DE_D,                DE_F,                DE_G,                DE_H,                DE_J,                DE_K,                DE_L,                DE_ODIA,             DE_ADIA,             MO(L2_SYM),          KC_ENT,              KC_HOME,
        KC_LSFT,             DE_LABK,             DE_Y,                DE_X,                DE_C,                DE_V,                DE_B,                DE_N,                DE_M,                DE_COMM,             DE_DOT,              DE_MINS,             KC_RSFT,             KC_UP,               KC_END,
        KC_LCTL,             KC_LGUI,             KC_LALT,                                                       LT(L3_NAV, KC_SPC),                                                                                      KC_LALT,             KC_RCTL,             KC_LEFT,             KC_DOWN,             KC_RGHT
    ),

    [L1_GAME] = LAYOUT_65_iso_blocker(
        KC_ESC,              DE_1,                DE_2,                DE_3,                DE_4,                DE_5,                DE_6,                DE_7,                DE_8,                DE_9,                DE_0,                KC_F11,              KC_F12,              KC_BSPC,             KC_DEL,
        KC_TAB,              DE_Q,                DE_W,                DE_E,                DE_R,                DE_T,                DE_Z,                DE_U,                DE_I,                DE_O,                DE_P,                DE_UDIA,             DE_PLUS,                                  GAME_TOG,
        DE_PLUS,             DE_A,                DE_S,                DE_D,                DE_F,                DE_G,                DE_H,                DE_J,                DE_K,                DE_L,                DE_ODIA,             DE_ADIA,             MO(L2_SYM),          KC_ENT,              KC_HOME,
        LOL_01,              DE_LABK,             DE_Y,                DE_X,                DE_C,                DE_V,                DE_B,                DE_N,                DE_M,                DE_COMM,             DE_DOT,              DE_MINS,             KC_RSFT,             KC_UP,               KC_END,
        KC_LCTL,             KC_LGUI,             KC_LALT,                                                       KC_SPC,                                                                                                  KC_LALT,             KC_RCTL,             KC_LEFT,             KC_DOWN,             KC_RGHT
    ),

    [L2_SYM] = LAYOUT_65_iso_blocker(
        KC_TRNS,             KC_F1,               KC_F2,               KC_F3,               KC_F4,               KC_F5,               KC_F6,               KC_F7,               KC_F8,               KC_F9,               KC_F10,              KC_F11,              KC_F12,              KC_TRNS,             KC_TRNS,
        KC_TRNS,             KC_TRNS,             DE_UNDS,             DE_LBRC,             DE_RBRC,             DE_CIRC,             DE_EXLM,             DE_LABK,             DE_RABK,             DE_EQL,              DE_AMPR,             KC_TRNS,             KC_TRNS,                                  KC_TRNS,
        KC_TRNS,             DE_BSLS,             DE_SLSH,             DE_LCBR,             DE_RCBR,             DE_ASTR,             DE_QUES,             DE_LPRN,             DE_RPRN,             DE_MINS,             DE_COLN,             DE_AT,               KC_TRNS,             KC_TRNS,             KC_TRNS,
        KC_TRNS,             KC_TRNS,             DE_HASH,             DE_DLR,              DE_PIPE,             DE_TILD,             DE_GRV,              DE_PLUS,             DE_PERC,             DE_DQUO,             DE_QUOT,             DE_SCLN,             KC_TRNS,             KC_TRNS,             KC_TRNS, 
        KC_TRNS,             KC_TRNS,             KC_TRNS,                                                       KC_TRNS,                                                                                                 KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS
    ),

    [L3_NAV] = LAYOUT_65_iso_blocker(
        KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,
        KC_TRNS,             KC_TRNS,             LCTL(KC_LEFT),       KC_UP,               LCTL(KC_RIGHT),      KC_TRNS,             KC_TRNS,             KC_KP_7,             KC_KP_8,             KC_KP_9,             KC_TRNS,             KC_TRNS,             KC_TRNS,                                  KC_TRNS,
        KC_TRNS,             KC_TRNS,             KC_LEFT,             KC_DOWN,             KC_RIGHT,            KC_TRNS,             KC_TRNS,             KC_KP_4,             KC_KP_5,             KC_KP_6,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,
        KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_KP_1,             KC_KP_2,             KC_KP_3,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,
        KC_TRNS,             KC_TRNS,             KC_TRNS,                                                       KC_KP_0,                                                                                                 KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS,             KC_TRNS
    ),



  };