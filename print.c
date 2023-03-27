//
// Created by Vladimir Schneider on 2023-03-23.
//

#include "Arduino.h"
#include <stdio.h>
#include <math.h>
#include "print.h"

static const char nanStr[] PROGMEM = { "nan" };
static const char infStr[] PROGMEM = { "inf" };
static const char ovfStr[] PROGMEM = { "ovf" };

void sof_putch(char c) {
    fputc(c, stdout);
}

void putnc(char c, uint8_t n) {
    while (n--) {
        fputc(c, stdout);
    }
}

static void sof_puts(const char *s) {
    fputs(s, stdout);
}

static void sof_puts_P(const char *s) {
    fputs_P(s, stdout);
}

static print_funcs stdoutFuncs = {
        sof_putch,
        putnc,
        sof_puts,
        sof_puts_P
};

void put_dig(uint8_t dig) {
    fp_put_dig(&stdoutFuncs, dig);
}

void fputs_lpad(const char *str, char ch, uint8_t pad) {
    fp_fputs_lpad(&stdoutFuncs, str, ch, pad);
}

void fputs_lpad_P(PGM_P str, char ch, uint8_t pad) {
    fp_fputs_lpad_P(&stdoutFuncs, str, ch, pad);
}

void print_uint32_lpad(uint32_t n, uint8_t radix, uint8_t pad, char ch, char insertCh) {
    fp_print_uint32_lpad(&stdoutFuncs, n, radix, pad, ch, insertCh);
}

void print_int32_lpad(int32_t n, uint8_t radix, uint8_t pad, char ch, char insertChPos) {
    fp_print_int32_lpad(&stdoutFuncs, n, radix, pad, ch, insertChPos);
}

void print_float(double number, uint8_t decimals) {
    fp_print_float(&stdoutFuncs, number, decimals);
}

void fp_put_dig(print_funcs *pf, uint8_t dig) {
    dig &= 0x0f;
    char c = (char) (dig > 9 ? 'A' - 10 + dig : '0' + dig);
    pf->pPutch(c);
}

void fp_fputs_lpad(print_funcs *pf, const char *str, char ch, uint8_t pad) {
    if (pad) {
        uint8_t len = strlen(str);
        if (pad > len) {
            pf->pPutnc(ch, pad - len);
        }
    }
    pf->pFputs(str);
}

void fp_fputs_lpad_P(print_funcs *pf, PGM_P str, char ch, uint8_t pad) {
    if (pad) {
        uint8_t len = strlen_P(str);
        if (pad > len) {
            pf->pPutnc(ch, pad - len);
        }
    }
    pf->pFputs_P(str);
}

void fp_print_uint32_lpad(print_funcs *pf, uint32_t n, uint8_t radix, uint8_t pad, char ch, char insertCh) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8 bits for digits chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];
    *str = '\0';

    if (radix < 2) radix = 10;

    do {
        uint8_t c = n % radix;
        n /= radix;

        *--str = (char) (c < 10 ? c + '0' : c + 'A' - 10);
    } while (n);

    uint8_t len = &buf[sizeof(buf) - 1] - str;

    if (pad > len) {
        // left pad with spaces or given chars
        pf->pPutnc(ch ? ch : ' ', pad - len - (insertCh ? 1 : 0));
    }

    if (insertCh) pf->pPutch(insertCh);
    pf->pFputs(str);
}

void fp_print_int32_lpad(print_funcs *pf, int32_t n, uint8_t radix, uint8_t pad, char ch, char insertChPos) {
    if (radix < 2 || radix == 10) {
        char insCh = 0;
        if (n < 0) {
            if (ch == 0) {
                ch = ' ';
            }

            insCh = '-';
            if (!pad || ch == ' ') {
                pf->pPutch('-');
                if (pad) pad--;
                insCh = 0;
            }
            n = -n;
        }
        fp_print_uint32_lpad(pf, n, radix, pad, ch, insCh);
    } else {
        fp_print_uint32_lpad(pf, n, radix, pad, ch, insertChPos);
    }
}

void fp_print_float(print_funcs *pf, double number, uint8_t decimals) {
    if (isnan(number)) pf->pFputs_P(nanStr);
    else if (isinf(number)) pf->pFputs_P(infStr);
    else if (number > 4294967040.0 || number < -4294967040.0) pf->pFputs_P(ovfStr);  // constant determined empirically
    else {
        // Handle negative numbers
        if (number < 0.0) {
            pf->pPutch('-');
            number = -number;
        }

        // Round correctly so that fprint_float(1.999, 2) prints as "2.00"
        double rounding = 0.5;
        for (uint8_t i = 0; i < decimals; ++i)
            rounding /= 10.0;

        number += rounding;

        // Extract the integer part of the number and ssd1306_print it
        unsigned long int_part = (unsigned long) number;
        double remainder = number - (double) int_part;
        fp_print_uint32_lpad(pf, int_part, 10, 0, 0, 0);

        // Print the decimal point, but only if there are digits beyond
        if (decimals > 0) {
            pf->pPutch('.');
        }

        // Extract digits from the remainder one at a time
        while (decimals-- > 0) {
            remainder *= 10.0;
            uint8_t dig = (uint8_t) (remainder);
            char c = (char) (dig > 9 ? 'A' - 10 + dig : '0' + dig);
            pf->pPutch(c);
            remainder -= dig;
        }
    }
}
