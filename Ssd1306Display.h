#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
//
// Created by Vladimir Schneider on 2023-03-09.
//

#ifndef SSD1306_SSD1306DISPLAY_H
#define SSD1306_SSD1306DISPLAY_H

#include "ssd1306_display.h"
#include "ssd1306_gfx.h"

#ifndef DEC
#define DEC 10
#endif

#ifndef HEX
#define HEX 16
#endif

#ifndef OCT
#define OCT 8
#endif

#ifndef BIN
#define BIN 2
#endif

#define PROPERTY_GET(pType, pName, pVar) \
    inline pType get##pName() { return pVar; };

#define PROPERTY_SET(pType, pName, pVar) \
    inline void set##pName(pType value) { pVar = value; };

#define PROPERTY(pType, pName, pVar) \
    PROPERTY_GET(pType, pName, pVar) \
    PROPERTY_SET(pType, pName, pVar)

class Ssd1306Display {
public:
    // backward compatibility for basics
    inline bool begin(uint8_t switchVcc = 0, uint8_t i2caddr = 0) {
        initDisplay();
        return true;
    }

    inline void setTextColor(uint16_t c) {
        gfx_fore_color = c;
        gfx_back_color = GFX_COLOR_NONE;
    }

    // @formatter:off
    inline void setTextSize(uint8_t s) { gfx_set_text_size_flags(s == 2 ? GFX_TEXT_FLAG_DOUBLE_SIZE : 0); }
    inline void setCursor(int16_t x, int16_t y) { moveTo(x, y); }
    inline void setTextCursor(int8_t x, int8_t y) { moveToText(x, y); }
    inline void drawPixel(int16_t x, int16_t y, color_t color) { gfx_set_pixel(x, (coord_y) y, color); }
    inline int16_t getCursorX() { return gfx_cursor_x; };
    inline int8_t getCursorY() { return gfx_cursor_y; };
    // @formatter:on

    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void fillRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
    void fillRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
    void getTextBounds(PGM_STR s, int16_t x, int16_t y, int16_t *pX0, int16_t *pY0, uint16_t *pW, uint16_t *pH);
    void invertDisplay(bool val);
    void setTextColor(uint16_t c, uint16_t bg);

    // @formatter:off
    inline void startTextBounds() { gfx_start_text_bounds(); }
    inline void endTextBounds(int16_t *pX0, int8_t *pY0, uint8_t *pW, uint8_t *pH) { gfx_end_text_bounds(pX0, pY0, pW, pH); }
    inline void getTextBounds(PGM_STR s, int16_t *pX0, int8_t *pY0, uint8_t *pW, uint8_t *pH) { gfx_get_text_bounds((PGM_P) s, pX0, pY0, pW, pH); }
    // @formatter:on

    // @formatter:off
    inline void startTextWrapOnSpc() { gfx_start_text_spc_wrap(); }
    inline void endTextWrapOnSpc() { gfx_end_text_spc_wrap(); }
    // @formatter:on

    // C implementation to map
    PROPERTY(uint8_t, Flags, gfx_flags);           // option flags
    PROPERTY_GET(uint8_t, TextFlags, gfx_text_flags); // text option flags
    PROPERTY(int8_t, CharW, gfx_char_x_size);          // char width updated when text size flags change
    PROPERTY(int8_t, CharH, gfx_char_y_size);          // char height updated when text size flags change

    PROPERTY(coord_x, X, gfx_cursor_x); // cursor
    PROPERTY(coord_x, MaxX, gfx_max_x); // cursor
    PROPERTY(coord_x, LeftMargin, gfx_margin_left); // left margin for text wrapping, normally 0, cX will be set to this on wrapping
    PROPERTY(coord_x, RightMargin, gfx_margin_right); // right margin for text wrapping, normally DISPLAY_XSIZE
    PROPERTY(coord_y, Y, gfx_cursor_y); // cursor
    PROPERTY(coord_y, MaxY, gfx_max_y); // cursor

    PROPERTY(color_t, ForeColor, gfx_fore_color);

    PROPERTY(color_t, GapColor, gfx_gap_color);            // dash gap color
    PROPERTY(color_t, BackColor, gfx_back_color);

