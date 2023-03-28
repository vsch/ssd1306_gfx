//
// Created by Vladimir Schneider on 2023-03-23.
//

#include <stdint.h>

#ifndef CONSOLE_DEBUG

#include <avr/pgmspace.h>

#endif

#include <stdio.h>
#include "Arduino.h"
#include "print.h"
#include "ssd1306_gfx.h"

#ifndef CONSOLE_DEBUG

/** Stream function wrapper. */
static int __gfx_putc(char c, FILE *stream) {
    (void) stream;   //suppress unused warning
    gfx_putc(c);
    return 0;
}

//stream setups
//https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html
FILE gfx_out = FDEV_SETUP_STREAM(__gfx_putc, NULL, _FDEV_SETUP_WRITE);

#else

int __ssd1306_putc(void *ignored, const char *s, int n) {
    int i = n;
    while (i-- > 0) {
        gfx_putc(*s++);
    }
    return 0;
}

FILE gfx_out;

#endif

void gfx_putnc(char c, uint8_t n) {
    while (n--) {
        gfx_putc(c);
    }
}

void gfx_fputs(const char *s) {
    while (*s) {
        gfx_putc(*s++);
    }
}

void gfx_fputs_P(const char *s) {
    for (;;) {
        char c = pgm_read_byte(s++);
        if (!c) break;
        gfx_putc(c);
    }
}

static print_funcs dispFuncs = {
        gfx_putc,
        gfx_putnc,
        gfx_fputs,
        gfx_fputs_P
};

void gfx_put_dig(uint8_t dig) {
    fp_put_dig(&dispFuncs, dig);
}

void gfx_fputs_lpad(const char *str, char ch, uint8_t pad) {
    fp_fputs_lpad(&dispFuncs, str, ch, pad);
}

void gfx_fputs_lpad_P(PGM_P str, char ch, uint8_t pad) {
    fp_fputs_lpad_P(&dispFuncs, str, ch, pad);
}

void gfx_print_float(double number, uint8_t digits) {
    fp_print_float(&dispFuncs, number, digits);
}

void gfx_print_int32_lpad(int32_t n, uint8_t radix, uint8_t pad, char ch) {
    fp_print_int32_lpad(&dispFuncs, n, radix, pad, ch, 0);
}

// writes integer i at current cursor startPosition
void gfx_print_int32(int32_t i) {
    fp_print_int32_lpad(&dispFuncs, i, 0, 0, 0, 0);
}

void gfx_print_uint32_lpad(uint32_t i, uint8_t radix, uint8_t pad, char ch) {
    fp_print_uint32_lpad(&dispFuncs, i, radix, pad, ch, 0);
}

void gfx_print_uint32(uint32_t i) {
    fp_print_uint32_lpad(&dispFuncs, i, 10, 0, 0, 0);
}

void gfx_print_int16_lpad(int16_t i, uint8_t radix, uint8_t pad, char ch) {
    fp_print_int32_lpad(&dispFuncs, i, radix, pad, ch, 0);
}

void gfx_print_int16(int16_t i) {
    fp_print_int32_lpad(&dispFuncs, i, 10, 0, 0, 0);
}

void gfx_print_uint16(uint16_t i) {
    fp_print_uint32_lpad(&dispFuncs, i, 10, 0, 0, 0);
}

void gfx_print_uint16_lpad(uint16_t i, uint8_t radix, uint8_t pad, char ch) {
    fp_print_uint32_lpad(&dispFuncs, i, radix, pad, ch, 0);
}

void gfx_print_int8_lpad(int8_t i, uint8_t radix, uint8_t pad, char ch) {
    fp_print_int32_lpad(&dispFuncs, i, radix, pad, ch, 0);
}

void gfx_print_int8(int8_t i) {
    fp_print_int32_lpad(&dispFuncs, i, 10, 0, 0, 0);
}

void gfx_print_uint8(uint8_t i) {
    fp_print_uint32_lpad(&dispFuncs, i, 10, 0, 0, 0);
}

void gfx_print_uint8_lpad(uint8_t i, uint8_t radix, uint8_t pad, char ch) {
    fp_print_uint32_lpad(&dispFuncs, i, radix, pad, ch, 0);
}

