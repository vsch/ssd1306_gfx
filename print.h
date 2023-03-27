//
// Created by Vladimir Schneider on 2023-03-23.
//

#ifndef COOKIEPORTIONER_PRINT_H
#define COOKIEPORTIONER_PRINT_H

#include <stdint.h>
#include <stdio.h>
#include "ssd1306_display.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct print_funcs {
    void (*pPutch)(char c);
    void (*pPutnc)(char c, uint8_t n);
    void (*pFputs)(const char *s);
    void (*pFputs_P)(const char *s);
} print_funcs;

extern void putnc(char c, uint8_t count);
extern void put_dig(uint8_t dig);
extern void fputs_lpad(const char *str, char ch, uint8_t pad);
extern void fputs_lpad_P(PGM_P str, char ch, uint8_t pad);
extern void print_uint32_lpad(uint32_t n, uint8_t radix, uint8_t pad, char ch, char insertCh);
extern void print_int32_lpad(int32_t n, uint8_t radix, uint8_t pad, char ch, char insertChPos);
extern void print_float(double number, uint8_t decimals);

extern void fp_put_dig(print_funcs *pf, uint8_t dig);
extern void fp_fputs_lpad(print_funcs *pf, const char *str, char ch, uint8_t pad);
extern void fp_fputs_lpad_P(print_funcs *pf, PGM_P str, char ch, uint8_t pad);
extern void fp_print_uint32_lpad(print_funcs *pf, uint32_t n, uint8_t radix, uint8_t pad, char ch, char insertCh);
extern void fp_puts_lpad(print_funcs *funcs, const char *str, char ch, uint8_t pad);
extern void fp_puts_lpad_P(print_funcs *funcs, PGM_P str, char ch, uint8_t pad);
extern void fp_print_uint32_lpad(print_funcs *funcs, uint32_t n, uint8_t radix, uint8_t pad, char ch, char insertCh);
extern void fp_print_int32_lpad(print_funcs *funcs, int32_t n, uint8_t radix, uint8_t pad, char ch, char insertChPos);
extern void fp_print_float(print_funcs *funcs, double number, uint8_t decimals);

#ifdef __cplusplus
};
#endif

#endif //COOKIEPORTIONER_PRINT_H
