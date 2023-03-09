#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
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
const uint8_t FONT_CHARS[96][SSD1306_CHAR_X_PIXELS] PROGMEM = {
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

uint8_t ssd1306_sendPos;

uint8_t ssd1306_flags;               // option flags
uint8_t ssd1306_cSizeX;           // updated when text size flags change
uint8_t ssd1306_cSizeY;          // updated when text size flags change

int16_t ssd1306_cX; // cursor
int16_t ssd1306_cLeft; // left margin for text wrapping
int16_t ssd1306_cRight; // right margin for text wrapping
int8_t ssd1306_cY; // cursor

color_t ssd1306_foreColor;
color_t ssd1306_gapColor;
color_t ssd1306_backColor;

uint8_t ssd1306_dashBits;            // solid/dash/dot pattern for line outlines (not text)
uint8_t ssd1306_dashSize;            // solid/dash/dot pattern for line outlines (not text)
uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

uint8_t ssd1306_displayBuffer[DISPLAY_YSIZE / 8][DISPLAY_XSIZE];      // display buffer

void ssd1306_clearDisplay() {
    ssd1306_flags = 0;
    ssd1306_cSizeX = CHAR_WIDTH;             // updated when text size flags change
    ssd1306_cSizeY = CHAR_HEIGHT;           // updated when text size flags change
    ssd1306_foreColor = SSD1306_COLOR_WHITE;
    ssd1306_gapColor = SSD1306_COLOR_BLACK;
    ssd1306_backColor = SSD1306_COLOR_BLACK;
    ssd1306_dashBits = SSD1306_BITS_DASH_NONE;
    ssd1306_dashSize = SSD1306_SIZE_DASH_NONE;
    ssd1306_cX = 0;
    ssd1306_cY = 0;
    ssd1306_cLeft = 0;
    ssd1306_cRight = DISPLAY_XSIZE;
    memset(ssd1306_displayBuffer, 0, sizeof(ssd1306_displayBuffer));
}

// ---------------------------------------------------------------------------
// SIMPLE GRAPHICS ROUTINES
//
// note: many routines have uint8_t parameters, to save space,
// but these can easily be changed to int params for larger displays.

// draw pixel with alpha
// if alpha < 16 then it is treated as fully transparent
// if alpha >= 240 then it is treated as fully opaque
// else the current pixel color is blended with given color using proportional component blending
void ssd1306_setPixel(int16_t x, int8_t y, color_t color) {
    if (color != SSD1306_COLOR_NONE && x >= 0 && x < DISPLAY_XSIZE && y >= 0 && y < DISPLAY_YSIZE) {
        uint8_t page = y >> 3;
        uint8_t mask = (uint8_t) (1 << (y & 7));
        if (color == SSD1306_COLOR_WHITE) {
            ssd1306_displayBuffer[page][x] |= mask;
        } else if (color == SSD1306_COLOR_BLACK) {
            ssd1306_displayBuffer[page][x] &= ~mask;
        } else if (color == SSD1306_COLOR_INVERT) {
            ssd1306_displayBuffer[page][x] ^= mask;
        }
    }
}

uint8_t ssd1306_nextDashBit() {
    if (ssd1306_dashOffset >= ssd1306_dashSize) {
        ssd1306_dashOffset = 0;
    }
    return ssd1306_dashBits & (1 << ssd1306_dashOffset++);
}

uint8_t ssd1306_nextDashPixelColor() {
    if (ssd1306_dashBits == SSD1306_BITS_DASH_NONE && ssd1306_dashSize == SSD1306_SIZE_DASH_NONE) {
        return ssd1306_foreColor;
    } else {
        return ssd1306_nextDashBit() ? ssd1306_foreColor : ssd1306_gapColor;
    }
}

void ssd1306_hLine(int16_t x0, int8_t y0, int16_t x1, color_t color) {
    if (x0 > x1) {
        int16_t x = x0;
        x0 = x1;
        x1 = x;
    }
    while (x0 <= x1) {
        ssd1306_setPixel(x0++, y0, color);
    }
}

// draws a vertical line in given color
void ssd1306_vLine(int16_t x0, int8_t y0, int8_t y1, color_t color) {
    if (y0 > y1) {
        int8_t y = y0;
        y0 = y1;
        y1 = y;
    }
    while (y0 <= y1) {
        ssd1306_setPixel(x0, y0++, color);
    }
}

void ssd1306_hLineTo(int16_t x1) {
    if (ssd1306_cX != x1) {
        if (ssd1306_cX < x1) {
            if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
                ssd1306_cX++;
            }
            while (ssd1306_cX <= x1) {
                ssd1306_setPixel(ssd1306_cX++, ssd1306_cY, ssd1306_nextDashPixelColor());
            }
            ssd1306_cX--;
        } else {
            if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
                ssd1306_cX--;
            }
            while (ssd1306_cX >= x1) {
                ssd1306_setPixel(ssd1306_cX--, ssd1306_cY, ssd1306_nextDashPixelColor());
            }
            ssd1306_cX++;
        }
        ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
    }
}

