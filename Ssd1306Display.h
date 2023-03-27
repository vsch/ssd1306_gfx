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
    inline bool begin(uint8_t switchVcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0) {
        initDisplay();
        return true;
    }

    inline void setTextColor(uint16_t c) {
        ssd1306_fore_color = c;
        ssd1306_back_color = SSD1306_COLOR_NONE;
    }

    // @formatter:off
    inline void setTextSize(uint8_t s) { ssd1306_set_text_size_flags(s == 2 ? SSD1306_TEXT_FLAG_DOUBLE_SIZE : 0); }
    inline void setCursor(int16_t x, int16_t y) { moveTo(x, y); }
    inline void setTextCursor(int8_t x, int8_t y) { moveToText(x, y); }
    inline void drawPixel(int16_t x, int16_t y, color_t color) { ssd1306_set_pixel(x, (coord_y) y, color); }
    inline int16_t getCursorX() { return ssd1306_cursor_x; };
    inline int8_t getCursorY() { return ssd1306_cursor_y; };
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
    inline void startTextBounds() { ssd_1306_start_text_bounds(); }
    inline void endTextBounds(int16_t *pX0, int8_t *pY0, uint8_t *pW, uint8_t *pH) { ssd_1306_end_text_bounds(pX0, pY0, pW, pH); }
    inline void getTextBounds(PGM_STR s, int16_t *pX0, int8_t *pY0, uint8_t *pW, uint8_t *pH) { ssd1306_get_text_bounds((PGM_P) s, pX0, pY0, pW, pH); }
    // @formatter:on

    // C implementation to map
    PROPERTY(uint8_t, Flags, ssd1306_flags);           // option flags
    PROPERTY_GET(uint8_t, TextFlags, ssd1306_text_flags); // text option flags
    PROPERTY(int8_t, CharW, ssd1306_char_x_size);          // char width updated when text size flags change
    PROPERTY(int8_t, CharH, ssd1306_char_y_size);          // char height updated when text size flags change

    PROPERTY(coord_x, X, ssd1306_cursor_x); // cursor
    PROPERTY(coord_x, MaxX, ssd1306_max_x); // cursor
    PROPERTY(coord_x, LeftMargin, ssd1306_margin_left); // left margin for text wrapping, normally 0, cX will be set to this on wrapping
    PROPERTY(coord_x, RightMargin, ssd1306_margin_right); // right margin for text wrapping, normally DISPLAY_XSIZE
    PROPERTY(coord_y, Y, ssd1306_cursor_y); // cursor
    PROPERTY(coord_y, MaxY, ssd1306_max_y); // cursor

    PROPERTY(color_t, ForeColor, ssd1306_fore_color);

    PROPERTY(color_t, GapColor, ssd1306_gap_color);            // dash gap color
    PROPERTY(color_t, BackColor, ssd1306_back_color);

    PROPERTY(uint8_t, DashBits, ssd1306_dash_bits);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashSize, ssd1306_dash_size);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashOffset, ssd1306_dashOffset);          // solid/dash/dot pattern for line outlines (not text)

    // wrapper on C implementation
    // @formatter:off
    inline void initDisplay() { ssd1306_init_display(); }
    inline void displayOff() { ssd1306_display_off(); }
    inline void displayOn() { ssd1306_display_on(); }
    inline void clearScreen() { ssd1306_clear_screen(); }
    inline void clearDisplay() { ssd1306_clear_screen(); }
    inline void display() { ssd1306_display(); }
    inline void setInvertedDisplay() { ssd1306_set_inverted(); }
    inline void clearInvertedDisplay() { ssd1306_clear_inverted(); }
    inline void setContrast(uint8_t contrast) { ssd1306_set_contrast(contrast); }
    inline void setLinePattern(uint16_t pattern) { ssd1306_set_line_pattern(pattern); }
    inline uint16_t getLinePattern() { return ssd1306_get_line_pattern(); }

    inline uint8_t nextDashPixelColor() { return ssd1306_next_dash_color(); }
    inline uint8_t nextDashBit() { return ssd1306_next_dash_bit(); }
    inline void setPixel(coord_x x, coord_y y, color_t color) { ssd1306_set_pixel(x, y, color); }
    inline void moveToText(int8_t col, int8_t row) { ssd1306_set_text_col_row(col, row); }
    inline void setColors(color_t fg, color_t bg) { ssd1306_set_colors(fg, bg); }
    inline void setColors(color_t fg, color_t bg, color_t gp) { ssd1306_set_pattern_colors(fg, bg, gp); }
    inline void moveTo(coord_x x, coord_y y) { ssd1306_move_to(x, y); }
    inline void moveToX(coord_x x) { ssd1306_move_x_to(x); }
    inline void moveToY(coord_y y) { ssd1306_move_y_to(y); }
    inline void moveBy(coord_x x, coord_y y) { ssd1306_move_by(x, y); }
    inline void moveXBy(coord_x x) { ssd1306_move_x_by(x); }
    inline void moveYBy(coord_y y) { ssd1306_move_y_by(y); }
    // @formatter:on


    // @formatter:off
    inline void drawCircleOctants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs) { ssd1306_draw_circle_octants(cx, cy, x, y, octs); }
    inline void fillCircleOctants(coord_x cx, coord_y cy, coord_x x, coord_y y, uint8_t octs) { ssd1306_fill_circle_octants(cx, cy, x, y, octs); }
    inline void circleOctants(int8_t r, uint8_t asQuadrants, uint8_t octs, fp_circle_octants drawOctants) { ssd1306_circle_octants(r, octs, drawOctants); }
    inline void hLine(coord_x x0, coord_y y0, coord_x x1, color_t color) { ssd1306_hline(x0, y0, x1, color); }
    inline void vLine(coord_x x0, coord_y y0, coord_y y1, color_t color) { ssd1306_vline(x0, y0, y1, color); }
    inline void hLineTo(coord_x x1) { ssd1306_hline_to(x1); }
    inline void vLineTo(coord_y y1) { ssd1306_vline_to(y1); }
    inline void lineTo(coord_x x1, coord_y y1) { ssd1306_line_to(x1, y1); }
    inline void doubleLineTo(coord_x x1, coord_y y1, int8_t xSpc, coord_y ySpc) { ssd1306_double_line_to(x1, y1, xSpc, ySpc); }
    inline void doubleHLineTo(coord_x x1, coord_y ySpc) { ssd1306_double_hline_to(x1, ySpc); }
    inline void doubleVLineTo(int16_t y1, int8_t xSpc) { ssd1306_double_vline_to(y1, xSpc); }
    inline void rect(coord_x x1, coord_y y1) { ssd1306_rect(x1, y1); }
    inline void circle(int8_t radius) { ssd1306_circle(radius); }
    inline void roundRect(coord_x x1, coord_y y1, int8_t r, uint8_t octs) { ssd1306_round_rect(x1, y1, r, octs); }
    // @formatter:on

    // text functions
    // @formatter:off
    inline void setTextSizeFlags(uint8_t flags) { ssd1306_set_text_size_flags(flags); }
    inline void setTextFlags(uint8_t flags) { ssd1306_set_text_flags(flags); }
    inline void clearTextFlags(uint8_t flags) { ssd1306_clear_text_flags(flags); }

    inline void setWrapText() { setTextFlags(SSD1306_TEXT_FLAG_WRAP); }
    inline void clearWrapText() { clearTextFlags(SSD1306_TEXT_FLAG_WRAP); }
    inline bool isTextWrapping() { return someSet(ssd1306_text_flags, SSD1306_TEXT_FLAG_WRAP); }

    inline void setDrawTextBorder() { ssd1306_set_text_flags(SSD1306_TEXT_FLAG_BORDER); }
    inline void clearDrawTextBorder() { ssd1306_clear_text_flags(SSD1306_TEXT_FLAG_BORDER); }
    inline bool isDrawTextBorder() { return (ssd1306_text_flags & SSD1306_TEXT_FLAG_BORDER); }

    inline bool isCharVisible() { return ssd1306_is_char_visible(); }
    inline bool isCharClipped() { return ssd1306_is_char_clipped(); }
    inline bool putCh(char ch) { return ssd1306_put_ch(ch); }
    // @formatter:on

    // @formatter:off
    inline void write(char ch) { ssd1306_putc(ch); }
    inline void print(char ch) { ssd1306_putc(ch); }
    inline void print(const char *str) { ssd1306_fputs(str); }