    PROPERTY(uint8_t, DashBits, gfx_dash_bits);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashSize, gfx_dash_size);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashOffset, gfx_dashOffset);          // solid/dash/dot pattern for line outlines (not text)

    // wrapper on C implementation
    // @formatter:off
    inline void initDisplay() { gfx_init_display(); }
    inline void displayOff() { gfx_display_off(); }
    inline void displayOn() { gfx_display_on(); }
    inline void clearScreen() { gfx_clear_screen(); }
    inline void clearDisplay() { gfx_clear_screen(); }
    inline void display() { gfx_display(); }
    inline void setInvertedDisplay() { gfx_set_inverted(); }
    inline void clearInvertedDisplay() { gfx_clear_inverted(); }
    inline void setContrast(uint8_t contrast) { gfx_set_contrast(contrast); }
    inline void setLinePattern(uint16_t pattern) { gfx_set_line_pattern(pattern); }
    inline uint16_t getLinePattern() { return gfx_get_line_pattern(); }

    inline uint8_t nextDashPixelColor() { return gfx_next_dash_color(); }
    inline uint8_t nextDashBit() { return gfx_next_dash_bit(); }
    inline void setPixel(coord_x x, coord_y y, color_t color) { gfx_set_pixel(x, y, color); }
    inline void moveToText(int8_t col, int8_t row) { gfx_set_text_col_row(col, row); }
    inline void setColors(color_t fg, color_t bg) { gfx_fore_color = fg, gfx_back_color = bg; }
    inline void setColors(color_t fg, color_t bg, color_t gp) { gfx_fore_color = fg, gfx_back_color = bg, gfx_gap_color = gp; }
    inline void moveTo(coord_x x, coord_y y) { gfx_cursor_x = x, gfx_cursor_y = y; }
    inline void moveToX(coord_x x) { gfx_cursor_x = x; }
    inline void moveToY(coord_y y) { gfx_cursor_x = y; }
    inline void moveBy(coord_x x, coord_y y) { gfx_cursor_x += x, gfx_cursor_y += y; }
    inline void moveXBy(coord_x x) { gfx_cursor_x += x; }
    inline void moveYBy(coord_y y) { gfx_cursor_y += y; }
    // @formatter:on


    // @formatter:off
    inline void drawCircleOctants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs) { gfx_draw_circle_octants(cx, cy, x, y, octs); }
    inline void fillCircleOctants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs) { gfx_fill_circle_octants(cx, cy, x, y, octs); }
    inline void circleOctants(int8_t r, uint8_t asQuadrants, uint8_t octs, fp_circle_octants drawOctants) { gfx_circle_octants(r, octs, drawOctants); }
    inline void hLine(coord_x x0, coord_y y0, coord_x x1, color_t color) { gfx_hline(x0, y0, x1, color); }
    inline void vLine(coord_x x0, coord_y y0, coord_y y1, color_t color) { gfx_vline(x0, y0, y1, color); }
    inline void hLineTo(coord_x x1) { gfx_hline_to(x1); }
    inline void vLineTo(coord_y y1) { gfx_vline_to(y1); }
    inline void lineTo(coord_x x1, coord_y y1) { gfx_line_to(x1, y1); }
    inline void doubleLineTo(coord_x x1, coord_y y1, int8_t xSpc, coord_y ySpc) { gfx_double_line_to(x1, y1, xSpc, ySpc); }
    inline void doubleHLineTo(coord_x x1, coord_y ySpc) { gfx_double_hline_to(x1, ySpc); }
    inline void doubleVLineTo(int16_t y1, int8_t xSpc) { gfx_double_vline_to(y1, xSpc); }
    inline void rect(coord_x x1, coord_y y1) { gfx_rect(x1, y1); }
    inline void circle(int8_t radius) { gfx_circle(radius); }
    inline void roundRect(coord_x x1, coord_y y1, int8_t r, uint8_t octs) { gfx_round_rect(x1, y1, r, octs); }
    // @formatter:on

    // text functions
    // @formatter:off
    inline void setTextSizeFlags(uint8_t flags) { gfx_set_text_size_flags(flags); }
    inline void setTextFlags(uint8_t flags) { gfx_set_text_flags(flags); }
    inline void clearTextFlags(uint8_t flags) { gfx_clear_text_flags(flags); }

    inline void setWrapText() { setTextFlags(GFX_TEXT_FLAG_WRAP); }
    inline void clearWrapText() { clearTextFlags(GFX_TEXT_FLAG_WRAP); }
    inline bool isTextWrapping() { return someSet(gfx_text_flags, GFX_TEXT_FLAG_WRAP); }

    inline void setDrawTextBorder() { gfx_set_text_flags(GFX_TEXT_FLAG_BORDER); }
    inline void clearDrawTextBorder() { gfx_clear_text_flags(GFX_TEXT_FLAG_BORDER); }
    inline bool isDrawTextBorder() { return (gfx_text_flags & GFX_TEXT_FLAG_BORDER); }

    inline bool isCharVisible() { return gfx_is_char_visible(); }
    inline bool isCharClipped() { return gfx_is_char_clipped(); }
    inline bool putCh(char ch) { return gfx_put_ch(ch); }
    // @formatter:on

    // @formatter:off
    inline void write(char ch) { gfx_putc(ch); }
    inline void print(char ch) { gfx_putc(ch); }
    inline void print(const char *str) { gfx_fputs(str); }
