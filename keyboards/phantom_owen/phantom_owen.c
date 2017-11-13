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
#include "phantom_owen.h"

void matrix_init_kb(void) {
    // put your keyboard start-up code here
    // runs once when the firmware starts up

    // To use GPIO PA13-PA15 and PB3-PB4, disable JTAG by writing 0b100 to 
    // MAPR register bits 23-26
    AFIO->MAPR |= 0x04000000; // SWJ_CFG[2:0] =0b100 - JTAG & SWJ disabled
    AFIO->MAPR &= 0xFCFFFFFF;

    // MFR: debug on
    debug_config.enable = true;
    debug_config.keyboard = true;
    debug_config.matrix = true;

    // Turn off the onboard LED (already configured as output by board.c)
    palClearPad(GPIOB, 1);

    led_init_ports();
    matrix_init_user();
}

void matrix_scan_kb(void) {
    // put your looping keyboard code here
    // runs every cycle (a lot)

    matrix_scan_user();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    // put your per-action keyboard code here
    // runs for every action, just before processing by the firmware

    return process_record_user(keycode, record);
}

