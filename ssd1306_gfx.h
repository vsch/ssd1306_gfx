//
// Created by Vladimir Schneider on 2023-03-08.
//

#ifndef SSD1306_SSD1306_GFX_H
#define SSD1306_SSD1306_GFX_H

#include <stddef.h> //uint8_t type
#include <stdbool.h> //uint8_t type
#include <stdio.h> //uint8_t type
#include "ssd1306_display.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SSD1306_SMALL_FONT) || defined(SSD1306_LARGE_FONT)
#define SSD1306_HAVE_ALT_FONTS
#endif

inline uint8_t someSet(uint16_t flags, uint16_t mask) {
    return (flags & mask);
}

inline uint8_t allSet(uint16_t flags, uint16_t mask) {
    return !(~flags & mask);
}

inline uint8_t noneSet(uint16_t flags, uint16_t mask) {
    return !(flags & mask);
}

// 128x64 display constants
#define SSD1306_096_XSIZE   128
#define SSD1306_096_YSIZE   64
#define SSD1306_096_XOFFSET 0
#define SSD1306_096_YOFFSET 0

// 128x32 display constants
#define SSD1306_091_XSIZE   128
#define SSD1306_091_YSIZE   32
#define SSD1306_091_XOFFSET 0
#define SSD1306_091_YOFFSET 0

#define SSD1306_FONT_X_PIXELS 5
#define SSD1306_FONT_Y_ROWS 1
#define SSD1306_FONT_Y_PIXELS 8
#define SSD1306_CHAR_WIDTH 6
#define SSD1306_CHAR_HEIGHT 8

extern const uint8_t FONT_CHARS[96][SSD1306_FONT_X_PIXELS] PROGMEM;

#ifndef SSD1306_OLED_TYPE
#define SSD1306_OLED_TYPE 91
#endif

#if SSD1306_OLED_TYPE == 96
#define DISPLAY_XSIZE   SSD1306_096_XSIZE
#define DISPLAY_YSIZE   SSD1306_096_YSIZE
#define DISPLAY_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#elif SSD1306_OLED_TYPE == 91
#define DISPLAY_XSIZE   SSD1306_091_XSIZE
#define DISPLAY_YSIZE   SSD1306_091_YSIZE
#define DISPLAY_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#else
#error "Unsupported Display Type "
#endif

#define CHAR_WIDTH SSD1306_CHAR_WIDTH
#define CHAR_HEIGHT SSD1306_CHAR_HEIGHT

typedef uint8_t color_t;

// Color constants
#define SSD1306_COLOR_NONE      0  // leave as is
#define SSD1306_COLOR_BLACK     1  // black or off
#define SSD1306_COLOR_WHITE     2  // white or on
#define SSD1306_COLOR_INVERT    3  // invert

#define COLOR_ON_COLOR(f, b)     (((b) & 3) | (((f) & 3) << 2))
#define COLOR_ON_COLOR_FOREGROUND(c)     (((c) >> 2) & 3)
#define COLOR_ON_COLOR_BACKGROUND(c)     ((c) & 3)

#define SSD1306_BITS_DASH_NONE           0b1
#define SSD1306_SIZE_DASH_NONE           1
#define SSD1306_BITS_DOT                 0b01
#define SSD1306_SIZE_DOT                 2
#define SSD1306_BITS_SPARSE_DOT          0b001
#define SSD1306_SIZE_SPARSE_DOT          3
#define SSD1306_BITS_DASH                0b0011
#define SSD1306_SIZE_DASH                4
#define SSD1306_BITS_LONG_DASH           0b00111
#define SSD1306_SIZE_LONG_DASH           5
#define SSD1306_BITS_DASH_DOT            0b01011
#define SSD1306_SIZE_DASH_DOT            5

#define LINE_PATTERN(b, s)  ((s) << 8 | (((b) & 0xff)))
#define GET_DASH_BITS(p) ((p) & 0xff)
#define GET_DASH_SIZE(p) ((p) >> 8)