// draws a vertical line in given color
void ssd1306_vLineTo(int8_t y1) {
    if (ssd1306_cY != y1) {
        if (ssd1306_cY < y1) {
            if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
                ssd1306_cY++;
            }
            while (ssd1306_cY <= y1) {
                ssd1306_setPixel(ssd1306_cX, ssd1306_cY++, ssd1306_nextDashPixelColor());
            }
            ssd1306_cY--;
        } else {
            if (ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH) {
                ssd1306_cY--;
            }
            while (ssd1306_cY >= y1) {
                ssd1306_setPixel(ssd1306_cX, ssd1306_cY--, ssd1306_nextDashPixelColor());
            }
            ssd1306_cY++;
        }
        ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
    }
}

void ssd1306_lineTo(int16_t x1, int8_t y1) {
    int8_t dx = abs(x1 - ssd1306_cX), sx = ssd1306_cX < x1 ? 1 : -1;
    int8_t dy = abs(y1 - ssd1306_cY), sy = ssd1306_cY < y1 ? 1 : -1;

    int8_t err = (dx > dy ? dx : -dy) / 2, e2;
    uint8_t skip = ssd1306_flags & SSD1306_FLAG_CONTINUED_PATH;

    for (;;) {
        if (skip) {
            skip = 0;
            continue;
        }
        ssd1306_setPixel(ssd1306_cX, ssd1306_cY, ssd1306_nextDashPixelColor());

        if (ssd1306_cX == x1 && ssd1306_cY == y1) {
            ssd1306_flags |= SSD1306_FLAG_CONTINUED_PATH;
            break;
        }

        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            ssd1306_cX += sx;
        }
        if (e2 < dy) {
            err += dx;
            ssd1306_cY += sy;
        }
    }
}

// draws a rectangle in given color
void ssd1306_rect(int16_t x1, int8_t y1) {
    int16_t cX = ssd1306_cX;
    int8_t cY = ssd1306_cY;
    if (ssd1306_backColor != SSD1306_COLOR_NONE) {
        while (ssd1306_cY <= y1) {
            ssd1306_hLine(cX, cY, x1, ssd1306_backColor);
            ssd1306_cY++;
        }
        ssd1306_cY = cY;
    }

    ssd1306_flags &= ~SSD1306_FLAG_CONTINUED_PATH;
    ssd1306_hLineTo(x1);
    ssd1306_vLineTo(y1);
    ssd1306_hLineTo(cX);
    ssd1306_vLineTo(cY + 1);
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

void ssd1306_fillCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t octs) {
    if (octs & 0x01) ssd1306_hLine(cx, cy - y, cx + x, ssd1306_backColor);
    if (octs & 0x02) ssd1306_vLine(cx + x, cy, cy - x, ssd1306_backColor);
    if (octs & 0x04) ssd1306_vLine(cx + y, cy, cy + y, ssd1306_backColor);
    if (octs & 0x08) ssd1306_hLine(cx, cy + y, cx + y, ssd1306_backColor);
    if (octs & 0x10) ssd1306_hLine(cx, cy + y, cx - y, ssd1306_backColor);
    if (octs & 0x20) ssd1306_vLine(cx - y, cy + x, cx - x, ssd1306_backColor);
    if (octs & 0x40) ssd1306_vLine(cx - y, cy - x, cx - x, ssd1306_backColor);
    if (octs & 0x80) ssd1306_hLine(cx, cy - x, cx - x, ssd1306_backColor);
}

