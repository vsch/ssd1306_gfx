#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "ssd1306_display.h"
#include "Arduino.h"

#include "ssd1306_gfx.h"

// Based on code from:
//
// TFT: Experiments interfacing ATmega328 to an SSD1306 1.8" LCD TFT display
// Author : Bruce E. Hall <bhall66@gmail.com>
// Website : w8bh.net
// Version : 1.0
// Date : 04 May 2014
// Target : ATmega328P microcontroller
// Language : C, using AVR studio 6
// Size : 3622 bytes

// ---------------------------------------------------------------------------
// GLOBAL VARIABLES
const char spcString[] PROGMEM = { ' ', '\0' };

const uint8_t FONT_CHARS[96][SSD1306_FONT_X_PIXELS] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, // 0x20 - (space)
        0x00, 0x00, 0x5F, 0x00, 0x00, // 0x21 - !
        0x00, 0x07, 0x00, 0x07, 0x00, // 0x22 - "
        0x14, 0x7F, 0x14, 0x7F, 0x14, // 0x23 - #
        0x24, 0x2A, 0x7F, 0x2A, 0x12, // 0x24 - $
        0x23, 0x13, 0x08, 0x64, 0x62, // 0x25 - %
        0x36, 0x49, 0x55, 0x22, 0x50, // 0x26 - &
        0x00, 0x05, 0x03, 0x00, 0x00, // 0x27 - '
        0x00, 0x1C, 0x22, 0x41, 0x00, // 0x28 - (
        0x00, 0x41, 0x22, 0x1C, 0x00, // 0x29 - )
        0x08, 0x2A, 0x1C, 0x2A, 0x08, // 0x2A - *
        0x08, 0x08, 0x3E, 0x08, 0x08, // 0x2B - +
        0x00, 0x50, 0x30, 0x00, 0x00, // 0x2C - ,
        0x08, 0x08, 0x08, 0x08, 0x08, // 0x2D - -
        0x00, 0x60, 0x60, 0x00, 0x00, // 0x2E - .
        0x20, 0x10, 0x08, 0x04, 0x02, // 0x2F - /
        0x3E, 0x51, 0x49, 0x45, 0x3E, // 0x30 - 0
        0x00, 0x42, 0x7F, 0x40, 0x00, // 0x31 - 1
        0x42, 0x61, 0x51, 0x49, 0x46, // 0x32 - 2
        0x21, 0x41, 0x45, 0x4B, 0x31, // 0x33 - 3
        0x18, 0x14, 0x12, 0x7F, 0x10, // 0x34 - 4
        0x27, 0x45, 0x45, 0x45, 0x39, // 0x35 - 5
        0x3C, 0x4A, 0x49, 0x49, 0x30, // 0x36 - 6
        0x01, 0x71, 0x09, 0x05, 0x03, // 0x37 - 7
        0x36, 0x49, 0x49, 0x49, 0x36, // 0x38 - 8
        0x06, 0x49, 0x49, 0x29, 0x1E, // 0x39 - 9
        0x00, 0x36, 0x36, 0x00, 0x00, // 0x3A - :
        0x00, 0x56, 0x36, 0x00, 0x00, // 0x3B - ;
        0x00, 0x08, 0x14, 0x22, 0x41, // 0x3C - <
        0x14, 0x14, 0x14, 0x14, 0x14, // 0x3D - =
        0x41, 0x22, 0x14, 0x08, 0x00, // 0x3E - >
        0x02, 0x01, 0x51, 0x09, 0x06, // 0x3F - ?
        0x32, 0x49, 0x79, 0x41, 0x3E, // 0x40 - @
        0x7E, 0x11, 0x11, 0x11, 0x7E, // 0x41 - A
        0x7F, 0x49, 0x49, 0x49, 0x36, // 0x42 - B
        0x3E, 0x41, 0x41, 0x41, 0x22, // 0x43 - C
        0x7F, 0x41, 0x41, 0x22, 0x1C, // 0x44 - D
        0x7F, 0x49, 0x49, 0x49, 0x41, // 0x45 - E
        0x7F, 0x09, 0x09, 0x01, 0x01, // 0x46 - F
        0x3E, 0x41, 0x41, 0x51, 0x32, // 0x47 - G
        0x7F, 0x08, 0x08, 0x08, 0x7F, // 0x48 - H
        0x00, 0x41, 0x7F, 0x41, 0x00, // 0x49 - I
        0x20, 0x40, 0x41, 0x3F, 0x01, // 0x4A - J
        0x7F, 0x08, 0x14, 0x22, 0x41, // 0x4B - K
        0x7F, 0x40, 0x40, 0x40, 0x40, // 0x4C - L
        0x7F, 0x02, 0x04, 0x02, 0x7F, // 0x4D - M
        0x7F, 0x04, 0x08, 0x10, 0x7F, // 0x4E - N
        0x3E, 0x41, 0x41, 0x41, 0x3E, // 0x4F - O
        0x7F, 0x09, 0x09, 0x09, 0x06, // 0x50 - P
        0x3E, 0x41, 0x51, 0x21, 0x5E, // 0x51 - Q
        0x7F, 0x09, 0x19, 0x29, 0x46, // 0x52 - R
        0x46, 0x49, 0x49, 0x49, 0x31, // 0x53 - S
        0x01, 0x01, 0x7F, 0x01, 0x01, // 0x54 - T
        0x3F, 0x40, 0x40, 0x40, 0x3F, // 0x55 - U
        0x1F, 0x20, 0x40, 0x20, 0x1F, // 0x56 - V
        0x7F, 0x20, 0x18, 0x20, 0x7F, // 0x57 - W
        0x63, 0x14, 0x08, 0x14, 0x63, // 0x58 - X
        0x03, 0x04, 0x78, 0x04, 0x03, // 0x59 - Y
        0x61, 0x51, 0x49, 0x45, 0x43, // 0x5A - Z
        0x00, 0x00, 0x7F, 0x41, 0x41, // 0x5B - [
        0x02, 0x04, 0x08, 0x10, 0x20, // 0x5C - "\"
        0x41, 0x41, 0x7F, 0x00, 0x00, // 0x5D - ]
        0x04, 0x02, 0x01, 0x02, 0x04, // 0x5E - ^
        0x40, 0x40, 0x40, 0x40, 0x40, // 0x5F - _
        0x00, 0x01, 0x02, 0x04, 0x00, // 0x60 - `
        0x20, 0x54, 0x54, 0x54, 0x78, // 0x61 - a
        0x7F, 0x48, 0x44, 0x44, 0x38, // 0x62 - b
        0x38, 0x44, 0x44, 0x44, 0x20, // 0x63 - c
        0x38, 0x44, 0x44, 0x48, 0x7F, // 0x64 - d
        0x38, 0x54, 0x54, 0x54, 0x18, // 0x65 - e
        0x08, 0x7E, 0x09, 0x01, 0x02, // 0x66 - f
        0x08, 0x14, 0x54, 0x54, 0x3C, // 0x67 - g
        0x7F, 0x08, 0x04, 0x04, 0x78, // 0x68 - h
        0x00, 0x44, 0x7D, 0x40, 0x00, // 0x69 - i
        0x20, 0x40, 0x44, 0x3D, 0x00, // 0x6A - j
        0x00, 0x7F, 0x10, 0x28, 0x44, // 0x6B - k
        0x00, 0x41, 0x7F, 0x40, 0x00, // 0x6C - l
        0x7C, 0x04, 0x18, 0x04, 0x78, // 0x6D - m
        0x7C, 0x08, 0x04, 0x04, 0x78, // 0x6E - n
        0x38, 0x44, 0x44, 0x44, 0x38, // 0x6F - o
        0x7C, 0x14, 0x14, 0x14, 0x08, // 0x70 - p
        0x08, 0x14, 0x14, 0x18, 0x7C, // 0x71 - q
        0x7C, 0x08, 0x04, 0x04, 0x08, // 0x72 - r
        0x48, 0x54, 0x54, 0x54, 0x20, // 0x73 - s
        0x04, 0x3F, 0x44, 0x40, 0x20, // 0x74 - t
        0x3C, 0x40, 0x40, 0x20, 0x7C, // 0x75 - u
        0x1C, 0x20, 0x40, 0x20, 0x1C, // 0x76 - v
        0x3C, 0x40, 0x30, 0x40, 0x3C, // 0x77 - w
        0x44, 0x28, 0x10, 0x28, 0x44, // 0x78 - x
        0x0C, 0x50, 0x50, 0x50, 0x3C, // 0x79 - y
        0x44, 0x64, 0x54, 0x4C, 0x44, // 0x7A - z
        0x00, 0x08, 0x36, 0x41, 0x00, // 0x7B - {
        0x00, 0x00, 0x7F, 0x00, 0x00, // 0x7C - |
        0x00, 0x41, 0x36, 0x08, 0x00, // 0x7D - }
        0x08, 0x08, 0x2A, 0x1C, 0x08, // 0x7E - ->
        0x08, 0x1C, 0x2A, 0x08, 0x08, // 0x7F - <-
};

#ifdef SSD1306_SMALL_FONT
const uint8_t SMALL_CHARS[17][SSD1306_SMALL_X_PIXELS * SSD1306_SMALL_Y_ROWS] PROGMEM = {
        0x00, 0x00, 0x00, // 0x20 - ' '
        0x19, 0x04, 0x13, // 0x25 - '%'
        0x04, 0x0E, 0x04, // 0x2b - '+'
        0x08, 0x18, 0x00, // 0x2c - ','
        0x04, 0x04, 0x04, // 0x2d - '-'
        0x18, 0x18, 0x00, // 0x2e - '.'
        0x18, 0x04, 0x03, // 0x2f - '/'
        0x1F, 0x11, 0x1F, // 0x30 - '0'
        0x12, 0x1F, 0x10, // 0x31 - '1'
        0x1D, 0x15, 0x17, // 0x32 - '2'
        0x15, 0x15, 0x1F, // 0x33 - '3'
        0x0C, 0x0A, 0x1F, // 0x34 - '4'
        0x17, 0x15, 0x1D, // 0x35 - '5'
        0x1E, 0x15, 0x1D, // 0x36 - '6'
        0x01, 0x1D, 0x03, // 0x37 - '7'
        0x1F, 0x15, 0x1F, // 0x38 - '8'
        0x17, 0x15, 0x0F, // 0x39 - '9'
};