// HACK: Only works if sizes are powers of two
#define COORDS_IN_DISPLAY(x, y)   (!((x) & ~(DISPLAY_XSIZE - 1)) && !((y) & ~(DISPLAY_YSIZE - 1)))

#define SSD1306_DASH_NONE        LINE_PATTERN(SSD1306_BITS_DASH_NONE, SSD1306_SIZE_DASH_NONE)
#define SSD1306_DOT              LINE_PATTERN(SSD1306_BITS_DOT, SSD1306_SIZE_DOT)
#define SSD1306_SPARSE_DOT       LINE_PATTERN(SSD1306_BITS_SPARSE_DOT, SSD1306_SIZE_SPARSE_DOT)
#define SSD1306_DASH             LINE_PATTERN(SSD1306_BITS_DASH, SSD1306_SIZE_DASH)
#define SSD1306_LONG_DASH        LINE_PATTERN(SSD1306_BITS_LONG_DASH, SSD1306_SIZE_LONG_DASH)
#define SSD1306_DASH_DOT         LINE_PATTERN(SSD1306_BITS_DASH_DOT, SSD1306_SIZE_DASH_DOT)

// octants for circle arcs are:
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
// octants for rects and rounded rects are:
// 128            1              2
//    /-------------------------\
//    |                          |
//    |                          |
// 64 |                          | 4
//    |                          |
//    |                          |
//    \-------------------------/
//  32            16              8
//
#define SSD1306_OCT_1           0x01
#define SSD1306_OCT_2           0x02
#define SSD1306_OCT_3           0x04
#define SSD1306_OCT_4           0x08
#define SSD1306_OCT_5           0x10
#define SSD1306_OCT_6           0x20
#define SSD1306_OCT_7           0x40
#define SSD1306_OCT_8           0x80
#define SSD1306_OCT_ALL         0xff

#define SSD1306_QUAD_1          (SSD1306_OCT_1 | SSD1306_OCT_2)
#define SSD1306_QUAD_2          (SSD1306_OCT_3 | SSD1306_OCT_4)
#define SSD1306_QUAD_3          (SSD1306_OCT_5 | SSD1306_OCT_6)
#define SSD1306_QUAD_4          (SSD1306_OCT_7 | SSD1306_OCT_8)

#define SSD1306_QUADS_ALL       (SSD1306_QUAD_1 | SSD1306_QUAD_2 | SSD1306_QUAD_3 | SSD1306_QUAD_4)
#define SSD1306_OCT_NON_QUADS   (SSD1306_OCT_ALL & ~SSD1306_QUADS_ALL)

// display flags
#define SSD1306_FLAG_NONE  0
#define SSD1306_FLAG_CONTINUED_PATH             0x01  // cursor startPosition represents a filled pixel that start of lineTo should not draw
#define SSD1306_FLAG_SIMULATED_PRINT            0x02  // cursor startPosition represents a filled pixel that start of lineTo should not draw
#define SSD1306_FLAG_SKIP_LEADING_SPC           0x04  // text was wrapped, so now we skip spaces until non-space appears
#define SSD1306_FLAG_HAD_CHARS_ON_LINE          0x08  // printed some non spaces on the line before the accumulated chars in the buffer
#define SSD1306_FLAG_HAD_NO_BREAK_IN_LINE       0x10  // flushed buffer had no spaces

#define SSD1306_TEXT_FLAG_BORDER                0x01  // add left and top text borders in background color
#define SSD1306_TEXT_FLAG_DOUBLE_WIDTH          0x02
#define SSD1306_TEXT_FLAG_DOUBLE_HEIGHT         0x04
#define SSD1306_TEXT_FLAG_BORDER_LINE           0x08  // set when char needs top border because it is the first line of text
#define SSD1306_TEXT_FLAG_BORDER_CHAR           0x10  // set when char needs left border because it is first in line
#define SSD1306_TEXT_FLAG_WRAP                  0x20  // wrap text for any characters outside print area
#define SSD1306_TEXT_FLAG_WRAP_ON_SPC           0x40  // wrap strings on previous space if non-space chars do not fit
#define SSD1306_TEXT_FLAG_SMALL_FONT            0x80  // half size reduced font