#ifndef CONSOLE_DEBUG
    inline void print(PGM_STR str) { gfx_fputs_P((PGM_P)str); }
#endif
    inline void print(int32_t i) { gfx_print_int32(i); }
    inline void print(int32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_int32_lpad(i, radix, pad, ch); }
    inline void print(uint32_t i) { gfx_print_uint32(i); }
    inline void print(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_uint32_lpad(i, radix, pad, ch); }
    inline void print(int16_t i) { gfx_print_int16(i); }
    inline void print(int16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_int16_lpad(i, radix, pad, ch); }
    inline void print(uint16_t i) { gfx_print_uint16(i); }
    inline void print(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_uint16_lpad(i, radix, pad, ch); }
    inline void print(int8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_int8_lpad(i, radix, pad, ch); }
    inline void print(uint8_t i) { gfx_print_uint8(i); }
    inline void printUInt8Pad(uint8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_uint8_lpad(i, radix, pad, ch); }
    inline void printDigit(uint8_t dig) { gfx_put_dig(dig); }
    inline void printInt8(int8_t i) { gfx_print_int8(i); }

    inline void println(char ch) { gfx_putc(ch); gfx_new_line(); }
    inline void println(const char *str) { gfx_fputs(str); gfx_new_line(); }
#ifndef CONSOLE_DEBUG
    inline void println(PGM_STR str) { gfx_fputs_P((PGM_P)str); gfx_new_line(); }
#endif
    inline void println(int32_t i) { gfx_print_int32(i); gfx_new_line(); }
    inline void println(int32_t i, uint8_t pad, char ch = '0') { gfx_print_int32_lpad(i, 0, pad, ch); gfx_new_line(); }
    inline void println(uint32_t i) { gfx_print_uint32(i); gfx_new_line(); }
    inline void println(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_uint32_lpad(i, radix, pad, ch); gfx_new_line(); }
    inline void println(int16_t i) { gfx_print_int16(i); gfx_new_line(); }
    inline void println(int16_t i, uint8_t pad, char ch = '0') { gfx_print_int16_lpad(i, 0, pad, ch); gfx_new_line(); }
    inline void println(uint16_t i) { gfx_print_uint16(i); gfx_new_line(); }
    inline void println(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { gfx_print_uint16_lpad(i, radix, pad, ch); gfx_new_line(); }
    inline void println(int8_t i, uint8_t pad, char ch = '0') { gfx_print_int8_lpad(i, 0, pad, ch); gfx_new_line(); }
    inline void println(uint8_t i) { gfx_print_uint8(i); gfx_new_line(); }
    inline void println(double d, uint8_t digits = 0) { gfx_print_float(d, digits); gfx_new_line(); }
// @formatter:on

    inline void printTextCentered(PGM_STR message, uint8_t flags) {
        gfx_print_centered_P((PGM_P) message, flags);
    }

    void printValue(uint8_t flags, int16_t value, uint16_t valueDivider, PGM_STR suffix);
};

extern Ssd1306Display display;

#endif //SSD1306_SSD1306DISPLAY_H

#pragma clang diagnostic pop
