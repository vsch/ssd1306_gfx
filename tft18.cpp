#include "tft18.h"

// Based on code from:
//
// TFT: Experiments interfacing ATmega328 to an ST7735 1.8" LCD TFT display
// Author : Bruce E. Hall <bhall66@gmail.com>
// Website : w8bh.net
// Version : 1.0
// Date : 04 May 2014
// Target : ATmega328P microcontroller
// Language : C, using AVR studio 6
// Size : 3622 bytes

#include <Arduino.h>
#include <avr/io.h>         // deal with port registers
#include <avr/interrupt.h>  // deal with interrupt calls
#include <avr/pgmspace.h>   // put character data into progmem

// ---------------------------------------------------------------------------
// GLOBAL VARIABLES
const uint8_t FONT_CHARS[96][TFT_CHAR_X_PIXELS] PROGMEM = {
        0x00, 0x00, 0x00, 0x00, 0x00, // (space)
        0x00, 0x00, 0x5F, 0x00, 0x00, // !
        0x00, 0x07, 0x00, 0x07, 0x00, // "
        0x14, 0x7F, 0x14, 0x7F, 0x14, // #
        0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
        0x23, 0x13, 0x08, 0x64, 0x62, // %
        0x36, 0x49, 0x55, 0x22, 0x50, // &
        0x00, 0x05, 0x03, 0x00, 0x00, // '
        0x00, 0x1C, 0x22, 0x41, 0x00, // (
        0x00, 0x41, 0x22, 0x1C, 0x00, // )
        0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
        0x08, 0x08, 0x3E, 0x08, 0x08, // +
        0x00, 0x50, 0x30, 0x00, 0x00, // ,
        0x08, 0x08, 0x08, 0x08, 0x08, // -
        0x00, 0x60, 0x60, 0x00, 0x00, // .
        0x20, 0x10, 0x08, 0x04, 0x02, // /
        0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
        0x00, 0x42, 0x7F, 0x40, 0x00, // 1
        0x42, 0x61, 0x51, 0x49, 0x46, // 2
        0x21, 0x41, 0x45, 0x4B, 0x31, // 3
        0x18, 0x14, 0x12, 0x7F, 0x10, // 4
        0x27, 0x45, 0x45, 0x45, 0x39, // 5
        0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
        0x01, 0x71, 0x09, 0x05, 0x03, // 7
        0x36, 0x49, 0x49, 0x49, 0x36, // 8
        0x06, 0x49, 0x49, 0x29, 0x1E, // 9
        0x00, 0x36, 0x36, 0x00, 0x00, // :
        0x00, 0x56, 0x36, 0x00, 0x00, // ;
        0x00, 0x08, 0x14, 0x22, 0x41, // <
        0x14, 0x14, 0x14, 0x14, 0x14, // =
        0x41, 0x22, 0x14, 0x08, 0x00, // >
        0x02, 0x01, 0x51, 0x09, 0x06, // ?
        0x32, 0x49, 0x79, 0x41, 0x3E, // @
        0x7E, 0x11, 0x11, 0x11, 0x7E, // A
        0x7F, 0x49, 0x49, 0x49, 0x36, // B
        0x3E, 0x41, 0x41, 0x41, 0x22, // C
        0x7F, 0x41, 0x41, 0x22, 0x1C, // D
        0x7F, 0x49, 0x49, 0x49, 0x41, // E
        0x7F, 0x09, 0x09, 0x01, 0x01, // F
        0x3E, 0x41, 0x41, 0x51, 0x32, // G
        0x7F, 0x08, 0x08, 0x08, 0x7F, // H
        0x00, 0x41, 0x7F, 0x41, 0x00, // I
        0x20, 0x40, 0x41, 0x3F, 0x01, // J
        0x7F, 0x08, 0x14, 0x22, 0x41, // K
        0x7F, 0x40, 0x40, 0x40, 0x40, // L
        0x7F, 0x02, 0x04, 0x02, 0x7F, // M
        0x7F, 0x04, 0x08, 0x10, 0x7F, // N
        0x3E, 0x41, 0x41, 0x41, 0x3E, // O
        0x7F, 0x09, 0x09, 0x09, 0x06, // P
        0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
        0x7F, 0x09, 0x19, 0x29, 0x46, // R
        0x46, 0x49, 0x49, 0x49, 0x31, // S
        0x01, 0x01, 0x7F, 0x01, 0x01, // T
        0x3F, 0x40, 0x40, 0x40, 0x3F, // U
        0x1F, 0x20, 0x40, 0x20, 0x1F, // V
        0x7F, 0x20, 0x18, 0x20, 0x7F, // W
        0x63, 0x14, 0x08, 0x14, 0x63, // X
        0x03, 0x04, 0x78, 0x04, 0x03, // Y
        0x61, 0x51, 0x49, 0x45, 0x43, // Z
        0x00, 0x00, 0x7F, 0x41, 0x41, // [
        0x02, 0x04, 0x08, 0x10, 0x20, // "\"
        0x41, 0x41, 0x7F, 0x00, 0x00, // ]
        0x04, 0x02, 0x01, 0x02, 0x04, // ^
        0x40, 0x40, 0x40, 0x40, 0x40, // _
        0x00, 0x01, 0x02, 0x04, 0x00, // `
        0x20, 0x54, 0x54, 0x54, 0x78, // a
        0x7F, 0x48, 0x44, 0x44, 0x38, // b
        0x38, 0x44, 0x44, 0x44, 0x20, // c
        0x38, 0x44, 0x44, 0x48, 0x7F, // d
        0x38, 0x54, 0x54, 0x54, 0x18, // e
        0x08, 0x7E, 0x09, 0x01, 0x02, // f
        0x08, 0x14, 0x54, 0x54, 0x3C, // g
        0x7F, 0x08, 0x04, 0x04, 0x78, // h
        0x00, 0x44, 0x7D, 0x40, 0x00, // i
        0x20, 0x40, 0x44, 0x3D, 0x00, // j
        0x00, 0x7F, 0x10, 0x28, 0x44, // k
        0x00, 0x41, 0x7F, 0x40, 0x00, // l
        0x7C, 0x04, 0x18, 0x04, 0x78, // m
        0x7C, 0x08, 0x04, 0x04, 0x78, // n
        0x38, 0x44, 0x44, 0x44, 0x38, // o
        0x7C, 0x14, 0x14, 0x14, 0x08, // p
        0x08, 0x14, 0x14, 0x18, 0x7C, // q
        0x7C, 0x08, 0x04, 0x04, 0x08, // r
        0x48, 0x54, 0x54, 0x54, 0x20, // s
        0x04, 0x3F, 0x44, 0x40, 0x20, // t
        0x3C, 0x40, 0x40, 0x20, 0x7C, // u
        0x1C, 0x20, 0x40, 0x20, 0x1C, // v
        0x3C, 0x40, 0x30, 0x40, 0x3C, // w
        0x44, 0x28, 0x10, 0x28, 0x44, // x
        0x0C, 0x50, 0x50, 0x50, 0x3C, // y
        0x44, 0x64, 0x54, 0x4C, 0x44, // z
        0x00, 0x08, 0x36, 0x41, 0x00, // {
        0x00, 0x00, 0x7F, 0x00, 0x00, // |
        0x00, 0x41, 0x36, 0x08, 0x00, // }
        0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
        0x08, 0x1C, 0x2A, 0x08, 0x08, // <-
};