#define SSD1306_TEXT_FLAG_DOUBLE_SIZE (SSD1306_TEXT_FLAG_DOUBLE_WIDTH | SSD1306_TEXT_FLAG_DOUBLE_HEIGHT)
#define SSD1306_TEXT_FLAG_CHAR_SIZE (SSD1306_TEXT_FLAG_DOUBLE_SIZE | SSD1306_TEXT_FLAG_SMALL_FONT)

#ifdef SSD1306_SMALL_FONT
#define SSD1306_SMALL_X_PIXELS 3
#define SSD1306_SMALL_Y_ROWS 1
#define SSD1306_SMALL_Y_PIXELS 6
#define SSD1306_SMALL_CHAR_WIDTH 4
#define SSD1306_SMALL_CHAR_HEIGHT 6
#define SSD1306_SMALL_FONT_SIZE 17

extern const uint8_t SMALL_CHARS[17][SSD1306_SMALL_X_PIXELS * SSD1306_SMALL_Y_ROWS] PROGMEM;
extern const char SMALL_CHAR_MAP[] PROGMEM;
extern const uint8_t SMALL_CHAR_SET[] PROGMEM;
#endif // SSD1306_SMALL_FONT

#ifdef SSD1306_LARGE_FONT
#define SSD1306_LARGE_X_PIXELS 10
#define SSD1306_LARGE_Y_ROWS 2
#define SSD1306_LARGE_Y_PIXELS 16
#define SSD1306_LARGE_CHAR_WIDTH 12
#define SSD1306_LARGE_CHAR_HEIGHT 16
#define SSD1306_LARGE_FONT_SIZE 13

extern const uint8_t LARGE_CHARS[13][SSD1306_LARGE_X_PIXELS * SSD1306_LARGE_Y_ROWS] PROGMEM;
extern const char LARGE_CHAR_MAP[] PROGMEM;
extern const uint8_t LARGE_CHAR_SET[] PROGMEM;
#endif // SSD1306_LARGE_FONT

// config flag values
#ifndef SSD1306_SWITCHCAPVCC
#define SSD1306_SWITCHCAPVCC    0x01 ///< Gen. display voltage from 3.3V, otherwise external vcc is used
#endif

// stream outputs to ssd1306 display
extern FILE ssd1306_out;

// initialization
extern void ssd1306_init_display();
// display functions
extern void ssd1306_set_inverted();
extern void ssd1306_clear_inverted();
extern void ssd1306_display_off();
extern void ssd1306_display_on();
extern void ssd1306_set_contrast(uint8_t contrast);
extern void ssd1306_display();

// graphics functions/variables
extern uint8_t ssd1306_flags;           // option flags
extern uint8_t ssd1306_text_flags;       // text option flags
extern int8_t ssd1306_char_x_size;          // char width updated when text size flags change
extern int8_t ssd1306_char_y_size;          // char height updated when text size flags change

extern coord_x ssd1306_cursor_x; // cursor
extern coord_x ssd1306_max_x; // max X value when printing a string and wrapping
extern coord_x ssd1306_min_x; // max X value when printing a string and wrapping
extern coord_x ssd1306_margin_left; // left margin for text wrapping, normally 0, cX will be set to this on wrapping
extern coord_x ssd1306_margin_right; // right margin for text wrapping, normally DISPLAY_XSIZE
extern coord_y ssd1306_cursor_y; // cursor
extern coord_y ssd1306_max_y; // max Y value when printing a string and wrapping
extern coord_y ssd1306_min_y; // max Y value when printing a string and wrapping

#define SSD1306_WRAP_BUFFER_SIZE (((DISPLAY_XSIZE)+(CHAR_WIDTH)-1) / (CHAR_WIDTH))

extern char ssd1306_wrap_buff[SSD1306_WRAP_BUFFER_SIZE]; // characters buffered when looking for space on which to possibly break
extern uint8_t ssd1306_wrap_buff_pos; // next character in buffer

extern color_t ssd1306_fore_color;
extern color_t ssd1306_gap_color;            // dash gap color
extern color_t ssd1306_back_color;