const char SMALL_CHAR_MAP[] PROGMEM = { " %+,-./0123456789" };

const uint8_t SMALL_CHAR_SET[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00,
        0x21, 0xf8, 0xff, 0x03,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
};
#endif // SSD1306_SMALL_FONT

#ifdef SSD1306_LARGE_FONT
const uint8_t LARGE_CHARS[13][SSD1306_LARGE_X_PIXELS * SSD1306_LARGE_Y_ROWS] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x2e - '.'
        0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00, 0x00,
        0xFC, 0xFE, 0x03, 0x03, 0xC3, 0xE3, 0x33, 0x1B, 0xFE, 0xFC, // 0x30 - '0'
        0x0F, 0x1F, 0x36, 0x33, 0x31, 0x30, 0x30, 0x30, 0x1F, 0x0F,
        0x00, 0x00, 0x0C, 0x0C, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, // 0x31 - '1'
        0x00, 0x00, 0x30, 0x30, 0x3F, 0x3F, 0x30, 0x30, 0x00, 0x00,
        0x0C, 0x0E, 0x03, 0x03, 0x03, 0x83, 0x83, 0x43, 0x7E, 0x3C, // 0x32 - '2'
        0x30, 0x38, 0x3C, 0x36, 0x33, 0x31, 0x30, 0x30, 0x30, 0x30,
        0x03, 0x03, 0x03, 0x23, 0x33, 0x33, 0x7B, 0xCF, 0x87, 0x03, // 0x33 - '3'
        0x0C, 0x1C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1F, 0x0F,
        0xC0, 0xE0, 0x30, 0x18, 0x0C, 0x0E, 0xFF, 0xFF, 0x00, 0x00, // 0x34 - '4'
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3F, 0x3F, 0x03, 0x03,
        0x3F, 0x3F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xE3, 0xC3, // 0x35 - '5'
        0x0C, 0x1C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1F, 0x0F,
        0xF0, 0xF8, 0x8C, 0xC6, 0xC2, 0xC3, 0xC1, 0xC1, 0x80, 0x00, // 0x36 - '6'
        0x0F, 0x1F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1F, 0x0F,
        0x03, 0x03, 0x03, 0x83, 0xC3, 0x63, 0x33, 0x1B, 0x0F, 0x07, // 0x37 - '7'
        0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x1C, 0xBE, 0xE3, 0xC3, 0xC3, 0xC3, 0xC3, 0xE3, 0xBE, 0x1C, // 0x38 - '8'
        0x0F, 0x1F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1F, 0x0F,
        0x3C, 0x7E, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x43, 0xFE, 0xFC, // 0x39 - '9'
        0x00, 0x00, 0x20, 0x20, 0x30, 0x10, 0x18, 0x0C, 0x07, 0x03,
        0x00, 0x00, 0x01, 0x03, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, // 0x6c - 'l'
        0x00, 0x00, 0x30, 0x30, 0x3F, 0x3F, 0x30, 0x30, 0x00, 0x00,
        0xF0, 0xF0, 0x30, 0x20, 0xC0, 0xC0, 0x20, 0x30, 0xF0, 0xE0, // 0x6d - 'm'
        0x3F, 0x3F, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00, 0x3F, 0x3F,
};

const char LARGE_CHAR_MAP[] PROGMEM = { ".0123456789lm" };

const uint8_t LARGE_CHAR_SET[] PROGMEM = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x40, 0xff, 0x03,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
};
#endif // SSD1306_LARGE_FONT

uint8_t ssd1306_flags;              // option flags
uint8_t ssd1306_text_flags;              // option flags
int8_t ssd1306_char_x_size;              // updated when text size flags change
int8_t ssd1306_char_y_size;              // updated when text size flags change

coord_x ssd1306_cursor_x;                 // cursor
coord_x ssd1306_max_x;                // max X value when printing a string and wrapping
coord_x ssd1306_min_x;                // max X value when printing a string and wrapping
coord_x ssd1306_margin_left;              // left margin for text wrapping
coord_x ssd1306_margin_right;             // right margin for text wrapping
coord_y ssd1306_cursor_y;                  // cursor
coord_y ssd1306_max_y;                // max Y value when printing a string and wrapping
coord_y ssd1306_min_y;                // max Y value when printing a string and wrapping

color_t ssd1306_fore_color;
color_t ssd1306_gap_color;
color_t ssd1306_back_color;

uint8_t ssd1306_dash_bits;            // solid/dash/dot pattern for line outlines (not text)
uint8_t ssd1306_dash_size;            // solid/dash/dot pattern for line outlines (not text)
uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

uint8_t ssd1306_wrap_buff_pos; // next character in buffer
char ssd1306_wrap_buff[SSD1306_WRAP_BUFFER_SIZE]; // characters buffered when looking for space on which to possibly break

#ifdef SERIAL_DEBUG_GFX_STATS
uint32_t ssd1306_put_ch_total;
uint16_t ssd1306_put_ch_count;
uint32_t ssd1306_put_ch_lp_total;
uint32_t ssd1306_put_ch_lp_count;
uint32_t ssd1306_putc_total;
uint16_t ssd1306_putc_count;
uint32_t ssd1306_put_pixel_total;
uint32_t ssd1306_put_pixel_count;
#endif

void ssd1306_clear_screen() {
    ssd1306_flags = 0;
    ssd1306_text_flags = 0;
    ssd1306_char_x_size = CHAR_WIDTH;    // updated when text size flags change
    ssd1306_char_y_size = CHAR_HEIGHT;   // updated when text size flags change
    ssd1306_fore_color = SSD1306_COLOR_WHITE;
    ssd1306_back_color = SSD1306_COLOR_NONE;
    ssd1306_gap_color = SSD1306_COLOR_NONE;
    ssd1306_dash_bits = SSD1306_BITS_DASH_NONE;
    ssd1306_dash_size = SSD1306_SIZE_DASH_NONE;
    ssd1306_cursor_x = 0;
    ssd1306_cursor_y = 0;
    ssd1306_margin_left = 0;
    ssd1306_margin_right = DISPLAY_XSIZE;
    ssd1306_wrap_buff_pos = 0;
    ssd1306_max_x = 0;
    ssd1306_min_x = 0;
    ssd1306_max_y = 0;
    ssd1306_min_y = 0;
    ssd1306_dashOffset = 0;
    memset(ssd1306_display_buffer, 0, sizeof(ssd1306_display_buffer));

#ifdef SERIAL_DEBUG_GFX_STATS
    ssd1306_put_ch_total = 0;
    ssd1306_put_ch_count = 0;
    ssd1306_putc_total = 0;
    ssd1306_putc_count = 0;
    ssd1306_put_ch_lp_total = 0;
    ssd1306_put_ch_lp_count = 0;
    ssd1306_put_pixel_total = 0;
    ssd1306_put_pixel_count = 0;

#endif
}

// ---------------------------------------------------------------------------
// SIMPLE GRAPHICS ROUTINES
//
// note: many routines have uint8_t parameters, to save space,
// but these can easily be changed to int params for larger displays.

// draw pixel in given color
void ssd1306_set_pixel(uint8_t x, coord_y y, color_t color) {
    // HACK: Only works if sizes are powers of two
    if (color != SSD1306_COLOR_NONE && COORDS_IN_DISPLAY(x, y)) {
        uint8_t page = y >> 3;
        uint8_t mask = (uint8_t) (1 << (y & 7));
        if (color == SSD1306_COLOR_WHITE) {
            ssd1306_display_buffer[page][x] |= mask;
        } else if (color == SSD1306_COLOR_BLACK) {
            ssd1306_display_buffer[page][x] &= ~mask;
        } else if (color == SSD1306_COLOR_INVERT) {
            ssd1306_display_buffer[page][x] ^= mask;
        }
    }
}

void ssd1306_set_line_pattern(uint16_t pattern) {
    ssd1306_dash_bits = GET_DASH_BITS(pattern);
    ssd1306_dash_size = GET_DASH_SIZE(pattern);
    ssd1306_dashOffset = 0;
}

uint16_t ssd1306_get_line_pattern() {
    return LINE_PATTERN(ssd1306_dash_bits, ssd1306_dash_size);
}

bool ssd1306_next_dash_bit() {
    if (ssd1306_dashOffset >= ssd1306_dash_size) {
        ssd1306_dashOffset = 0;
    }
    return ssd1306_dash_bits & (1 << ssd1306_dashOffset++);
}

color_t ssd1306_next_dash_color() {
    if (ssd1306_dash_bits == SSD1306_BITS_DASH_NONE && ssd1306_dash_size == SSD1306_SIZE_DASH_NONE) {
        return ssd1306_fore_color;
    } else {
        return ssd1306_next_dash_bit() ? ssd1306_fore_color : ssd1306_gap_color;
    }
}

void ssd1306_hline(coord_x x0, coord_y y0, coord_x x1, color_t color) {
    ssd1306_normalize_x(&x0, &x1);
    while (x0 <= x1) {
        ssd1306_set_pixel(x0++, y0, color);
    }
}

void ssd1306_hline_dashed(coord_x x0, coord_y y0, coord_x x1) {
    ssd1306_normalize_x(&x0, &x1);
    while (x0 <= x1) {
        ssd1306_set_pixel(x0++, y0, ssd1306_next_dash_color());
    }
}

