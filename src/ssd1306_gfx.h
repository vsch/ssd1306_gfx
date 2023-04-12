//
// Created by Vladimir Schneider on 2023-03-08.
//

#ifndef SSD1306_SSD1306_GFX_H
#define SSD1306_SSD1306_GFX_H

#include <stddef.h> //uint8_t type
#include <stdbool.h> //uint8_t type
#include <stdio.h> //uint8_t type

#ifdef __cplusplus
extern "C" {
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

#ifndef GFX_OLED_TYPE
#define GFX_OLED_TYPE 91
#endif

#ifndef SSD1306_CONFIG_FLAGS
#define SSD1306_CONFIG_FLAGS SSD1306_SWITCHCAPVCC
#endif

// this adds about 650 bytes to flash use and 7 bytes of RAM, but more than doubles character output to screen
#ifndef GFX_NO_BIT_BLIT
#define GFX_BIT_BLIT
#endif

#ifndef CONSOLE_DEBUG
#ifdef __cplusplus
typedef const __FlashStringHelper *PGM_STR;
#endif

#else
typedef const char *PGM_STR;

typedef const char *PGM_P;
typedef const char *PGM_STR;
#define PSTR(s)  s
#endif

// adds 646 bytes of FLASH use. Adds smoother double size chars for '0'-'9', 'm', 'l', and '.'
//#define GFX_LARGE_FONT

// adds 372 bytes of FLASH use. Adds 3x6 chars for '0'-'9', 'm', 'l', and '.'
//#define GFX_SMALL_FONT

#if defined(GFX_SMALL_FONT) || defined(GFX_LARGE_FONT)
#define GFX_HAVE_ALT_FONTS
#endif

typedef int16_t coord_x;
typedef int8_t coord_y;

// 128x64 display constants
#define GFX_096_XSIZE   128
#define GFX_096_YSIZE   64
#define GFX_096_XOFFSET 0
#define GFX_096_YOFFSET 0

// 128x32 display constants
#define GFX_091_XSIZE   128
#define GFX_091_YSIZE   32
#define GFX_091_XOFFSET 0
#define GFX_091_YOFFSET 0

#define GFX_FONT_X_PIXELS 5
#define GFX_FONT_Y_ROWS 1
#define GFX_FONT_Y_PIXELS 8
#define GFX_CHAR_WIDTH 6
#define GFX_CHAR_HEIGHT 8

extern const uint8_t FONT_CHARS[96][GFX_FONT_X_PIXELS] PROGMEM;

#if GFX_OLED_TYPE == 96
#define DISPLAY_XSIZE   GFX_096_XSIZE
#define DISPLAY_YSIZE   GFX_096_YSIZE
#define DISPLAY_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#elif GFX_OLED_TYPE == 91
#define DISPLAY_XSIZE   GFX_091_XSIZE
#define DISPLAY_YSIZE   GFX_091_YSIZE
#define DISPLAY_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#else
#error "Unsupported Display Type "
#endif

#define CHAR_WIDTH GFX_CHAR_WIDTH
#define CHAR_HEIGHT GFX_CHAR_HEIGHT

typedef uint8_t color_t;

// Color constants
#define GFX_COLOR_NONE      0  // leave as is
#define GFX_COLOR_BLACK     1  // black or off
#define GFX_COLOR_WHITE     2  // white or on
#define GFX_COLOR_INVERT    3  // invert

#define COLOR_ON_COLOR(f, b)     (((b) & 3) | (((f) & 3) << 2))
#define COLOR_ON_COLOR_FOREGROUND(c)     (((c) >> 2) & 3)
#define COLOR_ON_COLOR_BACKGROUND(c)     ((c) & 3)

#define GFX_BITS_DASH_NONE           0b1
#define GFX_SIZE_DASH_NONE           1
#define GFX_BITS_DOT                 0b01
#define GFX_SIZE_DOT                 2
#define GFX_BITS_SPARSE_DOT          0b001
#define GFX_SIZE_SPARSE_DOT          3
#define GFX_BITS_DASH                0b0011
#define GFX_SIZE_DASH                4
#define GFX_BITS_LONG_DASH           0b00111
#define GFX_SIZE_LONG_DASH           5
#define GFX_BITS_DASH_DOT            0b01011
#define GFX_SIZE_DASH_DOT            5

#define LINE_PATTERN(b, s)  ((s) << 8 | (((b) & 0xff)))
#define GET_DASH_BITS(p) ((p) & 0xff)
#define GET_DASH_SIZE(p) ((p) >> 8)

#define X_COORDS_VISIBLE(x0, x1)   ((x1) >= 0 && (x0) < DISPLAY_XSIZE)
#define X_COORDS_CLIPPED(x0, x1)   ((x0) < 0 || (x1) >= DISPLAY_XSIZE)

#ifdef GFX_PAGED_UPDATES

#ifndef GFX_UPDATE_PAGES
#define GFX_UPDATE_PAGES 1
#endif

extern coord_y gfx_update_page_y0; // this is update page * 8
extern coord_y gfx_update_page_y1; // this is (update page+GFX_UPDATE_PAGES)*8

extern void gfx_start_next_page();

// Y valid when in [gfx_update_page*8, (gfx_update_page+GFX_UPDATE_PAGES)*8)
//  000pp000 to 00qq000 where pp is gfx_update_page and qq is gfx_update_page+GFX_UPDATE_PAGES
#define Y_COORD_DISPLAY_Y0   gfx_update_page_y0
#define Y_COORD_DISPLAY_Y1   gfx_update_page_y1
#define COORDS_IN_DISPLAY(x, y)   (!((x) & ~(DISPLAY_XSIZE - 1)) && (y) >= Y_COORD_DISPLAY_Y0 && (y) < Y_COORD_DISPLAY_Y1)
#else

#define GFX_UPDATE_PAGES (DISPLAY_YSIZE / 8)

#define Y_COORD_DISPLAY_Y0   0
#define Y_COORD_DISPLAY_Y1   DISPLAY_YSIZE

// HACK: Only works if sizes are powers of two
#define COORDS_IN_DISPLAY(x, y)   (!((x) & ~(DISPLAY_XSIZE - 1)) && !((y) & ~(DISPLAY_YSIZE - 1)))
#endif // GFX_PAGED_UPDATES

#ifndef CONSOLE_DEBUG
#define GFX_BUFFER_UPDATE_PAGES GFX_UPDATE_PAGES
#else
#define GFX_BUFFER_UPDATE_PAGES (DISPLAY_YSIZE / 8)
#endif // CONSOLE_DEBUG

#define Y_COORD_IN_DISPLAY_Y0(y)   ((y) >= Y_COORD_DISPLAY_Y0)
#define Y_COORD_IN_DISPLAY_Y1(y)   ((y) < Y_COORD_DISPLAY_Y1)
#define Y_COORD_IN_DISPLAY(y)      (Y_COORD_IN_DISPLAY_Y0(y) && Y_COORD_IN_DISPLAY_Y1(y))
#define Y_COORDS_VISIBLE(y0, y1)   (Y_COORD_IN_DISPLAY_Y0(y1) && Y_COORD_IN_DISPLAY_Y1(y0))
#define Y_COORDS_CLIPPED(y0, y1)   (!(Y_COORD_IN_DISPLAY_Y0(y0) && Y_COORD_IN_DISPLAY_Y1(y1)))

#ifndef CONSOLE_DEBUG
#define Y_TO_PAGE(y)  (((y) - Y_COORD_DISPLAY_Y0) >> 3)
#else
// for testing, update the right location in the display buffer as if not paged
#define Y_TO_PAGE(y)  ((y) >> 3)
#endif // CONSOLE_DEBUG

#define CLIP_TO_UPDATE_PAGE_Y0(y)  ((y) < Y_COORD_DISPLAY_Y0 ? Y_COORD_DISPLAY_Y0 : (y))
#define CLIP_TO_UPDATE_PAGE_Y1(y)  ((y) >= Y_COORD_DISPLAY_Y1 ? Y_COORD_DISPLAY_Y1 : (y))
#define CLIP_TO_UPDATE_PAGE(y)  ((y) >= Y_COORD_DISPLAY_Y1 ? Y_COORD_DISPLAY_Y1 : (y) <= Y_COORD_DISPLAY_Y0 ? Y_COORD_DISPLAY_Y0 : (y))

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
#define GFX_OCT_1           0x01
#define GFX_OCT_2           0x02
#define GFX_OCT_3           0x04
#define GFX_OCT_4           0x08
#define GFX_OCT_5           0x10
#define GFX_OCT_6           0x20
#define GFX_OCT_7           0x40
#define GFX_OCT_8           0x80
#define GFX_OCT_ALL         0xff

#define GFX_QUAD_1          (GFX_OCT_1 | GFX_OCT_2)
#define GFX_QUAD_2          (GFX_OCT_3 | GFX_OCT_4)
#define GFX_QUAD_3          (GFX_OCT_5 | GFX_OCT_6)
#define GFX_QUAD_4          (GFX_OCT_7 | GFX_OCT_8)

#define GFX_QUADS_ALL       (GFX_QUAD_1 | GFX_QUAD_2 | GFX_QUAD_3 | GFX_QUAD_4)
#define GFX_OCT_NON_QUADS   (GFX_OCT_ALL & ~GFX_QUADS_ALL)

// display internal state flags
#define GFX_FLAG_NONE  0
#define GFX_FLAG_CONTINUED_PATH             0x01  // cursor startPosition represents a filled pixel that start of lineTo should not draw
#define GFX_FLAG_SIMULATED_PRINT            0x02  // cursor startPosition represents a filled pixel that start of lineTo should not draw
#define GFX_FLAG_SKIP_LEADING_SPC           0x04  // text was wrapped, so now we skip spaces until non-space appears
#define GFX_FLAG_HAD_CHARS_ON_LINE          0x08  // printed some non spaces on the line before the accumulated chars in the buffer
#define GFX_FLAG_HAD_NO_BREAK_IN_LINE       0x10  // flushed buffer had no spaces
#define GFX_FLAG_WRAP_ON_SPC                0x20  // wrap strings on previous space if non-space chars do not fit
#define GFX_FLAG_BORDER_LINE                0x40  // set when char needs top border because it is the first line of text
#define GFX_FLAG_BORDER_CHAR                0x80  // set when char needs left border because it is first in line

#define GFX_TEXT_FLAG_BORDER                0x01  // add left and top text borders in background color
#define GFX_TEXT_FLAG_DOUBLE_WIDTH          0x02
#define GFX_TEXT_FLAG_DOUBLE_HEIGHT         0x04
#define GFX_TEXT_FLAG_WRAP                  0x08  // wrap text for any characters outside print area
#define GFX_TEXT_FLAG_SMALL_FONT            0x10  // half size reduced font

#define GFX_TEXT_FLAG_DOUBLE_SIZE (GFX_TEXT_FLAG_DOUBLE_WIDTH | GFX_TEXT_FLAG_DOUBLE_HEIGHT)
#define GFX_TEXT_FLAG_CHAR_SIZE (GFX_TEXT_FLAG_DOUBLE_SIZE | GFX_TEXT_FLAG_SMALL_FONT)

#ifdef GFX_SMALL_FONT
#define GFX_SMALL_X_PIXELS 3
#define GFX_SMALL_Y_ROWS 1
#define GFX_SMALL_Y_PIXELS 6
#define GFX_SMALL_CHAR_WIDTH 4
#define GFX_SMALL_CHAR_HEIGHT 6
#define GFX_SMALL_FONT_SIZE 17

extern const uint8_t SMALL_CHARS[17][GFX_SMALL_X_PIXELS * GFX_SMALL_Y_ROWS] PROGMEM;
extern const char SMALL_CHAR_MAP[] PROGMEM;
extern const uint8_t SMALL_CHAR_SET[] PROGMEM;
#endif // GFX_SMALL_FONT

#ifdef GFX_LARGE_FONT
#define GFX_LARGE_X_PIXELS 10
#define GFX_LARGE_Y_ROWS 2
#define GFX_LARGE_Y_PIXELS 16
#define GFX_LARGE_CHAR_WIDTH 12
#define GFX_LARGE_CHAR_HEIGHT 16
#define GFX_LARGE_FONT_SIZE 13

extern const uint8_t LARGE_CHARS[13][GFX_LARGE_X_PIXELS * GFX_LARGE_Y_ROWS] PROGMEM;
extern const char LARGE_CHAR_MAP[] PROGMEM;
extern const uint8_t LARGE_CHAR_SET[] PROGMEM;
#endif // GFX_LARGE_FONT

// stream outputs to ssd1306 display
extern FILE gfx_out; // NOLINT(misc-non-copyable-objects)

// initialization
extern void gfx_init_display(uint8_t contrast);
// display functions
extern void gfx_set_inverted();
extern void gfx_clear_inverted();
extern void gfx_display_off();
extern void gfx_display_on();
extern void gfx_set_contrast(uint8_t contrast);
extern void gfx_display();

// graphics functions/variables
extern uint8_t gfx_flags;           // option flags
extern uint8_t gfx_text_flags;       // text option flags
extern int8_t gfx_char_x_size;          // char width updated when text size flags change
extern int8_t gfx_char_y_size;          // char height updated when text size flags change

extern coord_x gfx_cursor_x; // cursor
extern coord_x gfx_max_x; // max X value when printing a string and wrapping
extern coord_x gfx_min_x; // max X value when printing a string and wrapping
extern coord_x gfx_margin_left; // left margin for text wrapping, normally 0, cX will be set to this on wrapping
extern coord_x gfx_margin_right; // right margin for text wrapping, normally DISPLAY_XSIZE
extern coord_y gfx_cursor_y; // cursor
extern coord_y gfx_max_y; // max Y value when printing a string and wrapping
extern coord_y gfx_min_y; // max Y value when printing a string and wrapping

#define GFX_WRAP_BUFFER_SIZE (((DISPLAY_XSIZE)+(CHAR_WIDTH)-1) / (CHAR_WIDTH))

extern char gfx_wrap_buff[GFX_WRAP_BUFFER_SIZE]; // characters buffered when looking for space on which to possibly break
extern uint8_t gfx_wrap_buff_pos; // next character in buffer

extern color_t gfx_fore_color;
extern color_t gfx_back_color;

#ifndef GFX_NO_LINE_PATTERNS
#define GFX_DASH_NONE        LINE_PATTERN(GFX_BITS_DASH_NONE, GFX_SIZE_DASH_NONE)
#define GFX_DOT              LINE_PATTERN(GFX_BITS_DOT, GFX_SIZE_DOT)
#define GFX_SPARSE_DOT       LINE_PATTERN(GFX_BITS_SPARSE_DOT, GFX_SIZE_SPARSE_DOT)
#define GFX_DASH             LINE_PATTERN(GFX_BITS_DASH, GFX_SIZE_DASH)
#define GFX_LONG_DASH        LINE_PATTERN(GFX_BITS_LONG_DASH, GFX_SIZE_LONG_DASH)
#define GFX_DASH_DOT         LINE_PATTERN(GFX_BITS_DASH_DOT, GFX_SIZE_DASH_DOT)

extern color_t gfx_gap_color;            // dash gap color
extern uint8_t gfx_dash_bits;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t gfx_dash_size;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t gfx_dashOffset;          // solid/dash/dot pattern for line outlines (not text)
#endif // GFX_NO_LINE_PATTERNS

typedef struct gfx_display_buffer {
    const uint8_t twi_data;
    uint8_t display_buffer[GFX_BUFFER_UPDATE_PAGES][DISPLAY_XSIZE];
} gfx_display_buffer_t;

extern gfx_display_buffer_t gfx_display_data;
#define gfx_display_buffer gfx_display_data.display_buffer

// display functions
extern void gfx_clear_screen();

// user display operations
// primitives used in library
#ifndef GFX_NO_LINE_PATTERNS
extern color_t gfx_next_dash_color();
extern bool gfx_next_dash_bit();
extern void gfx_set_line_pattern(uint16_t pattern);
extern uint16_t gfx_get_line_pattern();
extern uint8_t gfx_have_line_pattern();
extern void gfx_hline_dashed(coord_x x0, coord_y y0, coord_x x1);
extern void gfx_vline_dashed(coord_x x0, coord_y y0, coord_y y1);
#else
#define gfx_next_dash_color()   gfx_fore_color
#define gfx_next_dash_bit()     1
#define gfx_have_line_pattern() 0
#define gfx_hline_dashed(x0, y0, x1) gfx_hline((x0), (y0), (x1), gfx_fore_color)
#define gfx_vline_dashed(x0, y0, y1) gfx_vline((x0), (y0), (y1), gfx_fore_color)
#endif // GFX_NO_LINE_PATTERNS

extern void gfx_set_pixel(uint8_t x, coord_y y, color_t color);
extern void gfx_draw_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs);
extern void gfx_fill_circle_octants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octants);