// ---------------------------------------------------------------------------
// SPI ROUTINES
//
//       b7   b6  b5   b4   b3   b2   b1   b0
// SPCR: SPIE SPE DORD MSTR CPOL CPHA SPR1 SPR0
//       0    1   0    1 .  0    0    0    0
//       0    1   0    1 .  1    1    0    0
//
// Both CPOL = CPHA = 0 or 1 work, for RAMRD with 0 reads all 1's, with 1 reads all 0's
// other read commands work in both modes
//
// SPIE - enable SPI interrupt
// SPE - enable SPI
// DORD - 0=MSB first, 1=LSB first
// MSTR - 0=slave, 1=master
// CPOL - 0=clock starts low, 1=clock starts high
// CPHA - 0=read on rising-edge, 1=read on falling-edge
// SPRx - 00=osc/4, 01=osc/16, 10=osc/64, 11=osc/128 //
// SPCR = 0x50: SPI enabled as Master, mode 0, at 8/4 = 2 MHz or 2x 4MHz
// SPCR = 0x5E: SPI enabled as Master, mode 3, at 8/64 = 125 KHz
// SPCR = 0x5F: SPI enabled as Master, mode 3, at 8/128 = 62.5 KHz

#define SPCR_NORMAL     0x50

Tft18::Tft18() {
    maxX = TFT_XSIZE;
    maxY = TFT_YSIZE;
    foreground = WHITE;
    background = BLACK;
    alpha = 255;
    col = 0;
    row = 0;
    charOffset(0, 0);
}