// draws a vertical line in given color
void ssd1306_vline(coord_x x0, coord_y y0, coord_y y1, color_t color) {
    ssd1306_normalize_y(&y0, &y1);
#ifdef SSD1306_BIT_BLIT
    if (y0 < 0) y0 = 0;
    if (x0 < 0) x0 = 0;

    if (y1 > DISPLAY_YSIZE) y1 = DISPLAY_YSIZE;

    // starting end
    uint8_t yStartOffset = y0 & 7;
    uint8_t startMask = 0xff << yStartOffset;
    uint8_t yEndOffset = y1 & 7;
    uint8_t endMask = 0xff >> (8 - yEndOffset - 1);
    uint8_t yStartPage = y0 >> 3;
    uint8_t yEndPage = y1 >> 3;

    while (yStartPage <= yEndPage) {
        if (yStartPage == yEndPage) {
            startMask &= endMask;
        }

        switch (color) {
            case SSD1306_COLOR_WHITE :
                ssd1306_display_buffer[yStartPage][x0] |= startMask;
                break;

            case SSD1306_COLOR_BLACK :
                ssd1306_display_buffer[yStartPage][x0] &= ~startMask;
                break;

            case SSD1306_COLOR_INVERT :
                ssd1306_display_buffer[yStartPage][x0] ^= startMask;
                break;

            case SSD1306_COLOR_NONE :
            default:
                return;
        }

        startMask = 0xff;
        yStartPage++;
    }
#else
    while (y0 <= y1) {
        ssd1306_set_pixel(x0, y0++, color);
    }
#endif
}

void ssd1306_vline_dashed(coord_x x0, coord_y y0, coord_y y1) {
    ssd1306_normalize_y(&y0, &y1);
    while (y0 <= y1) {
        ssd1306_set_pixel(x0, y0++, ssd1306_next_dash_color());
    }
}

void ssd1306_hline_to(coord_x x1) {
    if (ssd1306_cursor_x != x1) {
        coord_x x0 = ssd1306_cursor_x;
        ssd1306_cursor_x = x1;

        ssd1306_normalize_x(&x0, &x1);
        if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
            x0++;
        }
        ssd1306_hline_dashed(x0, ssd1306_cursor_y, x1);
        ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
    }
}

// draws a vertical line in given color
void ssd1306_vline_to(coord_y y1) {
    if (ssd1306_cursor_y != y1) {
        coord_y y0 = ssd1306_cursor_y;
        ssd1306_cursor_y = y1;

        ssd1306_normalize_y(&y0, &y1);
        if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
            y0++;
        }
        ssd1306_vline_dashed(ssd1306_cursor_x, y0, y1);
        ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
    }
}

void ssd1306_line_to(coord_x x1, coord_y y1) {
    if (x1 == ssd1306_cursor_x) {
        ssd1306_vline_to(y1);
    } else if (y1 == ssd1306_cursor_y) {
        ssd1306_hline_to(x1);
    } else {
        int8_t dx = abs(x1 - ssd1306_cursor_x), sx = ssd1306_cursor_x < x1 ? 1 : -1;
        int8_t dy = abs(y1 - ssd1306_cursor_y), sy = ssd1306_cursor_y < y1 ? 1 : -1;

        int8_t e2;
        int8_t err = (int8_t) ((dx > dy ? dx : -dy) / 2);
        uint8_t skip = ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH;

        for (;;) {
            if (skip) {
                skip = 0;
                continue;
            }
            ssd1306_set_pixel(ssd1306_cursor_x, ssd1306_cursor_y, ssd1306_next_dash_color());

            if (ssd1306_cursor_x == x1 && ssd1306_cursor_y == y1) {
                ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
                break;
            }

            e2 = err;
            if (e2 > -dx) {
                err -= dy;
                ssd1306_cursor_x += sx;
            }
            if (e2 < dy) {
                err += dx;
                ssd1306_cursor_y += sy;
            }
        }
    }
}

bool ssd1306_normalize_x(coord_x *pX1, coord_x *pX2) {
    if (*pX1 > *pX2) {
        int16_t t = *pX1;
        *pX1 = *pX2;
        *pX2 = t;
        return true;
    }
    return false;
}

bool ssd1306_normalize_y(coord_y *pY1, coord_y *pY2) {
    if (*pY1 > *pY2) {
        int8_t t = *pY1;
        *pY1 = *pY2;
        *pY2 = t;
        return true;
    }
    return false;
}

void ssd1306_double_line_to(coord_x x1, coord_y y1, int8_t xSpc, int8_t ySpc) {
    coord_x x0 = ssd1306_cursor_x;
    coord_y y0 = ssd1306_cursor_y;

    ssd1306_line_to(x1, y1);
    ssd1306_move_by(xSpc, ySpc);
    ssd1306_line_to(x0 + xSpc, y0 + ySpc);
    ssd1306_move_to(x1, y1);
}

void ssd1306_double_hline_to(coord_x x1, int8_t ySpc) {
    coord_x x0 = ssd1306_cursor_x;
    coord_y y0 = ssd1306_cursor_y;

    ssd1306_hline_to(x1);
    ssd1306_move_y_by(ySpc);
    ssd1306_hline_to(x0);
    ssd1306_move_to(x1, y0);
}

void ssd1306_double_vline_to(coord_y y1, int8_t xSpc) {
    coord_x x0 = ssd1306_cursor_x;
    coord_y y0 = ssd1306_cursor_y;

    ssd1306_vline_to(y1);
    ssd1306_move_x_by(xSpc);
    ssd1306_vline_to(y0);
    ssd1306_move_to(x0, y1);
}

void ssd1306_vlines(coord_x x0, coord_y y0, coord_x x1, coord_y y1, color_t color) {
    ssd1306_normalize_x(&x0, &x1);
    ssd1306_normalize_y(&y0, &y1);

    for (int i = x0; i <= x1; i++) {
        ssd1306_vline(i, y0, y1, ssd1306_back_color);
    }
}

void ssd1306_hlines(coord_x x0, coord_y y0, coord_x x1, coord_y y1, color_t color) {
#ifdef SSD1306_BIT_BLIT
    ssd1306_vlines(x0, y0, x1, y1, color);
#else
    // with blt can swap and turn these into vlines
    ssd1306_normalize_x(&x0, &x1);
    ssd1306_normalize_y(&y0, &y1);

    for (int i = y0; i <= y1; i++) {
        ssd1306_hline(x0, i, x1, ssd1306_back_color);
    }
#endif
}

// draws a rectangle in given color
void ssd1306_rect(coord_x x1, coord_y y1) {
    coord_x x0 = ssd1306_cursor_x;
    coord_y y0 = ssd1306_cursor_y;

    ssd1306_normalize_x(&x0, &x1);
    ssd1306_normalize_y(&y0, &y1);

    if (ssd1306_back_color != SSD1306_COLOR_NONE && x0 + 1 <= x1 - 1 && y0 + 1 <= y1 - 1) {
        ssd1306_vlines(x0 + 1, y0 + 1, x1 - 1, y1 - 1, ssd1306_back_color);
    }

    ssd1306_hline_dashed(x0, y0, x1);
    ssd1306_vline_dashed(x1, y0 + 1, y1);
    ssd1306_hline_dashed(x1 - 1, y1, x0);
    ssd1306_vline_dashed(x0, y1 - 1, y0 + 1);
}

// Function for circle-generation
// using Bresenham's algorithm
// octants are:
//         -x -y 128    1 +x -y
//            \     |     /
//  -y -x   64  \   |   /     2 +y -x
//                \ | /
//     -------------|---------------
//                / | \
//  -y +x  32   /   |   \     4 +y +x
//            /     |     \
//     -x +y   16      8 +x +y
//
// NOTE: y axis is +ve going down and -ve, in graphics coords
//
//  Drawing order for each quadrant is clockwise,
//  for odd octants it is clockwise for even octants it is counterclockwise
//
// modified from https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
// to be able to use dashed line drawing

void ssd1306_fill_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octants) {
//    printf("oct %d: x: %d, y: %d\n", octants, x, y);
    if ((octants & SSD1306_OCT_1) && x < y + 1) ssd1306_vline(cx + x, cy - y + 1, cy - x - (x ? 0 : 1), ssd1306_back_color);
    if ((octants & SSD1306_OCT_2) && x <= y - 1) ssd1306_hline(cx + y - 1, cy - x, cx + x + 1, ssd1306_back_color);

    if ((octants & SSD1306_OCT_3) && x <= y - 1) ssd1306_hline(cx + y - 1, cy + x, cx + x + (x ? 0 : 1), ssd1306_back_color);
    if ((octants & SSD1306_OCT_4)) ssd1306_vline(cx + x, cy + y - 1, cy + x + 1, ssd1306_back_color);

    if ((octants & SSD1306_OCT_5) && x < y + 1) ssd1306_vline(cx - x, cy + y - 1, cy + x + (x ? 0 : 1), ssd1306_back_color);
    if ((octants & SSD1306_OCT_6)) ssd1306_hline(cx - y + 1, cy + x, cx - x - 1, ssd1306_back_color);

    if ((octants & SSD1306_OCT_7) && x <= y - 1) ssd1306_hline(cx - y + 1, cy - x, cx - x - (x ? 0 : 1), ssd1306_back_color);
    if ((octants & SSD1306_OCT_8) && x < y + 1) ssd1306_vline(cx - x, cy - y + 1, cy - x - 1, ssd1306_back_color);
}

void ssd1306_draw_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs) {
    if (octs & SSD1306_OCT_1) ssd1306_set_pixel(cx + x, cy - y, ssd1306_next_dash_color());
    if (octs & SSD1306_OCT_2) ssd1306_set_pixel(cx + y, cy - x, ssd1306_next_dash_color());

    if (octs & SSD1306_OCT_3) ssd1306_set_pixel(cx + y, cy + x, ssd1306_next_dash_color());
    if (octs & SSD1306_OCT_4) ssd1306_set_pixel(cx + x, cy + y, ssd1306_next_dash_color());

    if (octs & SSD1306_OCT_5) ssd1306_set_pixel(cx - x, cy + y, ssd1306_next_dash_color());
    if (octs & SSD1306_OCT_6) ssd1306_set_pixel(cx - y, cy + x, ssd1306_next_dash_color());

    if (octs & SSD1306_OCT_7) ssd1306_set_pixel(cx - y, cy - x, ssd1306_next_dash_color());
    if (octs & SSD1306_OCT_8) ssd1306_set_pixel(cx - x, cy - y, ssd1306_next_dash_color());
}