typedef void (*fp_circle_octants)(coord_x, coord_y, coord_x, coord_y, uint8_t);
extern void gfx_circle_octants(int8_t r, uint8_t octants, fp_circle_octants drawOctants);

extern void gfx_hline(coord_x x0, coord_y y0, coord_x x1, color_t color);
extern void gfx_vline(coord_x x0, coord_y y0, coord_y y1, color_t color);
extern void gfx_hline_to(coord_x x1);            // draw horizontal line using foreground
extern void gfx_vline_to(coord_y y1);            // draw vertical line using foreground
extern void gfx_line_to(coord_x x1, coord_y y1); // draw line using foreground
extern void gfx_double_line_to(coord_x x1, coord_y y1, int8_t xSpc, int8_t ySpc);
extern void gfx_double_hline_to(coord_x x1, int8_t ySpc);
extern void gfx_double_vline_to(coord_y y1, int8_t xSpc);

extern void gfx_rect(coord_x x1, coord_y y1);   // draw rectangle outline in foreground, fill in background

extern bool gfx_normalize_x(coord_x *pX1, coord_x *pX2);
extern bool gfx_normalize_y(coord_y *pY1, coord_y *pY2);

extern void gfx_circle(int8_t radius);         // draw circle centered on cursor, outline in foreground, fill in background
extern void gfx_round_rect(coord_x x1, coord_y y1, int8_t r, uint8_t octants);   // draw rounded rectangle outline in foreground, fill in background