// Set the display orientation to 0,90,180,or 270 degrees
void Tft18::setOrientation(int rot) {
    uint8_t arg;
    switch (rot) {
        default:
            arg = 0x00;
            maxX = TFT_XSIZE;
            maxY = TFT_YSIZE;
            break;

        case TFT_ROT_90:
            maxX = TFT_YSIZE;
            maxY = TFT_XSIZE;
            arg = 0x60;
            break;

        case TFT_ROT_180:
            maxX = TFT_XSIZE;
            maxY = TFT_YSIZE;
            arg = 0xC0;
            break;

        case TFT_ROT_270:
            arg = 0xA0;
            maxX = TFT_YSIZE;
            maxY = TFT_XSIZE;
            break;
    }
    sendCmd(MADCTL);
    sendByte(arg);

    charOffset(0, 0);
}

void Tft18::setGama(uint8_t gama) {
    gama &= 0x0f;
    if (!gama) {
        gama = 1;
    }

    sendCmd(GAMSET);       // select gama
    sendByte(gama);
}

void Tft18::openSPI() {
    SPCR = SPCR_NORMAL;       // SPI enabled as Master, Mode0 at 4 MHz
    setBit(SPSR, SPI2X);      // double the SPI rate: 4-->8 MHz
    clearBit(TFT_CS_PORT, TFT_CS_BIT);  // select TFT CS

    clearBit(TFT_MOSI_PORT, TFT_MOSI_BIT); // disable pull up for PB3 MOSI when in input mode
}

void Tft18::closeSPI() {
    SPCR = 0x00;                        // clear SPI enable bit
    setBit(TFT_CS_PORT, TFT_CS_BIT);    // deselect TFT CS
}

void Tft18::sendByte(uint8_t byte) {
    SPDR = byte;            // initiate transfer
    while (!(SPSR & 0x80)); // wait for transfer to complete
}

void Tft18::sendCmd(uint8_t cmd) {
//    setBit(TFT_CS_PORT, TFT_CS);      // deselect TFT CS
    clearBit(TFT_DC_PORT, TFT_DC_BIT);  // B1=DC; 0=command, 1=data
//    clearBit(TFT_CS_PORT, TFT_CS);  // select TFT CS
    sendByte(cmd);
    setBit(TFT_DC_PORT, TFT_DC_BIT); // return DC high
}

void Tft18::sendWord(uint16_t word) {
    SPDR = (uint8_t)(word >> 8); // write hi uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = (uint8_t)(word & 0xFF); // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
}

void Tft18::sendColor(color_t color) {
#if COLMOD_PARAM == 5
        SPDR = (uint8_t)(color >> 8); // write hi uint8_t
        while (!(SPSR & 0x80)); // wait for transfer to complete
        SPDR = (uint8_t)(color & 0xFF); // write lo uint8_t
        while (!(SPSR & 0x80)); // wait for transfer to complete
#else
    uint8_t cBlue = color << 3;
    uint8_t cGreen = (color >> 3) & 0xFC;
    uint8_t cRed = (color >> 8) & 0xF8;
    SPDR = cRed; // write hi uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = cGreen; // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = cBlue; // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
#endif
}