// here we can go inefficiently and draw 8 pixels and only processing one octant
// or draw only 1 pixel and process a quadrant at a time but preserve circumference pixel drawing
// order so as to preserve dash pattern

#define MAX_DECISIONS 64

void ssd1306_circle_octants(int8_t r, uint8_t octants, fp_circle_octants drawOctants) {
    coord_x xc = ssd1306_cursor_x;
    coord_y yc = ssd1306_cursor_y;
    coord_x x;
    coord_y y;
    int16_t d;
    uint8_t dc;
    uint8_t ds[MAX_DECISIONS / 8];  // decisions for quads

    coord_x xe;
    coord_y ye;
    int16_t de = 0;
    uint8_t noSkip = 1;

    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;

    // go through octants one at a time to preserve dash pattern
    for (uint8_t octant = 0x01; octant; octant <<= 1) {
        if (de == 0 || (octant & 0x55 & octants)) {
            xc = ssd1306_cursor_x;
            yc = ssd1306_cursor_y;
            x = 0;
            y = r;
            d = 3 - 2 * r;

            if (octant & 0x55 & octants) {
                drawOctants(xc, yc, x, y, octant);
            }

            dc = 0;
            while (x < y && dc < MAX_DECISIONS) {
                // for each pixel we will
                // draw all eight pixels or only one for the current quadrant if dashed lines
                x++;

                // check for decision parameter and correspondingly update d, x, y
                uint8_t dp = dc >> 3;
                uint8_t db = 1 << (dc & 7);
                dc++;
                if (d > 0) {
                    y--;
                    d = d + 4 * (x - y) + 10;
                    ds[dp] |= db;
                } else {
                    d = d + 4 * x + 6;
                    ds[dp] &= ~db;
                }

                if (octant & 0x55 & octants) drawOctants(xc, yc, x, y, octant);
            }

            xe = x;
            ye = y;
            de = dc;
        }

        if (octant & 0xAA & octants) {
            // repeat, reversing order
            x = xe;
            y = ye;
            dc = de;
            noSkip = x <= y;

            if (noSkip && (octant & 0xAA & octants)) {
                drawOctants(xc, yc, x, y, octant);
            }
            while (x > 1) {
                x--;

                // for each pixel we will
                // draw all eight pixels or only one for the current quadrant if dashed lines
                --dc;
                uint8_t dp = dc >> 3;
                uint8_t db = 1 << (dc & 7);
                // check for decision parameter and correspondingly update d, x, y
                if (ds[dp] & db) {
                    y++;
                }

                if (noSkip) drawOctants(xc, yc, x, y, octant);
                noSkip = 1;
            }
        }
    }
}

// draws circle at x,y with given radius & color
void ssd1306_circle(int8_t radius) {
    // first fill
    // then outline
    // do all 8 octants
    if (ssd1306_back_color != SSD1306_COLOR_NONE) {
        ssd1306_circle_octants(radius, 0xff, ssd1306_fill_circle_octants);

        if (ssd1306_back_color != SSD1306_COLOR_NONE) {
            // draw the pixel at cx,cy
            ssd1306_set_pixel(ssd1306_cursor_x, ssd1306_cursor_y, ssd1306_back_color);
        }
    }

    ssd1306_circle_octants(radius, 0xff, ssd1306_draw_circle_octants);
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void ssd1306_round_rect(coord_x x1, coord_y y1, int8_t r, uint8_t octants) {
    bool doFill = ssd1306_back_color != SSD1306_COLOR_NONE;

    coord_x x0 = ssd1306_cursor_x;
    coord_y y0 = ssd1306_cursor_y;

    ssd1306_normalize_x(&x0, &x1);
    ssd1306_normalize_y(&y0, &y1);

    ssd1306_cursor_x += r; // x0+r
    coord_x x;
    coord_y y;

    x = x1 - r;
    if (octants & SSD1306_OCT_1) {
        ssd1306_hline_to(x); /* top side */
        if (doFill) {
            ssd1306_vlines(x0 + r, y0 + 1, x1 - r - 1, y0 + r, ssd1306_back_color);
            // DEBUG : remove
            // ssd1306_display();
        }
    }

    ssd1306_cursor_x = x;
    ssd1306_cursor_y += r; // y0+r
    if (octants & SSD1306_OCT_2) {
        ssd1306_circle_octants(r, SSD1306_QUAD_1, ssd1306_draw_circle_octants); // upper right corner
        if (doFill) ssd1306_circle_octants(r, SSD1306_QUAD_1, ssd1306_fill_circle_octants);
    }

    ssd1306_cursor_x += r; // x1
    y = y1 - r;
    if (octants & SSD1306_OCT_3) {
        ssd1306_vline_to(y); // right side
        if (doFill) {
            ssd1306_hlines(x1 - r, y0 + r, x1 - 1, y1 - r - 1, ssd1306_back_color);
            // DEBUG : remove
            // ssd1306_display();
        }
    }

    ssd1306_cursor_y = y;
    ssd1306_cursor_x -= r; // x1-r
    if (octants & SSD1306_OCT_4) {
        ssd1306_circle_octants(r, SSD1306_QUAD_2, ssd1306_draw_circle_octants); // lower right corner
        if (doFill) ssd1306_circle_octants(r, SSD1306_QUAD_2, ssd1306_fill_circle_octants);
    }

    ssd1306_cursor_y += r; // y1
    x = x0 + r;
    if (octants & SSD1306_OCT_5) {
        ssd1306_hline_to(x); // bottom side
        if (doFill) {
            ssd1306_vlines(x0 + r + 1, y1 - r, x1 - r, y1 - 1, ssd1306_back_color);
            // DEBUG : remove
            // ssd1306_display();
        }
    }

    ssd1306_cursor_x = x;
    ssd1306_cursor_y -= r; // y1-r
    if (octants & SSD1306_OCT_6) {
        ssd1306_circle_octants(r, SSD1306_QUAD_3, ssd1306_draw_circle_octants); // lower right corner
        if (doFill) ssd1306_circle_octants(r, SSD1306_QUAD_3, ssd1306_fill_circle_octants);
    }

    ssd1306_cursor_x -= r; // x0
    y = y0 + r;
    if (octants & SSD1306_OCT_7) {
        ssd1306_vline_to(y); // left side
        if (doFill) {
            ssd1306_hlines(x0 + 1, y0 + r + 1, x0 + r, y1 - r, ssd1306_back_color);
            // DEBUG : remove
            // ssd1306_display();
        }
    }

    ssd1306_cursor_y = y;
    ssd1306_cursor_x += r; // x0+r
    if (octants & SSD1306_OCT_8) {
        // upper right corner
        ssd1306_circle_octants(r, SSD1306_QUAD_4, ssd1306_draw_circle_octants);
        if (doFill) ssd1306_circle_octants(r, SSD1306_QUAD_4, ssd1306_fill_circle_octants);
    }

    if (doFill
        && ((octants & (SSD1306_OCT_1 | SSD1306_OCT_3 | SSD1306_OCT_5 | SSD1306_OCT_7)) == (SSD1306_OCT_1 | SSD1306_OCT_3 | SSD1306_OCT_5 | SSD1306_OCT_7))) {
        // fill the center
        ssd1306_vlines(x0 + r + 1, y0 + r + 1, x1 - r - 1, y1 - r - 1, ssd1306_back_color);
        // DEBUG : remove
        // ssd1306_display();
    }

    ssd1306_cursor_x -= r; // x0
    ssd1306_cursor_y -= r; // y0
}

void ssd1306_bitmap(const uint8_t bitmap[], uint8_t w, uint8_t h) {
    coord_y y = ssd1306_cursor_y;

    uint8_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
    uint8_t byte = 0;

    for (uint8_t j = 0; j < h; j++, y++) {
        for (uint8_t i = 0; i < w; i++) {
            if (i & 7) byte <<= 1;
            else byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);

            color_t color = (byte & 0x80) ? ssd1306_fore_color : ssd1306_back_color;
            if (color != SSD1306_COLOR_NONE) {
                ssd1306_set_pixel(ssd1306_cursor_x + i, y, color);
            }
        }
    }
}

#ifdef SSD1306_FANCY
// TEST: not done or debugged
// draws an ellipse of given width & height
// two-part Bresenham method
// note: slight discontinuity between parts on some (narrow) ellipses.
void ssd1306_ellipse(int cx, int cy, int width, int height, color_t color) {
    int a = width >> 1, b = height >> 1;
    int x = 0, y = b;
    long a2 = a * a * 2;
    long b2 = b * b * 2;
    long error = (long) a * a * b;
    long stopY = 0, stopX = a2 * b;
    while (stopY <= stopX) {
        ssd1306_setPixel(cx + x, cy + y, color);
        ssd1306_setPixel(cx + x, cy - y, color);
        ssd1306_setPixel(cx - x, cy + y, color);
        ssd1306_setPixel(cx - x, cy - y, color);
        x++;
        error -= b2 * (x - 1);
        stopY += b2;
        if (error < 0) {
            error += a2 * (y - 1);
            y--;
            stopX -= a2;
        }
    }
    x = a;
    y = 0;
    error = b * b * a;
    stopY = a * b2;
    stopX = 0;
    while (stopY >= stopX) {
        ssd1306_setPixel(cx + x, cy + y, color);
        ssd1306_setPixel(cx + x, cy - y, color);
        ssd1306_setPixel(cx - x, cy + y, color);
        ssd1306_setPixel(cx - x, cy - y, color);
        y++;
        error -= a2 * (y - 1);
        stopX += a2;
        if (error < 0) {
            error += b2 * (x - 1);
            x--;
            stopY -= b2;
        }
    }
}

// draws a filled ellipse of given width & height
void ssd1306_fillEllipse(int cx, int cy, int width, int height, color_t color) {
    int a = width >> 1, b = height >> 1; // get x & y radii
    int x1, x0 = a, y = 1;
    int dx = 0;
    long a2 = a * a, b2 = b * b;
    long a2b2 = (long) a2 * b2; // need longs: big numbers!

    hLine(cx - a, cx + a, cy, color); // draw center line

    while (y <= b) { // draw horizontal lines...
        for (x1 = x0 - (dx - 1); x1 > 0; x1--)
            if (b2 * x1 * x1 + a2 * y * y <= a2b2) break;

        dx = x0 - x1;
        x0 = x1;

        hLine(cx - x0, cx + x0, cy + y, color);
        hLine(cx - x0, cx + x0, cy - y, color);
        y += 1;
    }
}
#endif