extern void gfx_bitmap(const uint8_t bitmap[], uint8_t w, uint8_t h);
extern void gfx_clip_to_screen(coord_x *x, coord_y *y);

#define PB_NORMAL   0x00  // left to right, or bottom to top
#define PB_REVERSED 0x01  // right to left, or top to bottom
#define PB_VERTICAL 0x02

extern void gfx_progress_bar_to(uint8_t pbFlags, uint8_t progress, coord_x x1, coord_y y1, color_t undoneColor);
extern void gfx_print_display(FILE *stream);

#ifdef GFX_FANCY
extern void gfx_ellipse(int8_t width, int8_t height);         // draw ellipse centered on cursor, outline in foreground, fill in background
#endif

extern void gfx_update_char_size();
extern void gfx_flush_wrap_chars();
extern void gfx_set_text_size_flags(uint8_t flags); // double wide and/or double height
extern void gfx_set_text_flags(uint8_t flags); // double wide and/or double height
extern void gfx_clear_text_flags(uint8_t flags);   // text flags
extern void gfx_set_text_col_row(int8_t x, coord_y y);
extern bool gfx_is_char_visible();
extern bool gfx_is_char_clipped();

#ifdef SERIAL_DEBUG_GFX_STATS
extern uint32_t gfx_put_ch_total;
extern uint16_t gfx_put_ch_count;
extern uint32_t gfx_put_ch_lp_total;
extern uint32_t gfx_put_ch_lp_count;
extern uint32_t gfx_put_pixel_total;
extern uint32_t gfx_put_pixel_count;
extern uint32_t gfx_putc_total;
extern uint16_t gfx_putc_count;
#endif