void Tft18::sendColors(color_t color, uint16_t count) {
#if COLMOD_PARAM == 5
    while (count--) {
        SPDR = (uint8_t)(color >> 8); // write hi uint8_t
        while (!(SPSR & 0x80)); // wait for transfer to complete
        SPDR = (uint8_t)(color & 0xFF); // write lo uint8_t
        while (!(SPSR & 0x80)); // wait for transfer to complete
    }
#else
    while (count--) {
    uint8_t cBlue = color << 3;
    uint8_t cGreen = (color >> 3) & 0xFC;
    uint8_t cRed = (color >> 8) & 0xF8;
    SPDR = cRed; // write hi uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = cGreen; // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = cBlue; // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    }
#endif
}

// send 16-bit pixel data to the controller
// note: inlined spi xfer for optimization
void Tft18::send565(color_t color, uint16_t count) {
    sendCmd(RAMWR);
    sendColors(color, count);
}

void Tft18::hardwareReset() {
    clearBit(TFT_RST_PORT, TFT_RST_BIT);        // pull PB0 (digital 8) low
    _delay_ms(1);                               // 1mS is enough
    setBit(TFT_RST_PORT, TFT_RST_BIT);          // return PB0 high
    _delay_ms(150);                             // wait 150mS for reset to finish
}

void Tft18::initDisplay(uint8_t rot) {
    hardwareReset();            // initialize display controller
    _delay_ms(150);               // wait 150mS for TFT driver circuits
    sendCmd(SLPOUT);           // take display out of sleep mode
    sendCmd(COLMOD);           // select color mode:
    sendByte(COLMOD_PARAM);    // mode 5 = 16bit pixels (RGB565)
//    sendCmd(GAMSET);           // select gama
//    sendByte(GAMSET_PARAM);
//    setOrientation(rot);
    sendCmd(DISPON);           // turn display on!
    setOrientation(rot);
}

void Tft18::sendSetAddrWindow(int x0, int y0, int x1, int y1) {
    sendCmd(CASET);      // set column range (x0,x1)
    sendWord((uint16_t)x0);
    sendWord((uint16_t)x1);
    sendCmd(RASET);      // set row range (y0,y1)
    sendWord((uint16_t)y0);
    sendWord((uint16_t)y1);
}

void Tft18::clearScreen() {
    sendSetAddrWindow(0, 0, maxX - 1, maxY - 1); // set window to entire display
    sendCmd(RAMWR);
    sendColors(background, TFT_XSIZE * TFT_YSIZE);
}

// ---------------------------------------------------------------------------
// SIMPLE GRAPHICS ROUTINES
//
// note: many routines have uint8_t parameters, to save space,
// but these can easily be changed to int params for larger displays.
uint8_t Tft18::blend(uint8_t fore, uint8_t back, alpha_t alpha) {
    if (alpha == 0) return back;
    if (alpha == 255) return fore;

    color_t color = (uint16_t)fore * alpha + (uint16_t)back * (255 - alpha);
    return (uint8_t)(color >> 8);
}

uint16_t Tft18::blendColors(color_t c1, color_t c2, alpha_t alpha) {
    uint8_t bRed = blend(RGB565_RED(c1), RGB565_RED(c2), alpha);
    uint8_t bGreen = blend(RGB565_GREEN(c1), RGB565_GREEN(c2), alpha);
    uint8_t bBlue = blend(RGB565_BLUE(c1), RGB565_BLUE(c2), alpha);

    return RGB565(bRed, bGreen, bBlue);
}

// draw pixel with alpha
// if alpha < 16 then it is treated as fully transparent
// if alpha >= 240 then it is treated as fully opaque
// else the current pixel color is blended with given color using proportional component blending
void Tft18::setPixel(int x, int y, color_t color, alpha_t alpha) {
    if (alpha >= 16) {
        sendSetAddrWindow(x, y, x, y);
        if (alpha >= 240) {
            send565(color, 1);
        } else {
            send565(blendColors(color, background, alpha), 1);
        }
    }
}

void Tft18::sendPixels(color_t color, alpha_t alpha, uint16_t count) {
    if (alpha >= 16) {
        if (alpha >= 240) {
            send565(color, count);
        } else {
            send565(blendColors(color, background, alpha), count);
        }
    }
}

void Tft18::hLine(int x0, int x1, int y, color_t color) {
    if (alpha >= 16) {
        uint16_t width = (uint16_t)(x1 - x0 + 1);
        sendSetAddrWindow(x0, y, x1, y);

        if (alpha >= 240) {
            send565(color, width);
        } else {
            sendPixels(color, alpha, width);
        }
    }
}

// draws a vertical line in given color
void Tft18::vLine(int x, int y0, int y1, color_t color) {
    if (alpha >= 16) {
        uint16_t height = (uint16_t)(y1 - y0 + 1);
        sendSetAddrWindow(x, y0, x, y1);

        if (alpha >= 240) {
            send565(color, height);
        } else {
            sendPixels(color, alpha, height);
        }
    }
}

// an elegant implementation of the Bresenham algorithm, with alpha
void Tft18::line(int x0, int y0, int x1, int y1, color_t color) {
    if (alpha >= 16) {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;
        for (;;) {
            setPixel(x0, y0, color, alpha);
            if (x0 == x1 && y0 == y1) break;
            e2 = err;
            if (e2 > -dx) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dy) {
                err += dx;
                y0 += sy;
            }
        }
    }
}

// draws a rectangle in given color
void Tft18::rect(int x0, int y0, int x1, int y1, color_t color) {
    hLine(x0, x1, y0, color);
    hLine(x0, x1, y1, color);
    vLine(x0, y0, y1, color);
    vLine(x1, y0, y1, color);
}