extern uint8_t ssd1306_dash_bits;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dash_size;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

typedef struct ssd1306_display_buffer {
    const uint8_t twi_data;
    uint8_t display_buffer[DISPLAY_YSIZE / 8][DISPLAY_XSIZE];
} ssd1306_display_buffer_t;

extern ssd1306_display_buffer_t ssd1306_display_data;
#define ssd1306_display_buffer ssd1306_display_data.display_buffer

// display functions
extern void ssd1306_clear_screen();

// user display operations
// primitives used in library
extern color_t ssd1306_next_dash_color();
extern bool ssd1306_next_dash_bit();
extern void ssd1306_set_line_pattern(uint16_t pattern);
extern uint16_t ssd1306_get_line_pattern();

extern void ssd1306_set_pixel(uint8_t x, coord_y y, color_t color);
extern void ssd1306_draw_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs);
extern void ssd1306_fill_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octants);

typedef void (*fp_circle_octants)(coord_x, coord_y, coord_x, coord_y, uint8_t);
extern void ssd1306_circle_octants(int8_t r, uint8_t octants, fp_circle_octants drawOctants);

extern void ssd1306_hline(coord_x x0, coord_y y0, coord_x x1, color_t color);
extern void ssd1306_hline_dashed(coord_x x0, coord_y y0, coord_x x1);
extern void ssd1306_vline(coord_x x0, coord_y y0, coord_y y1, color_t color);
extern void ssd1306_vline_dashed(coord_x x0, coord_y y0, coord_y y1);
extern void ssd1306_hline_to(coord_x x1);            // draw horizontal line using foreground
extern void ssd1306_vline_to(coord_y y1);            // draw vertical line using foreground
extern void ssd1306_line_to(coord_x x1, coord_y y1); // draw line using foreground
extern void ssd1306_double_line_to(coord_x x1, coord_y y1, int8_t xSpc, int8_t ySpc);
extern void ssd1306_double_hline_to(coord_x x1, int8_t ySpc);
extern void ssd1306_double_vline_to(coord_y y1, int8_t xSpc);

extern void ssd1306_rect(coord_x x1, coord_y y1);   // draw rectangle outline in foreground, fill in background

extern bool ssd1306_normalize_x(coord_x *pX1, coord_x *pX2);
extern bool ssd1306_normalize_y(coord_y *pY1, coord_y *pY2);

extern void ssd1306_circle(int8_t radius);         // draw circle centered on cursor, outline in foreground, fill in background
extern void ssd1306_round_rect(coord_x x1, coord_y y1, int8_t r, uint8_t octants);   // draw rounded rectangle outline in foreground, fill in background

extern void ssd1306_bitmap(const uint8_t bitmap[], uint8_t w, uint8_t h);

#ifdef SSD1306_FANCY
extern void ssd1306_ellipse(int8_t width, int8_t height);         // draw ellipse centered on cursor, outline in foreground, fill in background
#endif

extern void ssd1306_update_char_size();
extern void ssd1306_flush_wrap_chars();
extern void ssd1306_set_text_size_flags(uint8_t flags); // double wide and/or double height
extern void ssd1306_changing_text_flags(uint8_t flags);
extern void ssd1306_set_text_flags(uint8_t flags); // double wide and/or double height
extern void ssd1306_clear_text_flags(uint8_t flags);   // text flags
extern void ssd1306_set_text_col_row(int8_t x, coord_y y);
extern bool ssd1306_is_char_visible();
extern bool ssd1306_is_char_clipped();

extern void ssd1306_set_colors(color_t fg, color_t bg);
extern void ssd1306_set_pattern_colors(color_t fg, color_t bg, color_t gp);
extern void ssd1306_move_to(coord_x x, coord_y y);
extern void ssd1306_move_x_to(coord_x x);
extern void ssd1306_move_y_to(coord_y y);
extern void ssd1306_move_by(coord_x x, coord_y y);
extern void ssd1306_move_x_by(coord_x x);
extern void ssd1306_move_y_by(coord_y y);