// ---------------------------------------------------------------------------
// TEXT ROUTINES
//
// Each ASCII character is 5x7, with one pixel space between characters
// So, character width = 6 pixels & character height = 8 pixels. //

void ssd1306_update_char_size() {
    ssd1306_char_y_size = ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_HEIGHT ? CHAR_HEIGHT * 2 : CHAR_HEIGHT;
    ssd1306_char_x_size = ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_WIDTH ? CHAR_WIDTH * 2 : CHAR_WIDTH;

#ifdef SSD1306_SMALL_FONT
    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_SMALL_FONT) {
        ssd1306_char_x_size = SSD1306_SMALL_CHAR_WIDTH;
        ssd1306_char_y_size = SSD1306_SMALL_CHAR_HEIGHT;
    }
#endif
}

void ssd1306_set_text_size_flags(uint8_t flags) {
    flags &= SSD1306_TEXT_FLAG_CHAR_SIZE;

    if (flags != (ssd1306_text_flags & SSD1306_TEXT_FLAG_CHAR_SIZE)) {
        // size changing
        ssd1306_clear_text_flags(SSD1306_TEXT_FLAG_CHAR_SIZE);
        ssd1306_set_text_flags(flags);
    }
}

void ssd1306_changing_text_flags(uint8_t flags) {
    if (flags & (SSD1306_TEXT_FLAG_WRAP | SSD1306_TEXT_FLAG_WRAP_ON_SPC | SSD1306_TEXT_FLAG_BORDER | SSD1306_TEXT_FLAG_CHAR_SIZE)) {
        // flush any chars to be printed with space wrapping
        ssd1306_flush_wrap_chars();
    }
}

void ssd1306_set_text_flags(uint8_t flags) {
    ssd1306_changing_text_flags(flags);

    if (flags & SSD1306_TEXT_FLAG_BORDER) {
        flags |= SSD1306_TEXT_FLAG_BORDER_CHAR | SSD1306_TEXT_FLAG_BORDER | SSD1306_TEXT_FLAG_BORDER_LINE;
    }

    if (flags & SSD1306_TEXT_FLAG_WRAP_ON_SPC) {
        flags |= SSD1306_TEXT_FLAG_WRAP;
    }

    if (flags & SSD1306_TEXT_FLAG_DOUBLE_SIZE) {
        flags &= ~SSD1306_TEXT_FLAG_SMALL_FONT;
    }

    ssd1306_text_flags |= flags;
    ssd1306_update_char_size();
}

void ssd1306_clear_text_flags(uint8_t flags) {
    ssd1306_changing_text_flags(flags);

    if (flags & SSD1306_TEXT_FLAG_BORDER) {
        flags |= SSD1306_TEXT_FLAG_BORDER_CHAR | SSD1306_TEXT_FLAG_BORDER | SSD1306_TEXT_FLAG_BORDER_LINE;
    }

    if (flags & SSD1306_TEXT_FLAG_WRAP) {
        flags |= SSD1306_TEXT_FLAG_WRAP_ON_SPC;
    }

    ssd1306_text_flags &= ~flags;
    ssd1306_update_char_size();
}

void ssd1306_set_text_col_row(int8_t x, coord_y y) {
    ssd1306_cursor_x = x * ssd1306_char_x_size;
    ssd1306_cursor_y = y * ssd1306_char_y_size;
}

bool ssd1306_is_char_visible() {
    return !(ssd1306_cursor_x + ssd1306_char_x_size <= 0 || ssd1306_cursor_x >= DISPLAY_XSIZE || ssd1306_cursor_y + ssd1306_char_y_size <= 0 ||
             ssd1306_cursor_y >= DISPLAY_YSIZE);
}

bool ssd1306_is_char_clipped() {
    return ssd1306_is_char_visible()
           && (ssd1306_cursor_x < 0 || ssd1306_cursor_x + ssd1306_char_x_size > DISPLAY_XSIZE || ssd1306_cursor_y < 0 || ssd1306_cursor_y + ssd1306_char_y_size > DISPLAY_YSIZE);
}

void ssd1306_set_colors(color_t fg, color_t bg) {
    ssd1306_flush_wrap_chars();
    ssd1306_fore_color = fg;
    ssd1306_back_color = bg;
}

void ssd1306_set_pattern_colors(color_t fg, color_t bg, color_t gp) {
    ssd1306_flush_wrap_chars();
    ssd1306_fore_color = fg;
    ssd1306_back_color = bg;
    ssd1306_gap_color = gp;
}

void ssd1306_move_to(coord_x x, coord_y y) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_x = x;
    ssd1306_cursor_y = y;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

void ssd1306_move_x_to(coord_x x) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_x = x;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

void ssd1306_move_y_to(coord_y y) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_y = y;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

void ssd1306_move_by(coord_x x, coord_y y) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_x += x;
    ssd1306_cursor_y += y;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

void ssd1306_move_x_by(coord_x x) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_x += x;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

void ssd1306_move_y_by(coord_y y) {
    ssd1306_flush_wrap_chars();
    ssd1306_cursor_y += y;
    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
}

#ifdef SSD1306_HAVE_ALT_FONTS

bool ssd1306_findSubstitutedChar(
        uint8_t ch,
        const char *pChars,
        uint8_t xPixels,
        uint8_t yPixels,
        const char *pCharMap,
        const uint8_t *pSet,
        const char **pBits
) {
    uint8_t flags = pgm_read_byte(pSet + (ch >> 3));
    bool chFlag = flags & (1 << (ch & 7));
#ifdef SERIAL_DEBUG_FONTS
    printf_P(PSTR("Char 0x%02X flags: 0x%02X, %S.\n"), (uint16_t)ch, flags, chFlag ? PST(" found") : PSTR(" not found"));
#endif

    if (chFlag) {
        // have it, now find it
        const char *pMap = pCharMap;
        char c = pgm_read_byte(pMap);
        while (c) {
            if (c == ch) {
                // have it
                uint8_t index = pMap - pCharMap;
                *pBits = pChars + (index * xPixels * ((yPixels + 7) / 8));
                return 1;
            }
            pMap++;
            c = pgm_read_byte(pMap);
        }
    }
    return 0;
}

#endif

#ifdef SSD1306_BIT_BLIT

#define SSD1306_BITBLIT_OP_DST_NONE     0x00  // nop
#define SSD1306_BITBLIT_OP_DST_CLEAR    0x01  // dst = 0, ie black
#define SSD1306_BITBLIT_OP_DST_SET      0x02  // dst = 1, ie white
#define SSD1306_BITBLIT_OP_DST_INVERT   0x03  // dst = ~dst, invert

#define SSD1306_BITBLIT_OP_DST          0x03  // destination op

#define SSD1306_BITBLIT_OP_NONE         (0x00 << 2)  // noop
#define SSD1306_BITBLIT_OP_OR           (0x01 << 2)  // dst |= src
#define SSD1306_BITBLIT_OP_AND          (0x02 << 2)  // dst &= src
#define SSD1306_BITBLIT_OP_OR_NOT       (0x03 << 2)  // dst |= ~src
#define SSD1306_BITBLIT_OP_AND_NOT      (0x04 << 2)  // dst &= ~src
#define SSD1306_BITBLIT_OP_XOR          (0x05 << 2)  // dst ^= src

#define SSD1306_BITBLIT_OP              (0x07 << 2)  // op

static const uint8_t colorOnColorToOp[] PROGMEM = {
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_NONE),        // NONE on NONE
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_AND),         // NONE on BLACK
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_OR_NOT),      // NONE on WHITE
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_XOR),       // NONE on INVERT
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_AND_NOT),     // BLACK on NONE
        (SSD1306_BITBLIT_OP_DST_CLEAR | SSD1306_BITBLIT_OP_NONE),       // BLACK on BLACK
        (SSD1306_BITBLIT_OP_DST_CLEAR | SSD1306_BITBLIT_OP_OR_NOT),     // BLACK on WHITE
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_AND_NOT),   // BLACK on INVERT
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_OR),          // WHITE on NONE
        (SSD1306_BITBLIT_OP_DST_CLEAR | SSD1306_BITBLIT_OP_OR),         // WHITE on BLACK
        (SSD1306_BITBLIT_OP_DST_SET | SSD1306_BITBLIT_OP_NONE),         // WHITE on WHITE
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_OR),        // WHITE on INVERT
        (SSD1306_BITBLIT_OP_DST_NONE | SSD1306_BITBLIT_OP_XOR),         // INVERT on NONE
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_AND),       // INVERT on BLACK
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_OR_NOT),    // INVERT on WHITE
        (SSD1306_BITBLIT_OP_DST_INVERT | SSD1306_BITBLIT_OP_NONE),      // INVERT on INVERT
};

static uint8_t blt_op;
static uint8_t blt_x0;
static uint8_t blt_x1;
static uint8_t blt_yPage;
static int8_t blt_yOffset;
static uint8_t blt_mask;
static const uint8_t *blt_p;