void Tft18::fillRect(int x0, int y0, int x1, int y1, color_t color) {
    if (alpha >= 16) {
        uint16_t area = (uint16_t)((x1 - x0 + 1) * (y1 - y0 + 1));
        sendSetAddrWindow(x0, y0, x1, y1);

        if (alpha >= 240) {
            send565(color, area);
        } else {
            sendPixels(color, alpha, area);
        }
    }
}

int Tft18::intSqrt(long val) {
    long mulMask = 0x0008000;
    int retVal = 0;
    if (val > 0) {
        while (mulMask != 0) {
            retVal |= mulMask;
            if (((uint32_t)retVal * retVal) > val)
                retVal &= ~mulMask;
            mulMask >>= 1;
        }
    }
    return retVal;
}

// draws circle quadrant(s) centered at x,y with given radius & color
// quad is a bit-encoded representation of which cartesian quadrant(s) to draw. // Remember that the y axis on our display is 'upside down':
// bit 0: draw quadrant I (lower right)
// bit 1: draw quadrant IV (upper right)
// bit 2: draw quadrant II (lower left)
// bit 3: draw quadrant III (upper left)
void Tft18::circleQuadrant(int xPos, int yPos, int radius, uint8_t quads, color_t color) {
    int x, xEnd = (707 * radius) / 1000 + 1;
    long r2 = (long)radius * radius;
    for (x = 0; x < xEnd; x++) {
        int y = intSqrt(r2 - x * x);
        if (quads & 0x01) {
            setPixel(xPos + x, yPos + y, color, alpha);   // lower right
            setPixel(xPos + y, yPos + x, color, alpha);
        }
        if (quads & 0x02) {
            setPixel(xPos + x, yPos - y, color, alpha);  // upper right
            setPixel(xPos + y, yPos - x, color, alpha);
        }
        if (quads & 0x04) {
            setPixel(xPos - x, yPos + y, color, alpha);  // lower left
            setPixel(xPos - y, yPos + x, color, alpha);
        }
        if (quads & 0x08) {
            setPixel(xPos - x, yPos - y, color, alpha);  // upper left
            setPixel(xPos - y, yPos - x, color, alpha);
        }
    }
}