#ifdef GFX_BIT_BLIT
extern void gfx_bitBlit(const uint8_t *pData, uint8_t xSize, uint8_t ySize);
#endif

extern bool gfx_put_ch(char ch);
extern void gfx_new_line();
extern void gfx_putc(char ch);
extern void gfx_putnc(char ch, uint8_t n);
extern void gfx_put_dig(uint8_t dig);
extern void gfx_fputs(const char *str);
extern void gfx_fputs_P(PGM_P str);
extern void gfx_fputs_lpad(const char *str, char ch, uint8_t pad);
extern void gfx_fputs_lpad_P(PGM_P str, char ch, uint8_t pad);
extern int gfx_printf(const char *fmt, ...);
extern int gfx_printf_P(PGM_P fmt, ...);
extern void gfx_print_float(double number, uint8_t digits);
extern void gfx_print_int32_lpad(int32_t n, uint8_t radix, uint8_t pad, char ch);
extern void gfx_print_int32(int32_t i);
extern void gfx_print_uint32_lpad(uint32_t i, uint8_t radix, uint8_t pad, char ch);
extern void gfx_print_uint32(uint32_t i);
extern void gfx_print_int16_lpad(int16_t i, uint8_t radix, uint8_t pad, char ch);
extern void gfx_print_int16(int16_t i);
extern void gfx_print_uint16(uint16_t i);
extern void gfx_print_uint16_lpad(uint16_t i, uint8_t radix, uint8_t pad, char ch);
extern void gfx_print_int8_lpad(int8_t i, uint8_t radix, uint8_t pad, char ch);
extern void gfx_print_int8(int8_t i);
extern void gfx_print_uint8(uint8_t i);
extern void gfx_print_uint8_lpad(uint8_t i, uint8_t radix, uint8_t pad, char ch);