static void bitBlt() {
    for (uint8_t x = blt_x0; x < blt_x1; x++, blt_p++) {
        switch (blt_op & SSD1306_BITBLIT_OP_DST) {
            case SSD1306_BITBLIT_OP_DST_CLEAR :
                ssd1306_display_buffer[blt_yPage][x] &= ~blt_mask;
                break;

            case SSD1306_BITBLIT_OP_DST_SET :
                ssd1306_display_buffer[blt_yPage][x] |= blt_mask;
                break;

            case SSD1306_BITBLIT_OP_DST_INVERT :
                ssd1306_display_buffer[blt_yPage][x] ^= blt_mask;
                break;

            case SSD1306_BITBLIT_OP_DST_NONE :
            default:
                break;
        }

        uint8_t src = blt_yOffset < 0 ? *blt_p >> -blt_yOffset : *blt_p << blt_yOffset;
        switch (blt_op & SSD1306_BITBLIT_OP) {
            case SSD1306_BITBLIT_OP_OR :
                ssd1306_display_buffer[blt_yPage][x] |= src & blt_mask;
                break;
            case SSD1306_BITBLIT_OP_AND :
                ssd1306_display_buffer[blt_yPage][x] &= src | ~blt_mask;
                break;
            case SSD1306_BITBLIT_OP_OR_NOT :
                ssd1306_display_buffer[blt_yPage][x] |= (~src) & blt_mask;
                break;
            case SSD1306_BITBLIT_OP_AND_NOT :
                ssd1306_display_buffer[blt_yPage][x] &= (~src) | ~blt_mask;
                break;
            case SSD1306_BITBLIT_OP_XOR :
                ssd1306_display_buffer[blt_yPage][x] ^= src & blt_mask;
                break;

            case SSD1306_BITBLIT_OP_NONE :
            default:
                break;
        }
    }
}

// bit blit maximum 8 rows of data to display buffer
void ssd1306_bitBlit(const uint8_t *pData, uint8_t xSize, uint8_t ySize) {
    coord_x x0 = ssd1306_cursor_x;

    if (x0 < 0) {
        pData += -x0;
        xSize -= -x0;
        x0 = 0;
    }
    blt_x0 = x0;

    coord_y y0 = ssd1306_cursor_y;
    int8_t blt_yPreOffset = 0;

    if (y0 < 0) {
        ySize -= -y0;
        blt_yPreOffset = y0;
        y0 = 0;
    }

    blt_yOffset = y0 & 7;
    blt_yPage = y0 >> 3;

    if (ySize - blt_yPreOffset < 8) {
        blt_mask = (0xff >> (8 - ySize - blt_yPreOffset)) << blt_yOffset;
    } else {
        blt_mask = 0xff << blt_yOffset;
    }

    blt_x1 = x0 + xSize;
    blt_op = pgm_read_byte(colorOnColorToOp + COLOR_ON_COLOR(ssd1306_fore_color, ssd1306_back_color));
    blt_p = pData;

    if (blt_x1 > DISPLAY_XSIZE) blt_x1 = DISPLAY_XSIZE;
    if (ySize > DISPLAY_YSIZE - y0) ySize = DISPLAY_YSIZE - y0;
    if (blt_yPreOffset) blt_yOffset = blt_yPreOffset;

    bitBlt();

    if (blt_yOffset + ySize > 8) {
        // have to split over two rows
        blt_yOffset = y0 & 7;
        blt_yPage++;
        blt_p = pData;
        blt_yOffset = blt_yOffset - 8;
        blt_mask = 0xff >> (-blt_yOffset + (8 - ySize - blt_yPreOffset));

        bitBlt();
    }
}

void ssd1306_double_width(uint8_t *pData, uint8_t xSize) {
    for (uint8_t s = 0, *ps = pData + xSize, *pd = ps + xSize; s < xSize; s++) {
        *--pd = *--ps;
        *--pd = *ps;
    }
}

uint16_t ssd1306_double_bits(uint8_t b) {
    uint16_t b1;
    uint16_t b2;

    b1 = b;
    b2 = (b1 & 0xf0) << 4;   // 0x0f00
    b1 &= 0x0f;              // 0x000f
    b1 |= b2;                // 0x0f0f

    b2 = (b1 & 0x0C0C) << 2;  // 0x3030
    b1 &= 0x0303;             // 0x0303
    b1 |= b2;                 // 0x3333

    b2 = (b1 & 0x2222) << 1;  // 0x4444
    b2 |= b2 << 1;            // 0xCCCC
    b1 &= 0x1111;             // 0x1111
    b1 |= b1 << 1;            // 0x3333
    b1 |= b2;                 // 0xffff

    return b1;
}

void ssd1306_double_height(uint8_t *pData, uint8_t xSize) {
    for (uint8_t s = 0, *ps = pData, *pd = pData + xSize; s < xSize; s++) {
        uint16_t b1;

        b1 = ssd1306_double_bits(*ps);
        *ps++ = b1 & 0xff;
        *pd++ = b1 >> 8;
    }
}

// ssd1306_print ch to display X,Y coordinates using ASCII 5x7 font
bool ssd1306_put_ch(char ch) {
    bool retVal = false;
#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t start = micros();
#endif

    if (ch == '\n') {
        // go to next line and left margin
        ssd1306_new_line();
    } else if (ch >= 32 && ssd1306_is_char_visible() && (ssd1306_fore_color != SSD1306_COLOR_NONE || ssd1306_back_color != SSD1306_COLOR_NONE)) {
        if (!(ssd1306_flags & SSD1306_FLAG_SIMULATED_PRINT)) {
            uint8_t textSize = ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_SIZE;
            uint8_t xPixels = SSD1306_FONT_X_PIXELS;
            uint8_t yPixels = SSD1306_FONT_Y_PIXELS;
            uint8_t charBits[20];

#ifndef SSD1306_HAVE_ALT_FONTS
            *((uint32_t *) (charBits)) = pgm_read_dword((const char *) FONT_CHARS[ch - 32]);
            charBits[4] = pgm_read_byte((const char *) FONT_CHARS[ch - 32] + 4);
#else
            const char *pBits = (const char *) FONT_CHARS[ch - 32];

#ifdef SSD1306_LARGE_FONT
            // see if we have replacement
            if (textSize == SSD1306_TEXT_FLAG_DOUBLE_SIZE && ssd1306_findSubstitutedChar(ch, (PGM_P) LARGE_CHARS, SSD1306_LARGE_X_PIXELS, SSD1306_LARGE_Y_PIXELS, LARGE_CHAR_MAP, LARGE_CHAR_SET, &pBits)) {
                textSize = 0;
                xPixels = SSD1306_LARGE_X_PIXELS;
                yPixels = SSD1306_LARGE_Y_PIXELS;
            }
#endif

#ifdef SSD1306_SMALL_FONT
            // see if we have replacement
            if ((ssd1306_text_flags & SSD1306_TEXT_FLAG_SMALL_FONT) && ssd1306_findSubstitutedChar(ch, (PGM_P) SMALL_CHARS, SSD1306_SMALL_X_PIXELS, SSD1306_SMALL_Y_PIXELS, SMALL_CHAR_MAP, SMALL_CHAR_SET, &pBits)) {
                textSize = 0;
                xPixels = SSD1306_SMALL_X_PIXELS;
                yPixels = SSD1306_SMALL_Y_PIXELS;
            }
#endif
            *((uint32_t *) (charBits)) = pgm_read_dword(pBits);
            if (xPixels == 10) {
                *((uint32_t *) (charBits + 4)) = pgm_read_dword(pBits + 4);
                *((uint32_t *) (charBits + 8)) = pgm_read_dword(pBits + 8);
                *((uint32_t *) (charBits + 12)) = pgm_read_dword(pBits + 12);
                *((uint32_t *) (charBits + 16)) = pgm_read_dword(pBits + 16);
            } else if (xPixels == 5) {
                charBits[4] = pgm_read_byte(pBits + 4);
            } // else if (xPixels == 4) { /* all read in */ }

#endif
            // may need to double some dimension
            if (textSize & SSD1306_TEXT_FLAG_DOUBLE_WIDTH) {
                ssd1306_double_width(charBits, xPixels);
                xPixels *= 2;
            }

            if (textSize & SSD1306_TEXT_FLAG_DOUBLE_HEIGHT) {
                // double height or double size, first double the height, then double width if double size
                ssd1306_double_height(charBits, xPixels);
                yPixels *= 2;
            }

            ssd1306_bitBlit(charBits, xPixels, yPixels > 8 ? 8 : yPixels);

            if (yPixels > 8) {
                ssd1306_cursor_y += 8;
                if (ssd1306_cursor_y < DISPLAY_YSIZE) {
                    ssd1306_bitBlit(charBits + xPixels, xPixels, yPixels - 8);
                }
                ssd1306_cursor_y -= 8;
            }

            // FIX: these overlap between lines, issue if using invert
            if (ssd1306_back_color != SSD1306_COLOR_NONE) {
                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER_CHAR) {
                    // add left border
                    ssd1306_vline(ssd1306_cursor_x - 1, ssd1306_cursor_y - 1, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_back_color);
                    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_WIDTH) {
                        ssd1306_vline(ssd1306_cursor_x - 2, ssd1306_cursor_y - 1, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_back_color);
                    }
                    ssd1306_text_flags &= ~SSD1306_TEXT_FLAG_BORDER_CHAR;
                }

                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER_LINE) {
                    // add top border
                    ssd1306_hline(ssd1306_cursor_x, ssd1306_cursor_y - 1, ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_back_color);
                }
            }

            // add bottom and right borders always
            // FIX: these overlap between lines, issue if using invert
            if (ssd1306_back_color != SSD1306_COLOR_NONE) {
                ssd1306_hline(ssd1306_cursor_x, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_back_color);
                ssd1306_vline(ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_cursor_y, ssd1306_cursor_y + ssd1306_char_y_size - 2, ssd1306_back_color);
                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_WIDTH) {
                    ssd1306_vline(ssd1306_cursor_x + ssd1306_char_x_size - 2, ssd1306_cursor_y, ssd1306_cursor_y + ssd1306_char_y_size - 2, ssd1306_back_color);
                }
            }
        }
        retVal = true;
    }

#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t end = micros();
    ssd1306_put_ch_total += end - start;
    ssd1306_put_ch_count++;
#endif
    return
            retVal;
}

#else
static uint8_t allBits;

static bool haveSetBitsAtOrAfter(uint8_t bit) {
    return allBits & (0xff << bit) & 0xff;
}