#ifndef CONSOLE_DEBUG
    inline void print(PGM_STR str) { ssd1306_fputs_P((PGM_P)str); }
#endif
    inline void print(int32_t i) { ssd1306_print_int32(i); }
    inline void print(int32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_int32_lpad(i, radix, pad, ch); }
    inline void print(uint32_t i) { ssd1306_print_uint32(i); }
    inline void print(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_uint32_lpad(i, radix, pad, ch); }
    inline void print(int16_t i) { ssd1306_print_int16(i); }
    inline void print(int16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_int16_lpad(i, radix, pad, ch); }
    inline void print(uint16_t i) { ssd1306_print_uint16(i); }
    inline void print(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_uint16_lpad(i, radix, pad, ch); }
    inline void print(int8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_int8_lpad(i, radix, pad, ch); }
    inline void print(uint8_t i) { ssd1306_print_uint8(i); }
    inline void printUInt8Pad(uint8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_uint8_lpad(i, radix, pad, ch); }
    inline void printDigit(uint8_t dig) { ssd1306_put_dig(dig); }
    inline void printInt8(int8_t i) { ssd1306_print_int8(i); }

    inline void println(char ch) { ssd1306_putc(ch); ssd1306_new_line(); }
    inline void println(const char *str) { ssd1306_fputs(str); ssd1306_new_line(); }