#ifdef SERIAL_DEBUG_GFX_STATS
extern uint32_t ssd1306_put_ch_total;
extern uint16_t ssd1306_put_ch_count;
extern uint32_t ssd1306_put_ch_lp_total;
extern uint32_t ssd1306_put_ch_lp_count;
extern uint32_t ssd1306_put_pixel_total;
extern uint32_t ssd1306_put_pixel_count;
extern uint32_t ssd1306_putc_total;
extern uint16_t ssd1306_putc_count;
#endif

extern void ssd1306_bitBlit(const uint8_t *pData, uint8_t xSize, uint8_t ySize);
extern bool ssd1306_put_ch(char ch);
extern void ssd1306_new_line();
extern void ssd1306_putc(char ch);
extern void ssd1306_putnc(char ch, uint8_t n);
extern void ssd1306_put_dig(uint8_t dig);
extern void ssd1306_fputs(const char *str);
extern void ssd1306_fputs_P(PGM_P str);
extern void ssd1306_fputs_lpad(const char *str, char ch, uint8_t pad);
extern void ssd1306_fputs_lpad_P(PGM_P str, char ch, uint8_t pad);
extern void ssd1306_print_float(double number, uint8_t digits);
extern void ssd1306_print_int32_lpad(int32_t n, uint8_t radix, uint8_t pad, char ch);
extern void ssd1306_print_int32(int32_t i);
extern void ssd1306_print_uint32_lpad(uint32_t i, uint8_t radix, uint8_t pad, char ch);
extern void ssd1306_print_uint32(uint32_t i);
extern void ssd1306_print_int16_lpad(int16_t i, uint8_t radix, uint8_t pad, char ch);
extern void ssd1306_print_int16(int16_t i);
extern void ssd1306_print_uint16(uint16_t i);
extern void ssd1306_print_uint16_lpad(uint16_t i, uint8_t radix, uint8_t pad, char ch);
extern void ssd1306_print_int8_lpad(int8_t i, uint8_t radix, uint8_t pad, char ch);
extern void ssd1306_print_int8(int8_t i);
extern void ssd1306_print_uint8(uint8_t i);
extern void ssd1306_print_uint8_lpad(uint8_t i, uint8_t radix, uint8_t pad, char ch);

// text bound helpers, put print and cursor moves between them to get the bounds
extern void ssd_1306_start_text_bounds();
extern void ssd_1306_end_text_bounds(coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH);

// for simplicity of a single string
extern void ssd1306_get_text_bounds(PGM_P s, coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH);

#ifdef SSD1306_BIT_BLIT
extern uint16_t ssd1306_double_bits(uint8_t b);
extern void ssd1306_double_width(uint8_t *pData, uint8_t xSize);
extern void ssd1306_double_height(uint8_t *pData, uint8_t xSize);
#endif

extern const char spcString[] PROGMEM;

#define PV_DECIMALS           0x03
#define PV_2X_SIZE            0x04
#define PV_2X_SIZE_UNITS_ONLY 0x08
#define PV_ALWAYS_PRINT_SIGN  0x10
#define PV_SPACE_SUFFIX       0x20   // if suffix is given then put full space before it, can be combined with others to add more spacing
#define PV_HALF_SPACE_SUFFIX  0x40   // if suffix is given then put half space before it, can be combined with others to add more spacing
#define PV_THIRD_SPACE_SUFFIX 0x80   // if suffix is given then put a third of space before it, can be combined with others to add more spacing
extern void ssd1306_print_value(uint8_t flags, int32_t value, uint16_t valueDivider, PGM_P suffix);

#define PC_CENTERED_Y 1
#define PC_CENTERED_X 2
extern void ssd1306_print_centered_P(PGM_P message, uint8_t flags);

#ifdef CONSOLE_DEBUG
#define CONSOLE_DISPLAY_BUFFER_SIZE ((DISPLAY_XSIZE + 3) * (DISPLAY_YSIZE + 2) + 1)
    extern void ssd1306_display_to_str(char *s);
#endif // CONSOLE_DEBUG

#ifdef __cplusplus
    }
#endif

#endif //SSD1306_SSD1306_GFX_H
