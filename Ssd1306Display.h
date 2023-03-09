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

#define SSD1306_WHITE SSD1306_COLOR_WHITE
#define SSD1306_BLACK SSD1306_COLOR_BLACK
#define SSD1306_INVERSE SSD1306_COLOR_INVERT
#define SSD1306_TRANSPARENT SSD1306_COLOR_NONE

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
    inline bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0) {
        initDisplay();
        return true;
    }

    inline void setTextColor(uint16_t c) { ssd1306_foreColor = c; ssd1306_backColor = SSD1306_COLOR_NONE; }

    inline void setTextSize(uint8_t s) {
        if (s == 2) {
            ssd1306_setTextFlags(SSD1306_FLAG_TEXT_DOUBLE_SIZE);
        } else {
            ssd1306_setTextFlags(0);
        }
    }

    inline void setCursor(int16_t x, int16_t y) { moveTo(x, y); }

    inline void setTextCursor(int8_t x, int8_t y) { moveToText(x, y); }

    inline void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
        setForeColor(color);
        moveTo(x0, y0);
        lineTo(x1, y1);
    }

    inline void drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
        setForeColor(color);
        moveTo(x0, y0);
        rect(x1, y1);
    }

    inline void fillRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
        setForeColor(SSD1306_COLOR_NONE);
        setBackColor(color);
        moveTo(x0, y0);
        rect(x1, y1);
    }

    inline void drawRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color) {
        setForeColor(color);
        moveTo(x0, y0);
        roundRect(x1, y1, r, SSD1306_OCT_ALL);
    }

    inline void fillRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color) {
        setForeColor(SSD1306_COLOR_NONE);
        setBackColor(color);
        moveTo(x0, y0);
        roundRect(x1, y1, r, SSD1306_OCT_ALL);
    }

    inline void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
        setForeColor(color);
        moveTo(x0, y0);
        circle(r);
    }

    inline void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
        setForeColor(SSD1306_COLOR_NONE);
        setBackColor(color);
        moveTo(x0, y0);
        circle(r);
    }

    inline void drawPixel(int16_t x, int16_t y, color_t color) { ssd1306_setPixel(x, y, color); }

    inline void drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
        setCursor(x, y);
        setForeColor(color);
        setBackColor(SSD1306_COLOR_NONE);
        ssd1306_bitmap(bitmap, w, h);
    }

    inline int16_t getCursorX(void) { return ssd1306_cX; };

    inline int8_t getCursorY(void) { return ssd1306_cY; };

    // FIX: needs work
    inline void getTextBounds(PGM_P s, int16_t x, int16_t y, int16_t *pX1, int16_t *pY1, uint16_t *pW, uint16_t *pH) {
        int8_t sy1;
        uint8_t sh, sw;
        ssd1306_getTextBounds(s, x, (int8_t) y, pX1, &sy1, &sw, &sh);
        if (pY1 != NULL) *pY1 = sy1;
        if (pW != NULL) *pW = sw;
        if (pH != NULL) *pH = sh;
    }

    inline void getTextBounds(const __FlashStringHelper *s, int16_t x, int8_t y, int16_t *x1, int8_t *y1, uint8_t *w, uint8_t *h) {
        ssd1306_getTextBounds((PGM_P) s, x, y, x1, y1, w, h);
    }


    // C implementation to map
    PROPERTY(uint8_t, Flags, ssd1306_flags);           // option flags
    PROPERTY(int8_t, SizeX, ssd1306_cSizeX);          // char width updated when text size flags change
    PROPERTY(int8_t, SizeY, ssd1306_cSizeY);          // char height updated when text size flags change

    PROPERTY(int16_t, X, ssd1306_cX); // cursor
    PROPERTY(int16_t, MaxX, ssd1306_maxX); // cursor
    PROPERTY(int16_t, LeftMargin, ssd1306_marginLeft); // left margin for text wrapping, normally 0, cX will be set to this on wrapping
    PROPERTY(int16_t, RightMargin, ssd1306_marginRight); // right margin for text wrapping, normally DISPLAY_XSIZE
    PROPERTY(int8_t, Y, ssd1306_cY); // cursor
    PROPERTY(int8_t, MaxY, ssd1306_maxY); // cursor

    PROPERTY(color_t, ForeColor, ssd1306_foreColor);

    PROPERTY(color_t, GapColor, ssd1306_gapColor);            // dash gap color
    PROPERTY(color_t, BackColor, ssd1306_backColor);

    PROPERTY(uint8_t, DashBits, ssd1306_dashBits);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashSize, ssd1306_dashSize);            // solid/dash/dot pattern for line outlines (not text)
    PROPERTY(uint8_t, DashOffset, ssd1306_dashOffset);          // solid/dash/dot pattern for line outlines (not text)

    // wrapper on C implementation

    // @formatter:off
    inline void initDisplay() { ssd1306_initDisplay(); }
    inline void displayOff() { ssd1306_displayOff(); }
    inline void displayOn() { ssd1306_displayOn(); }
    inline void clearScreen() { ssd1306_clearScreen(); }
    inline void clearDisplay() { ssd1306_clearScreen(); }
    inline void display() { ssd1306_display(); }
    inline void setInverted() { ssd1306_setInverted(); }
    inline void clearInverted() { ssd1306_clearInverted(); }
    inline void setContrast(uint8_t contrast) { ssd1306_setContrast(contrast); }
    inline void setLinePattern(uint16_t pattern) { ssd1306_setLinePattern(pattern); }
    inline uint16_t getLinePattern() { return ssd1306_getLinePattern(); }

    inline uint8_t nextDashPixelColor() { return ssd1306_nextDashPixelColor(); }
    inline uint8_t nextDashBit() { return ssd1306_nextDashBit(); }
    inline void setPixel(int16_t x, int8_t y, color_t color) { ssd1306_setPixel(x, y, color); }
    // @formatter:on

    inline void invertDisplay(bool val) {
        if (val) {
            ssd1306_setInverted();
        } else {
            ssd1306_clearInverted();
        }
    }

    inline void setTextColor(uint16_t c, uint16_t bg) {
        ssd1306_foreColor = c;
        ssd1306_backColor = bg;
    }

    inline void setColors(color_t fg, color_t bg) {
        ssd1306_foreColor = fg;
        ssd1306_backColor = bg;
    }

    inline void setColors(color_t fg, color_t bg, color_t gp) {
        ssd1306_foreColor = fg;
        ssd1306_backColor = bg;
        ssd1306_gapColor = gp;
    }

    inline void moveTo(int16_t x, int8_t y) {
        ssd1306_cX = x;
        ssd1306_cY = y;
        ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
    }

    inline void moveToText(int8_t x, int8_t y) { ssd1306_setTextColRow(x, y); }

    inline void moveBy(int16_t x, int8_t y) {
        ssd1306_cX += x;
        ssd1306_cY += y;
        ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
    }

    // @formatter:off
    inline void drawCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t octs) { ssd1306_drawCircleOctants(cx, cy, x, y, octs); }
    inline void fillCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t octs) { ssd1306_fillCircleOctants(cx, cy, x, y, octs); }
    inline void circleOctants(int8_t r, uint8_t asQuadrants, uint8_t octs, circleOctants drawOctants) { ssd1306_circleOctants(r, asQuadrants, octs, drawOctants); }
    inline void hLine(int16_t x0, int8_t y0, int16_t x1, color_t color) { ssd1306_hLine(x0, y0, x1, color); }
    inline void vLine(int16_t x0, int8_t y0, int8_t y1, color_t color) { ssd1306_vLine(x0, y0, y1, color); }
    inline void hLineTo(int16_t x1) { ssd1306_hLineTo(x1); }
    inline void vLineTo(int8_t y1) { ssd1306_vLineTo(y1); }
    inline void lineTo(int16_t x1, int8_t y1) { ssd1306_lineTo(x1, y1); }
    inline void rect(int16_t x1, int8_t y1) { ssd1306_rect(x1, y1); }
    inline void circle(int8_t radius) { ssd1306_circle(radius); }
    inline void roundRect(int16_t x1, int8_t y1, int8_t r, uint8_t octs) { ssd1306_roundRect(x1, y1, r, octs); }
    // @formatter:on

    // text functions
    // @formatter:off
    inline void setTextFlags(uint8_t flags) { ssd1306_setTextFlags(flags); }
    inline bool isCharVisible() { return ssd1306_isCharVisible(); }
    inline bool isCharClipped() { return ssd1306_isCharClipped(); }
    inline bool putCh(char ch) { return ssd1306_putCh(ch); }
    // @formatter:on

    // @formatter:off
    inline void write(char ch) { ssd1306_printChar(ch); }
    inline void print(char ch) { ssd1306_printChar(ch); }
    inline void print(char ch, uint8_t count) { ssd1306_printChars(ch, count); }
    inline void print(const char *str) { ssd1306_printText(str); }
    inline uint8_t print(const char *str, uint8_t count) { return ssd1306_printTextChars(str, count); }
    inline void print(const __FlashStringHelper *str) { ssd1306_printPgmText((PGM_P)str); }
    inline uint8_t print(const __FlashStringHelper *str, uint8_t count) { return ssd1306_printPgmTextChars((PGM_P)str, count); }
    inline void print(double d, uint8_t digits = 0) { ssd1306_printFloat(d, digits); }
    inline void print(int32_t i) { ssd1306_printInt32(i); }
    inline void print(int32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printInt32Pad(i, radix, pad, ch); }
    inline void print(uint32_t i) { ssd1306_printUInt32(i); }
    inline void print(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printUInt32Pad(i, radix, pad, ch); }
    inline void print(int16_t i) { ssd1306_printInt16(i); }
    inline void print(int16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printInt16Pad(i, radix, pad, ch); }
    inline void print(uint16_t i) { ssd1306_printUInt16(i); }
    inline void print(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printUInt16Pad(i, radix, pad, ch); }
    inline void print(int8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printInt8Pad(i, radix, pad, ch); }
    inline void print(uint8_t i) { ssd1306_printUInt8(i); }
    inline void printUInt8Pad(uint8_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printUInt8Pad(i, radix, pad, ch); }
    inline void printDigit(uint8_t dig) { ssd1306_printDigit(dig); }
    inline void printInt8(int8_t i) { ssd1306_printInt8(i); }

    inline void println(char ch) { ssd1306_printChar(ch); ssd1306_newLine(); }
    inline void println(char ch, uint8_t count) { ssd1306_printChars(ch, count); ssd1306_newLine(); }
    inline void println(const char *str) { ssd1306_printText(str); ssd1306_newLine(); }
    inline uint8_t println(const char *str, uint8_t count) { return ssd1306_printTextChars(str, count); ssd1306_newLine(); }
    inline void println(const __FlashStringHelper *str) { ssd1306_printPgmText((PGM_P)str); ssd1306_newLine(); }
    inline uint8_t println(const __FlashStringHelper *str, uint8_t count) { return ssd1306_printPgmTextChars((PGM_P)str, count); ssd1306_newLine(); }
    inline void println(int32_t i) { ssd1306_printInt32(i); ssd1306_newLine(); }
    inline void println(int32_t i, uint8_t pad, char ch = '0') { ssd1306_printInt32Pad(i, 0, pad, ch); ssd1306_newLine(); }
    inline void println(uint32_t i) { ssd1306_printUInt32(i); ssd1306_newLine(); }
    inline void println(uint32_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printUInt32Pad(i, radix, pad, ch); ssd1306_newLine(); }
    inline void println(int16_t i) { ssd1306_printInt16(i); ssd1306_newLine(); }
    inline void println(int16_t i, uint8_t pad, char ch = '0') { ssd1306_printInt16Pad(i, 0, pad, ch); ssd1306_newLine(); }
    inline void println(uint16_t i) { ssd1306_printUInt16(i); ssd1306_newLine(); }
    inline void println(uint16_t i, uint8_t radix, uint8_t pad = 0, char ch = '0') { ssd1306_printUInt16Pad(i, radix, pad, ch); ssd1306_newLine(); }
    inline void println(int8_t i, uint8_t pad, char ch = '0') { ssd1306_printInt8Pad(i, 0, pad, ch); ssd1306_newLine(); }
    inline void println(uint8_t i) { ssd1306_printUInt8(i); ssd1306_newLine(); }
    inline void println(double d, uint8_t digits = 0) { ssd1306_printFloat(d, digits); ssd1306_newLine(); }
// @formatter:on

};

extern Ssd1306Display display;

#endif //SSD1306_SSD1306DISPLAY_H