void ssd1306_drawCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t octs) {
    if (octs & 0x01) ssd1306_setPixel(cx + x, cy - y, ssd1306_nextDashPixelColor());
    if (octs & 0x02) ssd1306_setPixel(cx + y, cy - x, ssd1306_nextDashPixelColor());
    if (octs & 0x04) ssd1306_setPixel(cx + y, cy + x, ssd1306_nextDashPixelColor());
    if (octs & 0x08) ssd1306_setPixel(cx + x, cy + y, ssd1306_nextDashPixelColor());
    if (octs & 0x10) ssd1306_setPixel(cx - x, cy + y, ssd1306_nextDashPixelColor());
    if (octs & 0x20) ssd1306_setPixel(cx - y, cy + x, ssd1306_nextDashPixelColor());
    if (octs & 0x40) ssd1306_setPixel(cx - y, cy - x, ssd1306_nextDashPixelColor());
    if (octs & 0x80) ssd1306_setPixel(cx - x, cy - y, ssd1306_nextDashPixelColor());
}

// here we can go efficiently and draw 8 pixels and only processing one octant
// or draw only 1 pixel and process a quadrant at a time but preserve circumference pixel drawing
// order so as to preserve dash pattern
void ssd1306_circleOctants(int8_t r, uint8_t asQuadrants, uint8_t octs, circleOctants drawOctants) {
    int8_t xc = ssd1306_cX, yc = ssd1306_cY;
    int8_t x = 0, y = r;
    int8_t *e = asQuadrants ? &r : &y; // draw quadrants if requested
    int16_t d = 3 - 2 * r;

    drawOctants(xc, yc, x, y, octs & 0x55);

    while (x <= *e) {
        // for each pixel we will
        // draw all eight pixels or only one for the current quadrant if dashed lines
        x++;

        // check for decision parameter and correspondingly update d, x, y
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }

        drawOctants(xc, yc, x, y, octs);
    }
}