// ssd1306_print ch to display X,Y coordinates using ASCII 5x7 font
bool ssd1306_put_ch(char ch) {
    bool retVal = false;
#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t start = micros();
#endif

    if (ch == '\n') {
        // go to next line and left margin
        ssd1306_new_line();
    } else if (ch >= 32 && ssd1306_is_char_visible() && (ssd1306_fore_color != SSD1306_COLOR_NONE || ssd1306_back_color != SSD1306_COLOR_NONE)) {
        if (!(ssd1306_flags & SSD1306_FLAG_SIMULATED_PRINT)) {
            uint8_t row, col, *pData, *pDataStart, data, mask;
            uint8_t textSize = ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_SIZE;
            uint8_t xPixels = SSD1306_FONT_X_PIXELS;
            uint8_t yPixels = SSD1306_FONT_Y_PIXELS / SSD1306_FONT_Y_ROWS;
            uint8_t subRow = 0;

#ifndef SSD1306_HAVE_ALT_FONTS
            uint8_t charBits[10];
            *((uint32_t *) (charBits)) = pgm_read_dword((const char *) FONT_CHARS[ch - 32]);
            charBits[4] = pgm_read_byte((const char *) FONT_CHARS[ch - 32] + 4);
            pDataStart = charBits;

#else
            uint8_t charBits[20];
            const char *pBits = (const char *) FONT_CHARS[ch - 32];
            uint8_t subRows = SSD1306_FONT_Y_ROWS;

#ifdef SSD1306_LARGE_FONT
            // see if we have replacement
            if (textSize == SSD1306_TEXT_FLAG_DOUBLE_SIZE && ssd1306_findSubstitutedChar(ch, (PGM_P) LARGE_CHARS, SSD1306_LARGE_X_PIXELS, SSD1306_LARGE_Y_PIXELS, LARGE_CHAR_MAP, LARGE_CHAR_SET, &pBits)) {
                subRows = SSD1306_LARGE_Y_ROWS;
                textSize = 0;
                xPixels = SSD1306_LARGE_X_PIXELS;
                yPixels = SSD1306_LARGE_Y_PIXELS / SSD1306_LARGE_Y_ROWS;
            }
#endif

#ifdef SSD1306_SMALL_FONT
            // see if we have replacement
            if ((ssd1306_text_flags & SSD1306_TEXT_FLAG_SMALL_FONT) && ssd1306_findSubstitutedChar(ch, (PGM_P) SMALL_CHARS, SSD1306_SMALL_X_PIXELS, SSD1306_SMALL_Y_PIXELS, SMALL_CHAR_MAP, SMALL_CHAR_SET, &pBits)) {
                subRows = SSD1306_SMALL_Y_ROWS;
                textSize = 0;
                xPixels = SSD1306_SMALL_X_PIXELS;
                yPixels = SSD1306_SMALL_Y_PIXELS / SSD1306_SMALL_Y_ROWS;
            }
#endif
            *((uint32_t *) (charBits)) = pgm_read_dword(pBits);
            if (xPixels == 10) {
                *((uint32_t *) (charBits + 4)) = pgm_read_dword(pBits + 4);
                *((uint32_t *) (charBits + 8)) = pgm_read_dword(pBits + 8);
                *((uint32_t *) (charBits + 12)) = pgm_read_dword(pBits + 12);
                *((uint32_t *) (charBits + 16)) = pgm_read_dword(pBits + 16);
            } else if (xPixels == 5) {
                charBits[4] = pgm_read_byte(pBits + 4);
            } // else if (xPixels == 4) { /* all read in */ }

            for (subRow = 0, pDataStart = charBits; subRow < subRows; subRow++, pDataStart += xPixels) {
#endif
#ifdef SERIAL_DEBUG_GFX_STATS
                uint32_t startLoop = micros();
#endif

                if (ssd1306_back_color != SSD1306_COLOR_NONE && ssd1306_fore_color != SSD1306_COLOR_NONE) {
                    allBits = 0xff; // all are used
                } else {
                    allBits = 0;
                    for (col = 0, pData = pDataStart; col < xPixels; col++, pData++) {
                        allBits |= *pData;
                    }
                    if (ssd1306_back_color != SSD1306_COLOR_NONE) {
                        allBits ^= 0xff; // only 0's are significant
                    }
                }

                for (row = 0, mask = 0x01; row < yPixels && haveSetBitsAtOrAfter(row); row++, mask <<= 1) {
                    for (col = 0, pData = pDataStart; col < xPixels; col++, pData++) {
                        data = *pData; // *(charBits + col + subRow * xPixels);
#ifdef SERIAL_DEBUG_GFX_STATS
                        ssd1306_put_ch_lp_count++;
#endif
                        color_t color = (data & mask) ? ssd1306_fore_color : ssd1306_back_color;
                        if (color == SSD1306_COLOR_NONE) continue;

                        uint8_t row1 = row + (subRow << 3);
                        uint8_t row2;
                        uint8_t col1;
                        uint8_t col2;

                        switch (textSize) {
                            case SSD1306_TEXT_FLAG_DOUBLE_HEIGHT:
                                ssd1306_set_pixel(col1 = col + ssd1306_cursor_x, row2 = ssd1306_cursor_y + row + row, color);
                                ssd1306_set_pixel(col1, ++row2, color);
                                break;
                            case SSD1306_TEXT_FLAG_DOUBLE_WIDTH:
                                ssd1306_set_pixel(col2 = ssd1306_cursor_x + col + col, row1 + ssd1306_cursor_y, color);
                                ssd1306_set_pixel(++col2, row1, color);
                                break;
                            case SSD1306_TEXT_FLAG_DOUBLE_SIZE:
                                ssd1306_set_pixel(col2 = ssd1306_cursor_x + col + col, row2 = ssd1306_cursor_y + row + row, color);
                                ssd1306_set_pixel(++col2, row2, color);
                                ssd1306_set_pixel(--col2, ++row2, color);
                                ssd1306_set_pixel(++col2, row2, color);
                                break;

                            default: {
#ifdef SERIAL_DEBUG_GFX_STATS
                                uint32_t startPix = micros();
#endif
                                ssd1306_set_pixel(col + ssd1306_cursor_x, row1 + ssd1306_cursor_y, color);
#ifdef SERIAL_DEBUG_GFX_STATS
                                uint32_t endPix = micros();
                                ssd1306_put_pixel_total += endPix - startPix;
                                ssd1306_put_pixel_count++;
#endif
                                break;
                            }
                        }
                    }
                }
#ifdef SSD1306_HAVE_ALT_FONTS
            }
#endif
#ifdef SERIAL_DEBUG_GFX_STATS
            uint32_t endLoop = micros();
            ssd1306_put_ch_lp_total += endLoop - startLoop;
#endif

            // FIX: these overlap between lines, issue if using invert
            if (ssd1306_back_color != SSD1306_COLOR_NONE) {
                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER_CHAR) {
                    // add left border
                    ssd1306_vline(ssd1306_cursor_x - 1, ssd1306_cursor_y - 1, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_back_color);
                    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_WIDTH) {
                        ssd1306_vline(ssd1306_cursor_x - 2, ssd1306_cursor_y - 1, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_back_color);
                    }
                    ssd1306_text_flags &= ~SSD1306_TEXT_FLAG_BORDER_CHAR;
                }

                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER_LINE) {
                    // add top border
                    ssd1306_hline(ssd1306_cursor_x, ssd1306_cursor_y - 1, ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_back_color);
                }
            }

            // add bottom and right borders always
            // FIX: these overlap between lines, issue if using invert
            if (ssd1306_back_color != SSD1306_COLOR_NONE) {
                ssd1306_hline(ssd1306_cursor_x, ssd1306_cursor_y + ssd1306_char_y_size - 1, ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_back_color);
                ssd1306_vline(ssd1306_cursor_x + ssd1306_char_x_size - 1, ssd1306_cursor_y, ssd1306_cursor_y + ssd1306_char_y_size - 2, ssd1306_back_color);
                if (ssd1306_text_flags & SSD1306_TEXT_FLAG_DOUBLE_WIDTH) {
                    ssd1306_vline(ssd1306_cursor_x + ssd1306_char_x_size - 2, ssd1306_cursor_y, ssd1306_cursor_y + ssd1306_char_y_size - 2, ssd1306_back_color);
                }
            }
        }
        retVal = true;
    }

#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t end = micros();
    ssd1306_put_ch_total += end -
                            start;
    ssd1306_put_ch_count++;
#endif
    return
            retVal;
}
#endif

void ssd1306_new_line() {
    ssd1306_text_flags &= ~SSD1306_TEXT_FLAG_BORDER_LINE;

    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER) {
        ssd1306_text_flags |= SSD1306_TEXT_FLAG_BORDER_CHAR;
    }

    ssd1306_cursor_x = ssd1306_margin_left;
    ssd1306_cursor_y += ssd1306_char_y_size;

    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_WRAP_ON_SPC) {
        ssd1306_flags &= ~(SSD1306_FLAG_HAD_CHARS_ON_LINE);
        ssd1306_flags |= SSD1306_FLAG_SKIP_LEADING_SPC;
    } else {
        ssd1306_flags &= ~(SSD1306_FLAG_SKIP_LEADING_SPC | SSD1306_FLAG_HAD_CHARS_ON_LINE);
    }
#ifdef SERIAL_DEBUG_GFX
    serial_printC('\n');
#endif
};

bool ssd1306_putChWithMinMax(char ch) {
    bool retVal = 1;

    if (ch == '\n') {
        ssd1306_new_line();
    } else {
        if (ssd1306_cursor_x + ssd1306_char_x_size > ssd1306_margin_right) {
            ssd1306_new_line();
        }

        retVal = ssd1306_put_ch(ch);
        if (retVal) {
            int8_t sy = ssd1306_cursor_y;

#ifdef SERIAL_DEBUG_GFX
            serial_printC(ch);
#endif
            ssd1306_flags &= ~SSD1306_FLAG_SKIP_LEADING_SPC;

            if (ssd1306_min_x > ssd1306_cursor_x) ssd1306_min_x = ssd1306_cursor_x;
            if (ssd1306_min_y > sy) ssd1306_min_y = sy;

            ssd1306_cursor_x += ssd1306_char_x_size;
            if (ssd1306_max_x < ssd1306_cursor_x) ssd1306_max_x = ssd1306_cursor_x;
            if (ssd1306_max_y < ssd1306_cursor_y + ssd1306_char_y_size) ssd1306_max_y = ssd1306_cursor_y + ssd1306_char_y_size;
        }
    }
    return retVal;
};

