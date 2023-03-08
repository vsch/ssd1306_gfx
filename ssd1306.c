#include "ssd1306_display.h"

#include "Arduino.h"

#ifdef SERIAL_DEBUG

#endif // SERIAL_DEBUG

#include "twi.h"
#include "ssd1306.h"
#include "ssd1306_cmd.h"

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

extern uint8_t ssd1306_sendBuffer[TWI_BUFFER_LENGTH];
extern uint8_t ssd1306_sendPos;

extern uint8_t ssd1306_flags;               // option flags

extern int8_t ssd1306_cY; // cursor
extern int8_t ssd1306_cX; // cursor
extern int8_t ssd1306_cXRight; // right margin for text wrapping

extern color_t ssd1306_foreColor;
extern color_t ssd1306_backColor;

extern uint8_t ssd1306_dashBits;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashSize;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

extern uint8_t ssd1306_displayBuffer[DISPLAY_YSIZE][DISPLAY_XSIZE / 8];      // display buffer

void ssd1306_initDisplay(uint8_t configFlags) {
    ssd1306_clearDisplay();

    ssd1306_sendPos = 0;

    twi_init();
    twi_setFrequency(400000L);

    //hardwareReset();            // initialize display controller
    ssd1306_startTwiCmdFrame();

    static const uint8_t PROGMEM init1[] = {
            SSD1306_DISPLAYOFF,
            SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // default after reset???
            SET_MULTIPLEX_RATIO,
    };

    ssd1306_twiPgmByteList(init1, sizeof(init1));
    ssd1306_twiByte((uint8_t) (DISPLAY_YSIZE - 1));          // send ratio

    static const uint8_t PROGMEM init2[] = {
            SSD1306_SETDISPLAYOFFSET,             // 0xD3
            0x0,                                  // no offset
            SSD1306_SETSTARTLINE | 0x0,           // line #0
            SSD1306_CHARGEPUMP                    // 0x8D
    };
    ssd1306_twiPgmByteList(init2, sizeof(init2));

    ssd1306_twiByte((configFlags & SSD1306_EXTERNALVCC) ? SSD1306_CHARGEPUMP_DISABLE : SSD1306_CHARGEPUMP_ENABLE);

    static const uint8_t PROGMEM init3[] = {
            SSD1306_MEMORYMODE,                   // 0x20
            0x00,                                 // 0x0, horizontal
            SSD1306_SEGREMAP_REVERSED,            // 0xA1
            SSD1306_COMSCANDEC                    // 0xC8
    };
    ssd1306_twiPgmByteList(init3, sizeof(init3));

#if DISPLAY_XSIZE == 128
#if DISPLAY_YSIZE == 32
    static const uint8_t PROGMEM init4a[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x02,                               // sequential, left/right remap disabled
            SSD1306_SETCONTRAST,                // 0x81
            0x8F};
    ssd1306_twiPgmByteList(init4a, sizeof(init4a));
#elif DISPLAY_YSIZE == 64
    static const uint8_t PROGMEM init4b[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x12,                               // alternative com pins, left/right disabled
            SSD1306_SETCONTRAST};              // 0x81
    ssd1306_twiByteList(init4b, sizeof(init4b));
    ssd1306_twiByte((configFlags & SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
#else
#error "Unsupported display size"
#endif

#elif DISPLAY_XSIZE == 96 && DISPLAY_YSIZE == 16
    static const uint8_t PROGMEM init4c[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x2,    // ada x12
            SSD1306_SETCONTRAST};              // 0x81
    ssd1306_twiByteList(init4c, sizeof(init4c));
    ssd1306_twiByteList((configFlags & SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);
#else
#error "Unsupported display size"
#endif

    sendByte(SSD1306_SETPRECHARGE); // 0xd9
    sendByte((configFlags & SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

    static const uint8_t PROGMEM init5[] = {
            SSD1306_SETVCOMDETECT,               // 0xDB
            0x40,                                // reset default
            SSD1306_DISPLAYALLON_RESUME,         // 0xA4
            SSD1306_NORMALDISPLAY,               // 0xA6
            SSD1306_DEACTIVATE_SCROLL,
//            SSD1306_DISPLAYALLON,                   // all on for test
            SSD1306_DISPLAYON // Main screen turn on
    };
    ssd1306_twiPgmByteList(init5, sizeof(init5));
    ssd1306_endTwiFrame();
}

void ssd1306_startTwiCmdFrame() {
    // start accumulating commands
    ssd1306_sendPos = 0;
    ssd1306_twiByte((uint8_t) 0x00); // Co = 0, D/C = 0
}

void ssd1306_startTwiDataFrame() {
    // start accumulating commands
    ssd1306_sendPos = 0;
    ssd1306_twiByte((uint8_t) 0x40); // Co = 0, D/C = 1
}

void ssd1306_twiByte(uint8_t byte) {
    if (ssd1306_sendPos >= TWI_BUFFER_LENGTH) {
        // buffer is full, send it and wait for completion
        twi_writeTo(DISPLAY_ADDRESS, ssd1306_sendBuffer, ssd1306_sendPos, 1, 1);
        ssd1306_sendPos = 1; // keep the type command/data
    }
    ssd1306_sendBuffer[ssd1306_sendPos++] = byte;
}

void ssd1306_endTwiFrame() {
    // send the accumulated buffer
    twi_writeTo(DISPLAY_ADDRESS, ssd1306_sendBuffer, ssd1306_sendPos, 1, 1);
}

// actually send command and wait for completion
void ssd1306_sendCmd(uint8_t cmd) {
    startCmd();
    ssd1306_twiByte(cmd);
    endFrame();
}

void ssd1306_twiRepeatedByte(uint8_t byte, uint16_t count) {
    while (count-- > 0) {
        ssd1306_twiByte(byte);
    }
}

void ssd1306_twiPgmByteList(const uint8_t *bytes, uint16_t count) {
    while (count-- > 0) {
        ssd1306_twiByte(pgm_read_byte(bytes++));
    }
}

void ssd1306_twiByteList(uint8_t *bytes, uint16_t count) {
    while (count-- > 0) {
        ssd1306_twiByte(*bytes++);
    }
}

// Display functions

void ssd1306_setInverted() {
    sendCmd(SSD1306_INVERTDISPLAY);
}

void ssd1306_clearInverted() {
    sendCmd(SSD1306_NORMALDISPLAY);
}

void ssd1306_displayOff() {
    ssd1306_sendCmd(SSD1306_DISPLAYOFF);
}

void ssd1306_displayOn() {
    ssd1306_sendCmd(SSD1306_DISPLAYON);
}

void ssd1306_setContrast(uint8_t contrast) {
    ssd1306_startTwiCmdFrame();
    ssd1306_twiByte(SSD1306_SETCONTRAST);
    ssd1306_twiByte(contrast);
    ssd1306_endTwiFrame();
}

void ssd1306_clearDisplay() {
    memset(ssd1306_displayBuffer, 0, sizeof(ssd1306_displayBuffer));
}

void ssd1306_display() {
    ssd1306_startTwiCmdFrame();
    static const uint8_t dlist1[] PROGMEM = {
            SSD1306_PAGEADDR,
            0,                      // Page start address
            0xFF,                   // Page end (not really, but works here)
            SSD1306_COLUMNADDR, 0}; // Column start address
    ssd1306_twiPgmByteList(dlist1, sizeof(dlist1));
    ssd1306_twiByte(DISPLAY_XSIZE - 1); // Column end address
    ssd1306_endTwiFrame();

    ssd1306_startTwiDataFrame();
    ssd1306_twiByteList(ssd1306_displayBuffer, sizeof(ssd1306_displayBuffer));
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
void ssd1306_setPixel(int x, int y, color_t color) {
    if (x >= 0 && x < DISPLAY_XSIZE && y >= 0 && y < DISPLAY_YSIZE) {
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
    if (ssd1306_dashBits == SSD1306_BITS_DASH_NONE) {
        return ssd1306_foreColor;
    }

    if (ssd1306_dashOffset >= ssd1306_dashSize) {
        ssd1306_dashOffset = 0;
    }

    return (ssd1306_dashBits & (1 << ssd1306_dashOffset++) ? ssd1306_foreColor : ssd1306_backColor);
}

void ssd1306_hLine(int8_t x0, int8_t y0, int8_t x1, color_t color) {
    while (x0 <= x1) {
        setPixel(x0++, y0, color);
    }
}

// draws a vertical line in given color
void ssd1306_vLine(int8_t x0, int8_t y0, int8_t y1, color_t color) {
    while (y0 <= y1) {
        setPixel(x0, y0++, color);
    }
}

void ssd1306_hLineTo(int8_t x1) {
    while (ssd1306_cX <= x1) {
        setPixel(ssd1306_cX++, ssd1306_cY, ssd1306_nextDashPixelColor());
    }
}

// draws a vertical line in given color
void ssd1306_vLineTo(int8_t y1) {
    while (ssd1306_cY <= y1) {
        setPixel(ssd1306_cX, ssd1306_cY++, ssd1306_nextDashPixelColor());
    }
}

void ssd1306_lineTo(int8_t x1, int8_t y1) {
    int8_t dx = abs(x1 - ssd1306_cX), sx = ssd1306_cX < x1 ? 1 : -1;
    int8_t dy = abs(y1 - ssd1306_cY), sy = ssd1306_cY < y1 ? 1 : -1;

    int8_t err = (dx > dy ? dx : -dy) / 2, e2;
    for (;;) {
        setPixel(ssd1306_cX, ssd1306_cY, ssd1306_nextDashPixelColor());
        if (ssd1306_cX == x1 && ssd1306_cY == y1) break;
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
void ssd1306_rect(int8_t x1, int8_t y1) {
    int8_t cX = ssd1306_cX;
    int8_t cY = ssd1306_cY;

    if (ssd1306_backColor != SSD1306_COLOR_NONE) {
        while (ssd1306_cY <= y1) {
            ssd1306_hLine(cX, cY, x1, ssd1306_backColor);
            ssd1306_cY++;
        }
        ssd1306_cY = cY;
    }

    ssd1306_hLineTo(x1);
    ssd1306_vLineTo(y1);
    ssd1306_hLineTo(cX);
    ssd1306_vLineTo(cY);
}

// Function for circle-generation
// using Bresenham's algorithm
// octants are:
//         -x +y 128    1 +x +y
//            \     |     /
//  -y +x   64  \   |   /     2 +y +x
//                \ | /
//     -------------|---------------
//                / | \
//  -y -x  32   /   |   \     4 +y -x
//            /     |     \
//     -x -y   16      8 +x -y

// modified from https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
// to be able to use dashed line drawing
typedef void (*circleOctants)(int8_t, int8_t, int8_t, int8_t, uint8_t);

void ssd1306_fillCircleOctants(int8_t cx, int8_t cy, int8_t x, int8_t y, uint8_t dummy) {
    ssd1306_hLine(cx - x, cy + y, cx + x, ssd1306_backColor);
    ssd1306_hLine(cx - y, cy + x, cx + y, ssd1306_backColor);
    ssd1306_hLine(cx - y, cy - x, cx + y, ssd1306_backColor);
    ssd1306_hLine(cx - x, cy - y, cx + x, ssd1306_backColor);
}

void ssd1306_drawCircleOctants(int8_t cx, int8_t cy, int8_t x, int8_t y, uint8_t octs) {
    if (octs & 0x01) putpixel(cx + x, cy + y, ssd1306_nextDashPixelColor());
    if (octs & 0x02) putpixel(cx + y, cy + x, ssd1306_nextDashPixelColor());
    if (octs & 0x04) putpixel(cx + y, cy - x, ssd1306_nextDashPixelColor());
    if (octs & 0x08) putpixel(cx + x, cy - y, ssd1306_nextDashPixelColor());
    if (octs & 0x10) putpixel(cx - x, cy - y, ssd1306_nextDashPixelColor());
    if (octs & 0x20) putpixel(cx - y, cy - x, ssd1306_nextDashPixelColor());
    if (octs & 0x40) putpixel(cx - y, cy + x, ssd1306_nextDashPixelColor());
    if (octs & 0x80) putpixel(cx - x, cy + y, ssd1306_nextDashPixelColor());
}

void ssd1306_circleOctants(int8_t r, uint8_t octs, circleOctants drawOctants) {
    int8_t xc = ssd1306_cX, yc = ssd1306_cY;
    int8_t x = 0, y = r;
    int16_t d = 3 - 2 * r;
    drawOctants(xc, yc, x, y, octs);

    while (y >= x) {
        // for each pixel we will
        // draw all eight pixels
        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
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
    if (ssd1306_backColor != SSD1306_COLOR_NONE) {
        ssd1306_circleOctants(radius, 0xff, ssd1306_fillCircleOctants);
    }

    // then outline
    if (ssd1306_dashBits == SSD1306_BITS_DASH_NONE) {
        // do all 8 octants
        ssd1306_circleOctants(radius, 0xff, ssd1306_drawCircleOctants);
    } else {
        uint8_t octants; // do one at a time in order
        for (octants = 0x01; octants; octants <<= 1) {
            ssd1306_circleOctants(radius, octants, ssd1306_drawCircleOctants);
        }
    }
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void ssd1306_roundRect(int8_t x1, int8_t y1, int8_t r) {
    int8_t x0 = ssd1306_cX, y0 = ssd1306_cY;

    ssd1306_cX += r;
    ssd1306_hLineTo(x1 - r); // top side
    ssd1306_cY += r;
    ssd1306_circleOctants(r, 0x01, ssd1306_drawCircleOctants); // upper right corner
    ssd1306_circleOctants(r, 0x02, ssd1306_drawCircleOctants); // upper right corner

    ssd1306_cX += r;
    ssd1306_vLineTo(y1 - r); // right side
    ssd1306_cX -= r;
    ssd1306_circleOctants(r, 0x04, ssd1306_drawCircleOctants); // lower right corner
    ssd1306_circleOctants(r, 0x08, ssd1306_drawCircleOctants); // lower right corner

    ssd1306_cY += r;
    ssd1306_hLineTo(x0 + r); // bottom side
    ssd1306_cY -= r;
    ssd1306_circleOctants(r, 0x10, ssd1306_drawCircleOctants); // lower right corner
    ssd1306_circleOctants(r, 0x20, ssd1306_drawCircleOctants); // lower right corner

    ssd1306_cX -= r;
    ssd1306_vLineTo(y0 + r); // left side
    ssd1306_cX += r;
    ssd1306_circleOctants(r, 0x40, ssd1306_drawCircleOctants); // upper right corner
    ssd1306_circleOctants(r, 0x80, ssd1306_drawCircleOctants); // upper right corner

    ssd1306_cX -= r; // to restore it to original
    ssd1306_cY -= r; // to restore it to original
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
        setPixel(cx + x, cy + y, color);
        setPixel(cx + x, cy - y, color);
        setPixel(cx - x, cy + y, color);
        setPixel(cx - x, cy - y, color);
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
        setPixel(cx + x, cy + y, color);
        setPixel(cx + x, cy - y, color);
        setPixel(cx - x, cy + y, color);
        setPixel(cx - x, cy - y, color);
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

// ---------------------------------------------------------------------------
// TEXT ROUTINES
//
// Each ASCII character is 5x7, with one pixel space between characters
// So, character width = 6 pixels & character height = 8 pixels. //

// In portrait mode:
//    Display width = 128 pixels, so there are 21 chars/row (21x6 = 126).
//    Display height = 160 pixels, so there are 20 rows (20x8 = 160).
//    Total number of characters in portait mode = 21 x 20 = 420. //

// In landscape mode:
//    Display width is 160, so there are 26 chars/row (26x6 = 156).
//    Display height is 128, so there are 16 rows (16x8 = 128).
//    Total number of characters in landscape mode = 26x16 = 416.

void ssd1306_charOffset(uint8_t x, uint8_t y) {
    colOffset = static_cast<uint8_t>(x);
    rowOffset = static_cast<uint8_t>(y);
    maxCols = static_cast<uint8_t>((maxX - x) / SSD1306_CHAR_WIDTH);
    maxRows = static_cast<uint8_t>((maxY - y) / SSD1306_CHAR_HEIGHT);
}

void ssd1306_charCenterOffset() {
    charOffset(0, 0);
    colOffset = static_cast<uint8_t>((maxX - maxCols * SSD1306_CHAR_WIDTH + 1) / 2);
    rowOffset = static_cast<uint8_t>((maxY - maxRows * SSD1306_CHAR_HEIGHT + 1) / 2);
}

// position cursor on character x,y grid, where 0<x<20, 0<y<19.
void ssd1306_gotoCharXY(int col, int row) {
    this->charCol = static_cast<int8_t>(col);
    this->charRow = static_cast<int8_t>(row);
}

// moves character cursor to next position, assuming portrait orientation
void ssd1306_advanceCursor() {
    charCol++;             // advance x position
    if (charCol >= maxCols) {    // beyond right margin?
        charRow++;         // go to next line
        charCol = 0;       // at left margin
    }

    if (charRow >= maxRows)      // beyond bottom margin?
        charRow = 0;       // start at top again
}

// write ch to display X,Y coordinates using ASCII 5x7 font
#define OPTIMIZE_CHAR

// write ch to display X,Y coordinates using ASCII 5x7 font
void ssd1306_putCh(char ch) {
    if (isInPage(x, x + SSD1306_CHAR_WIDTH, y, y + SSD1306_CHAR_HEIGHT)) {
#ifdef OPTIMIZE_CHAR
        uint8_t charBits[5];
        *((uint32_t *) (charBits)) = pgm_read_dword(FONT_CHARS[ch - 32]);
        charBits[4] = pgm_read_byte(FONT_CHARS[ch - 32] + 4);
#endif
        uint8_t row, col, data, mask;
//    sendSetAddrWindow(x, y, x + SSD1306_CHAR_X_PIXELS - 1, y + SSD1306_CHAR_Y_PIXELS - 1);

        for (row = 0, mask = 0x01; row < SSD1306_CHAR_Y_PIXELS; row++, mask <<= 1) {
            for (col = 0; col < SSD1306_CHAR_X_PIXELS; col++) {
#ifdef OPTIMIZE_CHAR
                data = *(charBits + col);
//            sendColor((data & mask) ? foreground : background);
                setPixel(x + col, y + row, (data & mask) ? foreground : background);
#else
                data = pgm_read_byte(&(FONT_CHARS[ch - 32][col]));
//            sendColor((data & mask) ? foreground : background);
            setPixel(x + col, y + row, (data & mask) ? foreground : background);
#endif
            }
        }
    } else {
//        Serial.print("skipping ");
//        Serial.print(ch);
//        Serial.print(" ");
//        Serial.print(x);
//        Serial.print(", ");
//        Serial.print(y);
//        Serial.print(" pageStart ");
//        Serial.println(pageStart);
    }
}

// writes character to display at current cursor position.
void ssd1306_write(char ch) {
    if (charCol >= 0 && charCol < maxCols && charRow >= 0 && charRow < maxRows) {
        putCh(ch, charCol * SSD1306_CHAR_WIDTH + colOffset, charRow * SSD1306_CHAR_HEIGHT + rowOffset);
    }
    advanceCursor();
}

// writes string to display at current cursor position.
void ssd1306_write(char ch, int count) {
    while (count-- > 0) {
        if (charCol >= 0 && charCol < maxCols && charRow >= 0 && charRow < maxRows) {
            putCh(ch, charCol * SSD1306_CHAR_WIDTH + colOffset, charRow * SSD1306_CHAR_HEIGHT + rowOffset);
        }
        advanceCursor();
    }
}

void ssd1306_write(const __FlashStringHelper *text) {
    PGM_P p = reinterpret_cast<PGM_P>(text);
    while (1) {
        char c = pgm_read_byte(p++);
        if (c == 0) break;
        write(c);
    }
}

void ssd1306_write(const char *text) {
    while (*text) {
        write(*text++);
    }
}

// writes integer i at current cursor position
void ssd1306_write(int i) {
    char str[8];
    itoa(i, str, 10);
    write(str);
}

void ssd1306_writeDigit(int dig) {
    dig &= 0x0f;
    char c = (char) (dig > 9 ? 'A' - 10 + dig : '0' + dig);
    write(c);
}

// writes hexadecimal value of integer i at current cursor position
void ssd1306_writeHex(int i) {
    writeDigit(i >> 12);
    writeDigit(i >> 8);
    writeDigit(i >> 4);
    writeDigit(i);
}

void ssd1306_writeHex(long i) {
    writeHex((int) (i >> 16));
    write('.');
    writeHex((int) i);
}

void ssd1306_write(long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

void ssd1306_write(unsigned long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

// writes string to display at current cursor position.
uint8_t ssd1306_write(const char *text, uint8_t count) {
    for (; *text && count; text++, count--) // for all non-nul chars
        write(*text); // write the char
    return count;
}

void ssd1306_write(const char *buff, uint8_t pos, char c) {
    uint8_t len = strlen(buff);
    if (len >= pos) {
        write(buff, len - pos);
        write(c);
        write(buff + len - pos);
    } else {
        write(buff);
    }
}

void ssd1306_write(unsigned long i, uint8_t pos, char c) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff, pos, c);
}

void ssd1306_write(long i, uint8_t pos, char c) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff, pos, c);
}

void ssd1306_setTextSize(uint8_t cSize) {
}

#endif