// draws circle at x,y with given radius & color
void Tft18::circle(int xPos, int yPos, int radius, color_t color) {
    circleQuadrant(xPos, yPos, radius, 0x0F, color); // do all 4 quadrants
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void Tft18::roundRect(int x0, int y0, int x1, int y1, int r, color_t color) {
    hLine(x0 + r, x1 - r, y0, color); // top side
    hLine(x0 + r, x1 - r, y1, color); // bottom side
    vLine(x0, y0 + r, y1 - r, color); // left side
    vLine(x1, y0 + r, y1 - r, color); // right side
    circleQuadrant(x0 + r, y0 + r, r, 8, color); // upper left corner
    circleQuadrant(x1 - r, y0 + r, r, 2, color); // upper right corner
    circleQuadrant(x0 + r, y1 - r, r, 4, color); // lower left corner
    circleQuadrant(x1 - r, y1 - r, r, 1, color); // lower right corner
}

// draws filled circle at x,y with given radius & color
void Tft18::fillCircle(int xPos, int yPos, int radius, color_t color) {
    long r2 = radius * radius;
    for (int x = 0; x <= radius; x++) {
        int y = intSqrt(r2 - x * x);
        int y0 = yPos - y;
        int y1 = yPos + y;
        vLine(xPos + x, y0, y1, color);
        vLine(xPos - x, y0, y1, color);
    }
}

// draws an ellipse of given width & height
// two-part Bresenham method
// note: slight discontinuity between parts on some (narrow) ellipses.
void Tft18::ellipse(int x0, int y0, int width, int height, color_t color) {
    int a = width >> 1, b = height >> 1;
    int x = 0, y = b;
    long a2 = a * a * 2;
    long b2 = b * b * 2;
    long error = (long)a * a * b;
    long stopY = 0, stopX = a2 * b;
    while (stopY <= stopX) {
        setPixel(x0 + x, y0 + y, color, alpha);
        setPixel(x0 + x, y0 - y, color, alpha);
        setPixel(x0 - x, y0 + y, color, alpha);
        setPixel(x0 - x, y0 - y, color, alpha);
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
        setPixel(x0 + x, y0 + y, color, alpha);
        setPixel(x0 + x, y0 - y, color, alpha);
        setPixel(x0 - x, y0 + y, color, alpha);
        setPixel(x0 - x, y0 - y, color, alpha);
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
void Tft18::fillEllipse(int xPos, int yPos, int width, int height, color_t color) {
    int a = width >> 1, b = height >> 1; // get x & y radii
    int x1, x0 = a, y = 1;
    int dx = 0;
    long a2 = a * a, b2 = b * b;
    long a2b2 = (long)a2 * b2; // need longs: big numbers!
    hLine(xPos - a, xPos + a, yPos, color); // draw centerline
    while (y <= b) { // draw horizontal lines...
        for (x1 = x0 - (dx - 1); x1 > 0; x1--)
            if (b2 * x1 * x1 + a2 * y * y <= a2b2) break;

        dx = x0 - x1;
        x0 = x1;
        hLine(xPos - x0, xPos + x0, yPos + y, color);
        hLine(xPos - x0, xPos + x0, yPos - y, color);
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

void Tft18::charOffset(int x, int y) {
    colOffset = x;
    rowOffset = y;
    maxCol = (uint8_t)((maxX - x) / TFT_CHAR_WIDTH);
    maxRow = (uint8_t)((maxY - y) / TFT_CHAR_HEIGHT);
}

// position cursor on character x,y grid, where 0<x<20, 0<y<19.
void Tft18::gotoCharXY(int col, int row) {
    this->col = (uint8_t)col;
    this->row = (uint8_t)row;
}

// moves character cursor to next position, assuming portrait orientation
void Tft18::advanceCursor() {
    col++;             // advance x position
    if (col > maxCol) {    // beyond right margin?
        row++;         // go to next line
        col = 0;       // at left margin
    }

    if (row > maxRow)      // beyond bottom margin?
        row = 0;       // start at top again
}

// write ch to display X,Y coordinates using ASCII 5x7 font
#define OPTIMIZE_CHAR

// write ch to display X,Y coordinates using ASCII 5x7 font
void Tft18::putCh(char ch, int x, int y) {
#ifdef OPTIMIZE_CHAR
    uint8_t charBits[5];
    *((uint32_t *)(charBits)) = pgm_read_dword(FONT_CHARS[ch - 32]);
    charBits[4] = pgm_read_byte(FONT_CHARS[ch - 32] + 4);
#endif
    byte row, col, data, mask;
    sendSetAddrWindow(x, y, x + TFT_CHAR_X_PIXELS - 1, y + TFT_CHAR_Y_PIXELS - 1);
    sendCmd(RAMWR);
    for (row = 0, mask = 0x01; row < TFT_CHAR_Y_PIXELS; row++, mask <<= 1) {
        for (col = 0; col < TFT_CHAR_X_PIXELS; col++) {
#ifdef OPTIMIZE_CHAR
            data = *(charBits + col);
            sendColor((data & mask) ? foreground : background);
#else
            data = pgm_read_byte(&(FONT_CHARS[ch - 32][col]));
            sendColor((data & mask) ? foreground : background);
#endif
        }
    }
}

// writes character to display at current cursor position.
void Tft18::write(char ch) {
    putCh(ch, col * TFT_CHAR_WIDTH + colOffset, row * TFT_CHAR_HEIGHT + rowOffset);
    advanceCursor();
}

void Tft18::write(char ch, int count) {
    while (count-- > 0) {
        putCh(ch, col * TFT_CHAR_WIDTH + colOffset, row * TFT_CHAR_HEIGHT + rowOffset);
        advanceCursor();
    }
}

// writes string to display at current cursor position.
void Tft18::write(const char *text) {
    for (; *text; text++) // for all non-nul chars
        write(*text); // write the char
}

// writes integer i at current cursor position
void Tft18::write(int i) {
    char str[8]; // buffer for string result
    itoa(i, str, 10); // convert to string, base 10
    write(str);
}

void Tft18::writeDigit(int dig) {
    dig &= 0x0f;
    char c = (char)(dig > 9 ? 'A' - 10 + dig : '0' + dig);
    write(c);
}

// writes hexadecimal value of integer i at current cursor position
void Tft18::writeHex(int i) {
    // char str[8]; // buffer for string result
    // itoa(i, str, 16); // convert to base 16 (hex)
    // WriteString(str, color);

    writeDigit(i >> 12);
    writeDigit(i >> 8);
    writeDigit(i >> 4);
    writeDigit(i);
}

void Tft18::writeHex(long i) {
    writeHex((int)(i >> 16));
    write('.');
    writeHex((int)i);
}

void Tft18::write(long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

void Tft18::write(unsigned long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

