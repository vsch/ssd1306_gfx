//
// Created by Vladimir Schneider on 2023-03-17.
//

#include "Arduino.h"
#include "ssd1306_display.h"
#include "Ssd1306Display.h"

// Compatibility
void Ssd1306Display::invertDisplay(bool val) {
    if (val) {
        ssd1306_set_inverted();
    } else {
        ssd1306_clear_inverted();
    }
}

void Ssd1306Display::setTextColor(uint16_t c, uint16_t bg) {
    ssd1306_flush_wrap_chars();
    ssd1306_fore_color = c;
    ssd1306_back_color = bg;
}

void Ssd1306Display::getTextBounds(PGM_STR s, int16_t x, int16_t y, int16_t *pX0, int16_t *pY0, uint16_t *pW, uint16_t *pH) {
    int8_t sy0;
    uint8_t sh, sw;
    int16_t sx = ssd1306_cursor_x;
    int8_t sy = ssd1306_cursor_y;

    ssd1306_cursor_x = x;
    ssd1306_cursor_y = y;
    ssd1306_get_text_bounds((PGM_P) s, pX0, &sy0, &sw, &sh);
    ssd1306_cursor_x = sx;
    ssd1306_cursor_y = sy;
    if (pY0) *pY0 = sy0;
    if (pW) *pW = sw;
    if (pH) *pH = sh;
}

void Ssd1306Display::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    setForeColor(color);
    moveTo(x0, y0);
    lineTo(x1, y1);
}

void Ssd1306Display::drawRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    setForeColor(color);
    moveTo(x0, y0);
    rect(x1, y1);
}

void Ssd1306Display::fillRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    setForeColor(SSD1306_COLOR_NONE);
    setBackColor(color);
    moveTo(x0, y0);
    rect(x1, y1);
}

void Ssd1306Display::drawRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color) {
    setForeColor(color);
    moveTo(x0, y0);
    roundRect(x1, y1, r, SSD1306_OCT_ALL);
}

void Ssd1306Display::fillRoundRect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t r, uint16_t color) {
    setForeColor(SSD1306_COLOR_NONE);
    setBackColor(color);
    moveTo(x0, y0);
    roundRect(x1, y1, r, SSD1306_OCT_ALL);
}

void Ssd1306Display::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    setForeColor(color);
    moveTo(x0, y0);
    circle(r);
}

void Ssd1306Display::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    setForeColor(SSD1306_COLOR_NONE);
    setBackColor(color);
    moveTo(x0, y0);
    circle(r);
}

void Ssd1306Display::drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color) {
    setCursor(x, y);
    setForeColor(color);
    setBackColor(SSD1306_COLOR_NONE);
    ssd1306_bitmap(bitmap, w, h);
}

void Ssd1306Display::printValue(uint8_t flags, int32_t value, uint16_t valueDivider, PGM_STR suffix) {
    ssd1306_print_value(flags, value, valueDivider, (PGM_P)suffix);
}