void ssd1306_flush_wrap_chars() {
    if (ssd1306_wrap_buff_pos) {
        uint8_t textFlags = ssd1306_text_flags;
        ssd1306_text_flags &= ~(SSD1306_TEXT_FLAG_WRAP | SSD1306_TEXT_FLAG_WRAP_ON_SPC);

        uint8_t i;
        for (i = 0; i < ssd1306_wrap_buff_pos; i++) {
            char c = ssd1306_wrap_buff[i];
            if (c != ' ' || !(ssd1306_flags & SSD1306_FLAG_SKIP_LEADING_SPC)) {
                ssd1306_flags |= SSD1306_FLAG_HAD_CHARS_ON_LINE;
                ssd1306_flags &= ~SSD1306_FLAG_SKIP_LEADING_SPC;
                ssd1306_putChWithMinMax(c);
            }
        }

        ssd1306_flags &= ~SSD1306_FLAG_HAD_NO_BREAK_IN_LINE;
        ssd1306_text_flags = textFlags;
        ssd1306_wrap_buff_pos = 0;
    }
}

// writes character to display at current cursor startPosition.
void ssd1306_putc(char ch) {
#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t start = micros();
#endif
    uint8_t skip = 0;

    if (ssd1306_text_flags & SSD1306_TEXT_FLAG_WRAP) {
        // here add logic for wrapping on spaces if the char would fall after the right margin
        if (ssd1306_text_flags & SSD1306_TEXT_FLAG_WRAP_ON_SPC) {
            if (ch == '\n') {
                ssd1306_flush_wrap_chars();
                ssd1306_new_line();
                skip = 1;
            } else if (ch == ' ') {
                if (ssd1306_wrap_buff_pos) {
                    // flush accumulated
                    ssd1306_flush_wrap_chars();
                }

                // if we had just wrapped skip it all together, otherwise add to the buffer
                if (ssd1306_flags & SSD1306_FLAG_SKIP_LEADING_SPC) {
                    skip = 1;
                }
            }

            if (!skip) {
                if (ssd1306_cursor_x + (ssd1306_wrap_buff_pos + 1) * ssd1306_char_x_size >= ssd1306_margin_right) {
                    // flush them all now, if this is these are the first of the batch, otherwise, new line, then flush
                    if (ssd1306_flags & SSD1306_FLAG_HAD_CHARS_ON_LINE) {
                        // output new line then print these
                        // if had no spaces output at all for the line then we need to flush
                        // what is in the buffer as part of the same line because it is one solid block of text
                        if (ssd1306_flags & SSD1306_FLAG_HAD_NO_BREAK_IN_LINE) {
                            ssd1306_flush_wrap_chars();
                        }

                        ssd1306_new_line();
                    } else {
                        int solidBlock = (ssd1306_flags & SSD1306_FLAG_SKIP_LEADING_SPC) && (ssd1306_wrap_buff_pos == 0 || ssd1306_wrap_buff[0] != ' ');

                        ssd1306_flush_wrap_chars();

                        if (solidBlock) {
                            // this is a solid block that the next char will overflow the margin, so afterwards, new line
                            ssd1306_new_line();
                        }

                        // signal forced flush because it is the first block
                        ssd1306_flags |= SSD1306_FLAG_HAD_NO_BREAK_IN_LINE;
                    }
                }

                if (ch != ' ' || !(ssd1306_flags & SSD1306_FLAG_SKIP_LEADING_SPC)) {
                    // should not be, at this point we should have flushed the buffer
                    if (ssd1306_wrap_buff_pos >= SSD1306_WRAP_BUFFER_SIZE) {
                        ssd1306_wrap_buff_pos = SSD1306_WRAP_BUFFER_SIZE - 1;
                    }
                    ssd1306_wrap_buff[ssd1306_wrap_buff_pos++] = ch;
                }

                skip = 1;
            }
        } else {
            if (ch == ' ' && (ssd1306_flags & SSD1306_FLAG_SKIP_LEADING_SPC)) {
                skip = 1;
            }
        }
    }

    if (!skip) {
        ssd1306_putChWithMinMax(ch);
    }
#ifdef SERIAL_DEBUG_GFX_STATS
    uint32_t end = micros();
    ssd1306_putc_total += end - start;
    ssd1306_putc_count++;
#endif
}

static int16_t sx;
static int8_t sy;

void ssd_1306_start_text_bounds() {
    sx = ssd1306_cursor_x;
    sy = ssd1306_cursor_y;
    ssd1306_max_x = INT16_MIN;
    ssd1306_min_x = INT16_MAX;
    ssd1306_max_y = INT8_MIN;
    ssd1306_min_y = INT8_MAX;

    ssd1306_flags |= SSD1306_FLAG_SIMULATED_PRINT;
}

void ssd_1306_end_text_bounds(coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH) {
    ssd1306_flush_wrap_chars();

    ssd1306_cursor_x = sx;
    ssd1306_cursor_y = sy;

    if (pX0) *pX0 = ssd1306_cursor_x;
    if (pY0) *pY0 = ssd1306_cursor_y;
    if (pW) *pW = ssd1306_max_x - ssd1306_min_x;
    if (pH) *pH = ssd1306_max_y - ssd1306_min_y;

    ssd1306_flags &= ~SSD1306_FLAG_SIMULATED_PRINT;
};

void ssd1306_get_text_bounds(PGM_P s, coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH) {
    ssd_1306_start_text_bounds();
    ssd1306_fputs_P(s);
    ssd_1306_end_text_bounds(pX0, pY0, pW, pH);
}

void ssd1306_print_value(uint8_t flags, int32_t value, uint16_t valueDivider, PGM_P suffix) {
    uint8_t textSizeFlags = ssd1306_text_flags;

    if (someSet(flags, PV_2X_SIZE | PV_2X_SIZE_UNITS_ONLY)) ssd1306_set_text_size_flags(SSD1306_TEXT_FLAG_DOUBLE_SIZE);
    int32_t valueUnits = value / valueDivider;

    if (value > 0 && someSet(flags, PV_ALWAYS_PRINT_SIGN)) {
        ssd1306_putc('+');
    }

    if (!valueUnits && value < 0) {
        ssd1306_putc('-');
    }

    ssd1306_print_int32(valueUnits);
    uint8_t valueDecimals = flags & PV_DECIMALS;

    if (valueDecimals) {
        uint16_t decimalDigits = 0;
        PGM_P prefix = PSTR(".");

        if (value < 0) {
            value = -value;
        }

        if (valueDecimals == 1) {
            decimalDigits = (value * 10 / valueDivider) % 10;
        } else if (valueDecimals == 2) {
            decimalDigits = (value * 100 / valueDivider) % 100;
            if (decimalDigits < 10) {
                prefix = PSTR(".0");
            }
        } else {
            decimalDigits = (value * 1000 / valueDivider) % 1000;
            if (decimalDigits < 10) {
                prefix = PSTR(".00");
            } else if (decimalDigits < 100) {
                prefix = PSTR(".0");
            }
        }

        if (someSet(flags, PV_2X_SIZE_UNITS_ONLY)) ssd1306_set_text_size_flags(0);
        ssd1306_fputs_P(prefix);
        ssd1306_print_uint16(decimalDigits);
    }

    if (someSet(flags, PV_SPACE_SUFFIX)) ssd1306_move_x_by(ssd1306_char_x_size);
    if (someSet(flags, PV_THIRD_SPACE_SUFFIX)) ssd1306_move_x_by((coord_x) (ssd1306_char_x_size / 3));
    if (someSet(flags, PV_HALF_SPACE_SUFFIX)) ssd1306_move_x_by((coord_x) (ssd1306_char_x_size / 2));

    if (suffix) {
        if (someSet(flags, PV_2X_SIZE_UNITS_ONLY)) ssd1306_set_text_size_flags(SSD1306_TEXT_FLAG_DOUBLE_SIZE);
        ssd1306_fputs_P((PGM_P) suffix);
    }

    ssd1306_set_text_size_flags(textSizeFlags);
}

void ssd1306_print_centered_P(PGM_P message, uint8_t flags) {
    uint8_t w, h;
    coord_x x;
    coord_y y;

    ssd1306_get_text_bounds((PGM_P) message, &x, &y, &w, &h);

    if (someSet(flags, PC_CENTERED_Y) && DISPLAY_YSIZE > y + h) {
        ssd1306_move_y_by((coord_y) ((DISPLAY_YSIZE - y - h) / 2));
    }
    if (someSet(flags, PC_CENTERED_X) && DISPLAY_XSIZE > x + w) {
        ssd1306_move_x_by((coord_x) ((DISPLAY_XSIZE - x - w) / 2));
    }

    ssd1306_fputs_P((PGM_P) message);
}


#ifdef CONSOLE_DEBUG

ssd1306_display_buffer_t ssd1306_display_data;

void ssd1306_display() {
    char test_displayBuffer[CONSOLE_DISPLAY_BUFFER_SIZE];
    ssd1306_display_to_str(test_displayBuffer);
    puts(test_displayBuffer);
};

void ssd1306_display_to_str(char *s) {
    ssd1306_flush_wrap_chars();

    *s++ = '/';
    for (int i = 0; i < DISPLAY_XSIZE; i++) {
        *s++ = '-';
    }
    *s++ = '\\';
    *s++ = '\n';

    for (int pg = 0; pg < DISPLAY_YSIZE / 8; pg++) {
        for (int b = 1; b < 256; b *= 2) {
            *s++ = '|';
            for (int c = 0; c < DISPLAY_XSIZE; c++) {
                *s++ = ssd1306_display_buffer[pg][c] & b ? '@' : ' ';
            }
            *s++ = '|';
            *s++ = '\n';
        }
    }

    *s++ = '\\';
    for (int i = 0; i < DISPLAY_XSIZE; i++) {
        *s++ = '-';
    }
    *s++ = '/';
    *s++ = '\n';

    *s++ = '\0';
};

#endif