// draws circle at x,y with given radius & color
void ssd1306_circle(int8_t radius) {
    // first fill
    // then outline
    if (ssd1306_dashBits == SSD1306_BITS_DASH_NONE && ssd1306_dashSize == SSD1306_SIZE_DASH_NONE) {
        // do all 8 octants
        if (ssd1306_backColor != SSD1306_COLOR_NONE) {
            ssd1306_circleOctants(radius, 0, 0xff, ssd1306_fillCircleOctants);
        }

        ssd1306_circleOctants(radius, 0, 0xff, ssd1306_drawCircleOctants);
    } else {
        uint8_t octants; // do one quadrant at a time in order, to preserve dash pattern
        for (octants = 0x01; octants; octants <<= 2) {
            if (ssd1306_backColor != SSD1306_COLOR_NONE) {
                ssd1306_circleOctants(radius, 1, octants, ssd1306_fillCircleOctants);
            }
            ssd1306_circleOctants(radius, 1, octants, ssd1306_drawCircleOctants);
        }
    }
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void ssd1306_roundRect(int16_t x1, int8_t y1, int8_t r) {
    int8_t x0 = ssd1306_cX, y0 = ssd1306_cY;
    ssd1306_cX += r; // x0+r
    ssd1306_hLineTo(x1 - r); // top side
    ssd1306_cY += r; // y0+r
    ssd1306_circleOctants(r, 1, 0x01, ssd1306_drawCircleOctants); // upper right corner

    ssd1306_cX += r; // x1
    ssd1306_vLineTo(y1 - r); // right side
    ssd1306_cX -= r; // x1-r
    ssd1306_circleOctants(r, 1, 0x04, ssd1306_drawCircleOctants); // lower right corner

    ssd1306_cY += r; // y1
    ssd1306_hLineTo(x0 + r); // bottom side
    ssd1306_cY -= r; // y1-r
    ssd1306_circleOctants(r, 1, 0x10, ssd1306_drawCircleOctants); // lower right corner

    ssd1306_cX -= r; // x0
    ssd1306_vLineTo(y0 + r); // left side
    ssd1306_cX += r; // x0+r
    ssd1306_circleOctants(r, 1, 0x40, ssd1306_drawCircleOctants); // upper right corner

    ssd1306_cX -= r; // x0
    ssd1306_cY -= r; // y0
}

#ifdef SSD1306_FANCY
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

void ssd1306_updateCharSize() {
    ssd1306_cSizeY = ssd1306_flags & SSD1306_FLAG_TEXT_DOUBLE_HEIGHT ? CHAR_HEIGHT * 2 : CHAR_HEIGHT;
    ssd1306_cSizeX = ssd1306_flags & SSD1306_FLAG_TEXT_DOUBLE_WIDTH ? CHAR_WIDTH * 2 : CHAR_WIDTH;
}

void ssd1306_setTextFlags(uint8_t flags) {
    ssd1306_flags &= ~(SSD1306_FLAG_TEXT_DOUBLE_SIZE | SSD1306_FLAG_BORDER_TEXT_CHAR | SSD1306_FLAG_TEXT_BORDER | SSD1306_FLAG_BORDER_TEXT_LINE);

    ssd1306_flags |= flags & SSD1306_FLAG_TEXT_DOUBLE_SIZE;
    ssd1306_cSizeX = (flags & SSD1306_FLAG_TEXT_DOUBLE_WIDTH) ? CHAR_WIDTH * 2 : CHAR_WIDTH;
    ssd1306_cSizeY = (flags & SSD1306_FLAG_TEXT_DOUBLE_HEIGHT) ? CHAR_HEIGHT * 2 : CHAR_HEIGHT;

    if (flags & SSD1306_FLAG_TEXT_BORDER) {
        ssd1306_flags |= SSD1306_FLAG_BORDER_TEXT_CHAR | SSD1306_FLAG_TEXT_BORDER | SSD1306_FLAG_BORDER_TEXT_LINE;
    }
}

uint8_t ssd1306_isCharVisible() {
    return !(ssd1306_cX + ssd1306_cSizeX < 0 || ssd1306_cX > DISPLAY_XSIZE || ssd1306_cY + ssd1306_cSizeY < 0 ||
             ssd1306_cY > DISPLAY_YSIZE);
}

// ssd1306_write ch to display X,Y coordinates using ASCII 5x7 font
bool ssd1306_putCh(char ch) {
    if (ch == '\n') {
        // go to next line and left margin
        ssd1306_cX = ssd1306_cLeft;
        ssd1306_cY += ssd1306_cSizeY;
        ssd1306_flags &= ~SSD1306_FLAG_TEXT_WRAPPED | SSD1306_FLAG_BORDER_TEXT_LINE;

        if (ssd1306_flags & SSD1306_FLAG_TEXT_BORDER) {
            ssd1306_flags |= SSD1306_FLAG_BORDER_TEXT_CHAR;
        }
        return false;
    } else if (ssd1306_isCharVisible() && ssd1306_foreColor != SSD1306_COLOR_NONE && ssd1306_backColor != SSD1306_COLOR_NONE) {
        uint8_t charBits[5];
        *((uint32_t *) (charBits)) = pgm_read_dword((const char *) FONT_CHARS[ch - 32]);
        charBits[4] = pgm_read_byte((const char *) FONT_CHARS[ch - 32] + 4);
        uint8_t row, col, data, mask;

        for (row = 0, mask = 0x01; row < SSD1306_CHAR_Y_PIXELS; row++, mask <<= 1) {
            for (col = 0; col < SSD1306_CHAR_X_PIXELS; col++) {
                data = *(charBits + col);
                color_t color = (data & mask) ? ssd1306_foreColor : ssd1306_backColor;
                uint8_t col2 = 2 * col;
                int row2 = 2 * row;

                switch (ssd1306_flags & SSD1306_FLAG_TEXT_DOUBLE_SIZE) {
                    case SSD1306_FLAG_TEXT_DOUBLE_HEIGHT:
                        ssd1306_setPixel(ssd1306_cX + col, ssd1306_cY + row2, color);
                        ssd1306_setPixel(ssd1306_cX + col, ssd1306_cY + row2 + 1, color);
                        break;
                    case SSD1306_FLAG_TEXT_DOUBLE_WIDTH:
                        ssd1306_setPixel(ssd1306_cX + col2, ssd1306_cY + row, color);
                        ssd1306_setPixel(ssd1306_cX + col2 + 1, ssd1306_cY + row, color);
                        break;
                    case SSD1306_FLAG_TEXT_DOUBLE_SIZE:
                        ssd1306_setPixel(ssd1306_cX + col2, ssd1306_cY + row2, color);
                        ssd1306_setPixel(ssd1306_cX + col2 + 1, ssd1306_cY + row2, color);
                        ssd1306_setPixel(ssd1306_cX + col2, ssd1306_cY + row2 + 1, color);
                        ssd1306_setPixel(ssd1306_cX + col2 + 1, ssd1306_cY + row2 + 1, color);
                        break;
                    default:
                        ssd1306_setPixel(ssd1306_cX + col, ssd1306_cY + row, color);
                        break;
                }

                // FIX: these overlap between lines, issue if using invert
                if (ssd1306_backColor != SSD1306_COLOR_NONE) {
                    if (ssd1306_flags & SSD1306_FLAG_BORDER_TEXT_CHAR) {
                        // add left border
                        ssd1306_vLine(ssd1306_cX - 1, ssd1306_cY - 1, ssd1306_cY + ssd1306_cSizeY - 1, ssd1306_backColor);
                        ssd1306_flags &= ~SSD1306_FLAG_BORDER_TEXT_CHAR;
                    }

                    if (ssd1306_flags & SSD1306_FLAG_BORDER_TEXT_LINE) {
                        // add top border
                        ssd1306_hLine(ssd1306_cX, ssd1306_cY - 1, ssd1306_cX + ssd1306_cSizeX - 1, ssd1306_backColor);
                    }
                }
            }
        }

        // add bottom and right borders always
        // FIX: these overlap between lines, issue if using invert
        ssd1306_hLine(ssd1306_cX, ssd1306_cY + ssd1306_cSizeY - 1, ssd1306_cX + ssd1306_cSizeX - 1, ssd1306_backColor);
        ssd1306_vLine(ssd1306_cX + ssd1306_cSizeX - 1, ssd1306_cY, ssd1306_cY + ssd1306_cSizeY - 2, ssd1306_backColor);
    }
    return true;
}

// writes character to display at current cursor position.
void ssd1306_writeChar(char ch) {
    if (ssd1306_flags & SSD1306_FLAG_TEXT_WRAP) {
        if (ssd1306_cX + ssd1306_cSizeX > ssd1306_cRight) {
            // wrap text
            ssd1306_flags |= SSD1306_FLAG_TEXT_WRAPPED;
            ssd1306_flags &= ~SSD1306_FLAG_BORDER_TEXT_LINE;

            if (ssd1306_flags & SSD1306_FLAG_TEXT_BORDER) {
                ssd1306_flags |= SSD1306_FLAG_BORDER_TEXT_CHAR;
            }

            ssd1306_cX = ssd1306_cLeft;
            ssd1306_cY += ssd1306_cSizeY;
        }
        if (ch != ' ' || !(ssd1306_flags & SSD1306_FLAG_TEXT_WRAPPED)) {
            ssd1306_flags &= ~SSD1306_FLAG_TEXT_WRAPPED;
            if (ssd1306_putCh(ch)) ssd1306_cX += ssd1306_cSizeX;
        }
    } else {
        if (ssd1306_putCh(ch)) ssd1306_cX += ssd1306_cSizeX;
    }
}

// writes string to display at current cursor position.
void ssd1306_writeRepeatedChars(char ch, uint8_t count) {
    while (count-- > 0) {
        ssd1306_writeChar(ch);
    }
}

void ssd1306_writeText(const char *str) {
    while (*str) {
        ssd1306_writeChar(*str++);
    }
}

void ssd1306_writePgmText(const char *str) {
    PGM_P p = (PGM_P) (str);
    for (;;) {
        char c = pgm_read_byte(p++);
        if (!c) break;
        ssd1306_writeChar(c);
    }
}

// writes string to display at current cursor position.
uint8_t ssd1306_writeTextChars(const char *str, uint8_t count) {
    for (; *str && count; str++, count--) {
        ssd1306_writeChar(*str);
    }
    return count;
}

uint8_t ssd1306_writePgmTextChars(PGM_P str, uint8_t count) {
    PGM_P p = (PGM_P) (str);
    for (; count; p++, count--) {
        char c = pgm_read_byte(p);
        if (!c) break;
        ssd1306_writeChar(c);
    }
    return count;
}

void ssd1306_writeTextLeftPad(PGM_P str, char ch, uint8_t pad) {
    if (pad) {
        uint8_t len = strlen(str);
        if (pad > len) {
            ssd1306_writeRepeatedChars(ch, pad - len);
        }
    }
    ssd1306_writeText(str);
}

void ssd1306_writePgmTextLeftPad(const char *str, char ch, uint8_t pad) {
    if (pad) {
        uint8_t len = 0;
        PGM_P p = str;
        while (pgm_read_byte(p++) && len < pad) len++;
        if (pad > len) {
            ssd1306_writeRepeatedChars(ch, pad - len);
        }
    }
    ssd1306_writeText(str);
}

// writes integer i at current cursor position
void ssd1306_writeInt32Pad(int32_t i, char ch, uint8_t pad) {
    char str[8];
    ltoa(i, str, 10);
    ssd1306_writeTextLeftPad(str, ch, pad);
}

void ssd1306_writeInt32(int32_t i) {
    ssd1306_writeInt32Pad(i, 0, 0);
}

void ssd1306_writeUInt32Pad(uint32_t i, uint8_t radix, char ch, uint8_t pad) {
    char str[8];
    ultoa(i, str, radix);
    ssd1306_writeTextLeftPad(str, ch, pad);
}

void ssd1306_writeUInt32(uint32_t i) {
    ssd1306_writeUInt32Pad(i, 10, 0, 0);
}

void ssd1306_writeInt16Pad(int16_t i, char ch, uint8_t pad) {
    ssd1306_writeInt32Pad(i, ch, pad);
}

void ssd1306_writeInt16(int16_t i) {
    ssd1306_writeInt32Pad(i, 0, 0);
}

void ssd1306_writeUInt16(uint16_t i) {
    ssd1306_writeUInt32Pad(i, 10, 0, 0);
}

void ssd1306_writeUInt16Pad(uint16_t i, uint8_t radix, char ch, uint8_t pad) {
    ssd1306_writeUInt32Pad(i, radix, ch, pad);
}

void ssd1306_writeInt8Pad(int8_t i, char ch, uint8_t pad) {
    ssd1306_writeInt32Pad(i, ch, pad);
}

void ssd1306_writeInt8(int8_t i) {
    ssd1306_writeInt32Pad(i, 0, 0);
}

void ssd1306_writeUInt8(uint8_t i) {
    ssd1306_writeUInt32Pad(i, 10, 0, 0);
}

void ssd1306_writeUInt8Pad(uint8_t i, uint8_t radix, char ch, uint8_t pad) {
    ssd1306_writeUInt32Pad(i, radix, ch, pad);
}

void ssd1306_writeDigit(uint8_t dig) {
    dig &= 0x0f;
    char c = (char) (dig > 9 ? 'A' - 10 + dig : '0' + dig);
    ssd1306_writeChar(c);
}

#ifdef CONSOLE_DEBUG

void display() {
    putchar('/');
    for (int i = 0; i < DISPLAY_XSIZE; i++) {
        putchar('-');
    }
    putchar('\\');
    putchar('\n');
    for (int p = 0; p < DISPLAY_YSIZE / 8; p++) {
        for (int b = 1; b < 256; b *= 2) {
            putchar('|');
            for (int c = 0; c < DISPLAY_XSIZE; c++) {
                putchar(ssd1306_displayBuffer[p][c] & b ? '@' : ' ');
            }
            putchar('|');
            putchar('\n');
        }
    }
    putchar('\\');
    for (int i = 0; i < DISPLAY_XSIZE; i++) {
        putchar('-');
    }
    putchar('/');
    putchar('\n');
}

#endif