// text bound helpers, put print and cursor moves between them to get the bounds
extern void gfx_start_text_bounds();
extern void gfx_end_text_bounds(coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH);
extern void gfx_get_text_bounds(PGM_P s, coord_x *pX0, coord_y *pY0, uint8_t *pW, uint8_t *pH); // for simplicity of a single string

// all text output between, is treated as a single string for wrapping on space purposes
extern void gfx_start_text_spc_wrap();
extern void gfx_end_text_spc_wrap();

#ifdef GFX_BIT_BLIT
extern uint16_t gfx_double_bits(uint8_t b);
extern void gfx_double_width(uint8_t *pData, uint8_t xSize);
extern void gfx_double_height(uint8_t *pData, uint8_t xSize);
#endif

extern const char spcString[] PROGMEM;

#define PV_DECIMALS           0x03
#define PV_2X_SIZE            0x04
#define PV_2X_SIZE_UNITS_ONLY 0x08
#define PV_ALWAYS_PRINT_SIGN  0x10
#define PV_SPACE_SUFFIX       0x20   // if suffix is given then put full space before it, can be combined with others to add more spacing
#define PV_HALF_SPACE_SUFFIX  0x40   // if suffix is given then put half space before it, can be combined with others to add more spacing
#define PV_THIRD_SPACE_SUFFIX 0x80   // if suffix is given then put a third of space before it, can be combined with others to add more spacing
extern void gfx_print_value(uint8_t flags, int16_t value, int16_t valueDivider, PGM_P suffix);

#define PC_CENTERED_Y 1
#define PC_CENTERED_X 2
extern void gfx_print_centered_P(PGM_P message, uint8_t flags);

#ifdef CONSOLE_DEBUG
#define CONSOLE_DISPLAY_BUFFER_SIZE ((DISPLAY_XSIZE + 3) * (DISPLAY_YSIZE + 2) + 1)
extern void gfx_display_to_str(char *s);
#endif // CONSOLE_DEBUG

#ifdef __cplusplus
}
#endif

#endif //SSD1306_SSD1306_GFX_H
