#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "hal.h"
#include "timer.h"
#include "wait.h"
#include "print.h"
#include "matrix.h"


/*
 * MFR's Chibi Phantom - Owen version
 * Column pins are input with internal pull-up. Row pins are output and strobe with low.
 * Key is low or 0 when activated.
 *
 *     col: { PTD0, PTD1, PTD4, PTD5, PTD6, PTD7, PTC1, PTC2 }
 *     row: { PTB2, PTB3, PTB18, PTB19, PTC0, PTC8, PTC9, PTC10, PTC11 }
 */
/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static bool debouncing = false;
static uint16_t debouncing_time = 0;

static matrix_row_t read_cols(void);

void matrix_init(void)
{
    // To use GPIO PA13-PA15 and PB3-PB4, disable JTAG by writing 0b100 to 
    // MAPR register bits 23-26
    AFIO->MAPR |= 0x04000000; // SWJ_CFG[2:0] =0b100 - JTAG & SWJ disabled
    AFIO->MAPR &= 0xFCFFFFFF;

    // Turn off the onboard LED (already configured as output by board.c)
    palClearPad(GPIOB, 1);

//debug_matrix = true;
    /* Column(sense) */
/* Column pin configuration
 * col: 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
 * pin: PB7     PB5     PB3     PA14    PB0     PB10    PA8     PB14    PB12
 * pin:     PB6     PB4     PA15    PA13    PB2     PB11    PB15    PB13
 */
    palSetPadMode(GPIOB,  7,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  6,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  5,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  4,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  3,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 15,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 14,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 13,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  0,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB,  2,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 10,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 11,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA,  8,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 15,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 14,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 13,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 12,  PAL_MODE_INPUT_PULLUP);

    /* Row(strobe) */
/* Row pin configuration
 * row: 5   4   3   2   1   0
 * pin: PA0 PA1 PA2 PA3 PA4 PA5
 */
    palSetPadMode(GPIOA, 0,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 1,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 2,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 3,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 4,  PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, 5,  PAL_MODE_OUTPUT_PUSHPULL);

/* Set rows HIGH to start */
    palSetPad(GPIOA, 0);
    palSetPad(GPIOA, 1);
    palSetPad(GPIOA, 2);
    palSetPad(GPIOA, 3);
    palSetPad(GPIOA, 4);
    palSetPad(GPIOA, 5);

    memset(matrix, 0, MATRIX_ROWS);
    memset(matrix_debouncing, 0, MATRIX_ROWS);
}

uint8_t matrix_scan(void)
{
    for (int row = 0; row < MATRIX_ROWS; row++) {
        matrix_row_t data = 0;

        // strobe row by pulling LOW
        switch (row) {
            case 0: palClearPad(GPIOA, 5); break;
            case 1: palClearPad(GPIOA, 4); break;
            case 2: palClearPad(GPIOA, 3); break;
            case 3: palClearPad(GPIOA, 2); break;
            case 4: palClearPad(GPIOA, 1); break;
            case 5: palClearPad(GPIOA, 0); break;
        }

        wait_us(20); // need wait to settle pin state

        // read col data: { PTD0, PTD1, PTD4, PTD5, PTD6, PTD7, PTC1, PTC2 }
        data = read_cols();

        // un-strobe row
        switch (row) {
            case 0: palSetPad(GPIOA, 5); break;
            case 1: palSetPad(GPIOA, 4); break;
            case 2: palSetPad(GPIOA, 3); break;
            case 3: palSetPad(GPIOA, 2); break;
            case 4: palSetPad(GPIOA, 1); break;
            case 5: palSetPad(GPIOA, 0); break;
        }

        if (matrix_debouncing[row] != data) {
            matrix_debouncing[row] = data;
            debouncing = true;
            debouncing_time = timer_read();
        }
    }

    if (debouncing && timer_elapsed(debouncing_time) > DEBOUNCE) {
        for (int row = 0; row < MATRIX_ROWS; row++) {
            matrix[row] = matrix_debouncing[row];
        }
        debouncing = false;
    }
    return 1;
}

static matrix_row_t read_cols(void) 
{
    // Port B: 7-3=0-4, 0=8, 2=9, 10=10, 11=11, 15-12=13-16
    // Port A: 15=5, 14=6, 13=7, 8=12
    return (((palReadPad(GPIOB, 7)==PAL_HIGH) ? 0 : (1<<0)) |
        ((palReadPad(GPIOB, 6)==PAL_HIGH) ? 0 : (1<<1)) |
        ((palReadPad(GPIOB, 5)==PAL_HIGH) ? 0 : (1<<2)) |
        ((palReadPad(GPIOB, 4)==PAL_HIGH) ? 0 : (1<<3)) |
        ((palReadPad(GPIOB, 3)==PAL_HIGH) ? 0 : (1<<4)) |
        ((palReadPad(GPIOA,15)==PAL_HIGH) ? 0 : (1<<5)) |
        ((palReadPad(GPIOA,14)==PAL_HIGH) ? 0 : (1<<6)) |
        ((palReadPad(GPIOA,13)==PAL_HIGH) ? 0 : (1<<7)) |
        ((palReadPad(GPIOB, 0)==PAL_HIGH) ? 0 : (1<<8)) |
        ((palReadPad(GPIOB, 2)==PAL_HIGH) ? 0 : (1<<9)) |
        ((palReadPad(GPIOB,10)==PAL_HIGH) ? 0 : (1<<10)) |
        ((palReadPad(GPIOB,11)==PAL_HIGH) ? 0 : (1<<11)) |
        ((palReadPad(GPIOA, 8)==PAL_HIGH) ? 0 : (1<<12)) |
        ((palReadPad(GPIOB,15)==PAL_HIGH) ? 0 : (1<<13)) |
        ((palReadPad(GPIOB,14)==PAL_HIGH) ? 0 : (1<<14)) |
        ((palReadPad(GPIOB,13)==PAL_HIGH) ? 0 : (1<<15)) |
        ((palReadPad(GPIOB,12)==PAL_HIGH) ? 0 : (1<<16))
        );
}

bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & (1<<col));
}

matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    xprintf("\nr/c 01234567\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%X0: ", row);
        matrix_row_t data = matrix_get_row(row);
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (data & (1<<col))
                xprintf("1");
            else
                xprintf("0");
        }
        xprintf("\n");
    }
}
