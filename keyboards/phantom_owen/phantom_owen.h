/* Copyright 2017 Mike Roszkowski <mfr@rosz.net>
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
#ifndef PHANTOM_OWEN_H
#define PHANTOM_OWEN_H

#include "quantum.h"

/* 
 * Keymap for chibi Phantom -- Owen variant
 */

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define KEYMAP( \
    K0A,      K0C, K0D, K0E, K0F, K0G, K0H, K0I, K0J, K0K, K0L, K0M, K0N,      K0O, K0P, K0Q, \
    K1A, K1B, K1C, K1D, K1E, K1F, K1G, K1H, K1I, K1J, K1K, K1L, K1M, K1N,      K1O, K1P, K1Q, \
    K2A, K2B, K2C, K2D, K2E, K2F, K2G, K2H, K2I, K2J, K2K, K2L, K2M, K2N,      K2O, K2P, K2Q, \
    K3A, K3B, K3C, K3D, K3E, K3F, K3G, K3H, K3I, K3J, K3K, K3L,      K3N,                     \
    K4A, K4B, K4C, K4D, K4E, K4F, K4G, K4H, K4I, K4J, K4K,      K4M,                K4P,      \
    K5A, K5B, K5C,           K5F,                K5J, K5K, K5L,      K5N,      K5O, K5P, K5Q  \
) { \
/*          0    1      2      3      4      5     6      7      8      9      10     11     12     13     14     15     16   */  \
/* 0 */   { K0A, KC_NO, K0C,   K0D,   K0E,   K0F,  K0G,   K0H,   K0I,   K0J,   K0K,   K0L,   K0M,   K0N,   K0O,   K0P,   K0Q   }, \
/* 1 */   { K1A, K1B,   K1C,   K1D,   K1E,   K1F,  K1G,   K1H,   K1I,   K1J,   K1K,   K1L,   K1M,   K1N,   K1O,   K1P,   K1Q   }, \
/* 2 */   { K2A, K2B,   K2C,   K2D,   K2E,   K2F,  K2G,   K2H,   K2I,   K2J,   K2K,   K2L,   K2M,   K2N,   K2O,   K2P,   K2Q   }, \
/* 3 */   { K3A, K3B,   K3C,   K3D,   K3E,   K3F,  K3G,   K3H,   K3I,   K3J,   K3K,   K3L,   KC_NO, K3N,   KC_NO, KC_NO, KC_NO }, \
/* 4 */   { K4A, K4B,   K4C,   K4D,   K4E,   K4F,  K4G,   K4H,   K4I,   K4J,   K4K,   KC_NO, K4M,   KC_NO, KC_NO, K4P,   KC_NO }, \
/* 5 */   { K5A, K5B,   K5C,   KC_NO, KC_NO, K5F,  KC_NO, KC_NO, KC_NO, K5J,   K5K,   K5L,   KC_NO, K5N,   K5O,   K5P,   K5Q   }  \
}

#endif