#ifndef CONSOLE_DEBUG
    inline void println(PGM_STR str) { ssd1306_fputs_P((PGM_P)str); ssd1306_new_line(); }
#endif
    inline void println(int32_t i) { ssd1306_print_int32(i); ssd1306_new_line(); }
    inline void println(int32_t i, uint8_t pad, char ch = '0') { ssd1306_print_int32_lpad(i, 0, pad, ch); ssd1306_new_line(); }
    inline void println(uint32_t i) { ssd1306_print_uint32(i); ssd1306_new_line(); }
    inline void println(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_uint32_lpad(i, radix, pad, ch); ssd1306_new_line(); }
    inline void println(int16_t i) { ssd1306_print_int16(i); ssd1306_new_line(); }
    inline void println(int16_t i, uint8_t pad, char ch = '0') { ssd1306_print_int16_lpad(i, 0, pad, ch); ssd1306_new_line(); }
    inline void println(uint16_t i) { ssd1306_print_uint16(i); ssd1306_new_line(); }
    inline void println(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_print_uint16_lpad(i, radix, pad, ch); ssd1306_new_line(); }
    inline void println(int8_t i, uint8_t pad, char ch = '0') { ssd1306_print_int8_lpad(i, 0, pad, ch); ssd1306_new_line(); }
    inline void println(uint8_t i) { ssd1306_print_uint8(i); ssd1306_new_line(); }
    inline void println(double d, uint8_t digits = 0) { ssd1306_print_float(d, digits); ssd1306_new_line(); }
// @formatter:on

    inline void printTextCentered(PGM_STR message, uint8_t flags) {
        ssd1306_print_centered_P((PGM_P) message, flags);
    }

    inline void printValue(uint8_t flags, int32_t value, uint16_t valueDivider, PGM_STR suffix) {
        ssd1306_print_value(flags, value, valueDivider, (PGM_P)suffix);
    }
};

extern Ssd1306Display display;

#endif //SSD1306_SSD1306DISPLAY_H

#pragma clang diagnostic pop
