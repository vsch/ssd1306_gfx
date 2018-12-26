#include "st7735.h"

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
const uint8_t FONT_CHARS[96][ST7735_CHAR_X_PIXELS] PROGMEM = {
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

St7735::St7735(uint8_t typeFlags) {
    flags = typeFlags;

    foreground = RGB_WHITE;
    background = RGB_BLACK;
    alpha = 255;
}

void St7735::setOrientation(uint8_t rot) {
    switch (ST7735_SIZE(flags)) {
        case ST7735_SIZE_096:
            setOrientation(rot, ST7735_096_XSIZE, ST7735_096_YSIZE, ST7735_096_XOFFSET, ST7735_096_YOFFSET);
            break;

        default:
        case ST7735_SIZE_180:
            setOrientation(rot, ST7735_180_XSIZE, ST7735_180_YSIZE, ST7735_180_XOFFSET, ST7735_180_YOFFSET);
            break;
    }
}

void St7735::initDisplay(uint8_t rot) {
    switch (ST7735_SIZE(flags)) {
        case ST7735_SIZE_096:
            initDisplay(rot, ST7735_096_XSIZE, ST7735_096_YSIZE, ST7735_096_XOFFSET, ST7735_096_YOFFSET,
                        (flags & (uint8_t)(ST7735_INVERTED)) != 0);
            break;

        default:
        case ST7735_SIZE_180:
            initDisplay(rot, ST7735_180_XSIZE, ST7735_180_YSIZE, ST7735_180_XOFFSET, ST7735_180_YOFFSET,
                        (flags & (uint8_t)(ST7735_INVERTED)) != 0);
            break;
    }
}

void St7735::setInverted() {
    sendCmd((uint8_t)((flags & ST7735_INVERTED) ? INVOFF : INVON));
}

void St7735::clearInverted() {
    sendCmd((uint8_t)((flags & ST7735_INVERTED) ? INVON : INVOFF));
}

// Set the display orientation to 0,90,180,or 270 degrees
void St7735::setOrientation(uint8_t rot, uint16_t xSize, uint16_t ySize, uint8_t xOffset, uint8_t yOffset) {
    uint8_t arg;
    switch (rot) {
        case ST7735_ROT_90:
        case ST7735_ROT_270:
            arg = (uint8_t)((rot == ST7735_ROT_90) ? 0x60 : 0xA0);
            maxX = ySize;
            maxY = xSize;
            offsetX = yOffset;
            offsetY = xOffset;
            break;

        case ST7735_ROT_180:
        case ST7735_ROT_0:
        default:
            arg = (uint8_t)((rot == ST7735_ROT_180) ? 0xC0 : 0x00);
            maxX = xSize;
            maxY = ySize;
            offsetX = xOffset;
            offsetY = yOffset;
            break;
    }
    sendCmd(MADCTL);
    sendByte(arg);

    charOffset(0, 0);
}

void
St7735::initDisplay(uint8_t rot, uint16_t xSize, uint16_t ySize, uint8_t xOffset, uint8_t yOffset, int8_t inversion) {
    hardwareReset();            // initialize display controller
    _delay_ms(150);               // wait 150mS for TFT driver circuits
    sendCmd(SLPOUT);           // take display out of sleep mode
    sendCmd(COLMOD);           // select color mode:
    sendByte(COLMOD_PARAM);    // mode 5 = 16bit pixels (RGB565)
    sendCmd(GAMSET);           // select gama
    sendByte(GAMSET_PARAM);
    sendCmd((uint8_t)(inversion ? INVON : INVOFF));
    sendCmd(DISPON);           // turn display on!
    setOrientation(rot, xSize, ySize, xOffset, yOffset);
}

void St7735::setGama(uint8_t gama) {
    gama &= 0x0f;
    if (!gama) {
        gama = 1;
    }

    sendCmd(GAMSET);       // select gama
    sendByte(gama);
}

void St7735::openSPI() {
    SPCR = SPCR_NORMAL;       // SPI enabled as Master, Mode0 at 4 MHz
    setBit(SPSR, SPI2X);      // double the SPI rate: 4-->8 MHz
    clearBit(ST7735_CS_PORT, ST7735_CS_BIT);  // select TFT CS

    clearBit(ST7735_MOSI_PORT, ST7735_MOSI_BIT); // disable pull up for PB3 MOSI when in input mode
}

void St7735::closeSPI() {
    SPCR = 0x00;                        // clear SPI enable bit
    setBit(ST7735_CS_PORT, ST7735_CS_BIT);    // deselect TFT CS
}

void St7735::sendByte(uint8_t byte) {
    SPDR = byte;            // initiate transfer
    while (!(SPSR & 0x80)); // wait for transfer to complete
}

void St7735::sendCmd(uint8_t cmd) {
//    setBit(ST7735_CS_PORT, ST7735_CS);      // deselect TFT CS
    clearBit(ST7735_DC_PORT, ST7735_DC_BIT);  // B1=DC; 0=command, 1=data
//    clearBit(ST7735_CS_PORT, ST7735_CS);  // select TFT CS
    sendByte(cmd);
    setBit(ST7735_DC_PORT, ST7735_DC_BIT); // return DC high
}

void St7735::sendWord(uint16_t word) {
    SPDR = static_cast<uint8_t>(word >> 8); // write hi uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = static_cast<uint8_t>(word & 0xFF); // write lo uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
}

void St7735::sendColor(color_t color) {
#if COLMOD_PARAM == 5
    SPDR = static_cast<uint8_t>(color >> 8); // write hi uint8_t
    while (!(SPSR & 0x80)); // wait for transfer to complete
    SPDR = static_cast<uint8_t>(color & 0xFF); // write lo uint8_t
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

void St7735::sendColors(color_t color, uint16_t count) {
#if COLMOD_PARAM == 5
    uint8_t byte1 = static_cast<uint8_t>(color >> 8); // write hi uint8_t
    uint8_t byte2 = static_cast<uint8_t>(color & 0xFF); // write low uint8_t
    while (count--) {
        SPDR = byte1; // write hi uint8_t
        while (!(SPSR & 0x80)); // wait for transfer to complete
        SPDR = byte2; // write lo uint8_t
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
void St7735::send565(color_t color, uint16_t count) {
    sendCmd(RAMWR);
    sendColors(color, count);
}

void St7735::hardwareReset() {
    clearBit(ST7735_RST_PORT, ST7735_RST_BIT);        // pull PB0 (digital 8) low
    _delay_ms(1);                               // 1mS is enough
    setBit(ST7735_RST_PORT, ST7735_RST_BIT);          // return PB0 high
    _delay_ms(150);                             // wait 150mS for reset to finish
}

void St7735::sendSetAddrWindow(int x0, int y0, int x1, int y1) {
    sendCmd(CASET);      // set column range (x0,x1)
    sendWord(static_cast<uint16_t>(x0 + offsetX));
    sendWord(static_cast<uint16_t>(x1 + offsetX));
    sendCmd(RASET);      // set row range (y0,y1)
    sendWord(static_cast<uint16_t>(y0 + offsetY));
    sendWord(static_cast<uint16_t>(y1 + offsetY));
}

void St7735::clearScreen() {
    sendSetAddrWindow(0, 0, maxX - 1, maxY - 1); // set window to entire display
    sendCmd(RAMWR);
    sendColors(background, static_cast<uint16_t>(maxX * maxY));
}

bool St7735::isBGR() {
    return (flags & ST7735_BGR) != 0;
}

uint16_t St7735::rgb(uint8_t r, uint8_t g, uint8_t b) {
    if ((flags & ST7735_BGR) != 0) {
        // swap R & B
        return _RGB(b, g, r);
    }
    return _RGB(r, g, b);
}

// ---------------------------------------------------------------------------
// SIMPLE GRAPHICS ROUTINES
//
// note: many routines have uint8_t parameters, to save space,
// but these can easily be changed to int params for larger displays.
uint8_t St7735::blend(uint8_t fore, uint8_t back, alpha_t alpha) {
    if (alpha == 0) return back;
    if (alpha == 255) return fore;

    color_t color = (uint16_t)fore * alpha + (uint16_t)back * (255 - alpha);
    return static_cast<uint8_t>(color >> 8);
}

uint16_t St7735::blendColors(color_t c1, color_t c2, alpha_t alpha) {
    uint8_t bRed = blend(RGB565_RED(c1), RGB565_RED(c2), alpha);
    uint8_t bGreen = blend(RGB565_GREEN(c1), RGB565_GREEN(c2), alpha);
    uint8_t bBlue = blend(RGB565_BLUE(c1), RGB565_BLUE(c2), alpha);

    return RGB565(bRed, bGreen, bBlue);
}

// draw pixel with alpha
// if alpha < 16 then it is treated as fully transparent
// if alpha >= 240 then it is treated as fully opaque
// else the current pixel color is blended with given color using proportional component blending
void St7735::setPixel(int x, int y, color_t color, alpha_t alpha) {
    if (alpha >= 16) {
        sendSetAddrWindow(x, y, x, y);
        if (alpha >= 240) {
            send565(color, 1);
        } else {
            send565(blendColors(color, background, alpha), 1);
        }
    }
}

void St7735::sendPixels(color_t color, alpha_t alpha, uint16_t count) {
    if (alpha >= 16) {
        if (alpha >= 240) {
            send565(color, count);
        } else {
            send565(blendColors(color, background, alpha), count);
        }
    }
}

void St7735::hLine(int x0, int x1, int y, color_t color) {
    if (alpha >= 16) {
        sendSetAddrWindow(x0, y, x1, y);
        uint16_t width = static_cast<uint16_t>(x1 - x0 + 1);
        if (alpha >= 240) {
            send565(color, width);
        } else {
            sendPixels(color, alpha, width);
        }
    }
}

// draws a vertical line in given color
void St7735::vLine(int x, int y0, int y1, color_t color) {
    if (alpha >= 16) {
        sendSetAddrWindow(x, y0, x, y1);
        uint16_t height = static_cast<uint16_t>(y1 - y0 + 1);
        if (alpha >= 240) {
            send565(color, height);
        } else {
            sendPixels(color, alpha, height);
        }
    }
}

// an elegant implementation of the Bresenham algorithm, with alpha
void St7735::line(int x0, int y0, int x1, int y1, color_t color) {
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
void St7735::rect(int x0, int y0, int x1, int y1, color_t color) {
    hLine(x0, x1, y0, color);
    hLine(x0, x1, y1, color);
    vLine(x0, y0, y1, color);
    vLine(x1, y0, y1, color);
}

void St7735::fillRect(int x0, int y0, int x1, int y1, color_t color) {
    if (alpha >= 16) {
        sendSetAddrWindow(x0, y0, x1, y1);
        uint16_t area = static_cast<uint16_t>((x1 - x0 + 1) * (y1 - y0 + 1));
        if (alpha >= 240) {
            send565(color, area);
        } else {
            sendPixels(color, alpha, area);
        }
    }
}

int St7735::intSqrt(long val) {
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
void St7735::circleQuadrant(int xPos, int yPos, int radius, uint8_t quads, color_t color) {
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
void St7735::circle(int xPos, int yPos, int radius, color_t color) {
    circleQuadrant(xPos, yPos, radius, 0x0F, color); // do all 4 quadrants
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void St7735::roundRect(int x0, int y0, int x1, int y1, int r, color_t color) {
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
void St7735::fillCircle(int xPos, int yPos, int radius, color_t color) {
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
void St7735::ellipse(int x0, int y0, int width, int height, color_t color) {
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
void St7735::fillEllipse(int xPos, int yPos, int width, int height, color_t color) {
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

void St7735::charOffset(uint8_t x, uint8_t y) {
    colOffset = static_cast<uint8_t>(x);
    rowOffset = static_cast<uint8_t>(y);
    maxCols = static_cast<uint8_t>((maxX - x) / ST7735_CHAR_WIDTH);
    maxRows = static_cast<uint8_t>((maxY - y) / ST7735_CHAR_HEIGHT);
}

// position cursor on character x,y grid, where 0<x<20, 0<y<19.
void St7735::gotoCharXY(int col, int row) {
    this->col = static_cast<int16_t>(col);
    this->row = static_cast<int16_t>(row);
}

// moves character cursor to next position, assuming portrait orientation
void St7735::advanceCursor() {
    col++;             // advance x position
    if (col > maxCols) {    // beyond right margin?
        row++;         // go to next line
        col = 0;       // at left margin
    }

    if (row > maxRows)      // beyond bottom margin?
        row = 0;       // start at top again
}

// write ch to display X,Y coordinates using ASCII 5x7 font
#define OPTIMIZE_CHAR

// write ch to display X,Y coordinates using ASCII 5x7 font
void St7735::putCh(char ch, int x, int y) {
#ifdef OPTIMIZE_CHAR
    uint8_t charBits[5];
    *((uint32_t *)(charBits)) = pgm_read_dword(FONT_CHARS[ch - 32]);
    charBits[4] = pgm_read_byte(FONT_CHARS[ch - 32] + 4);
#endif
    uint8_t row, col, data, mask;
    sendSetAddrWindow(x, y, x + ST7735_CHAR_X_PIXELS - 1, y + ST7735_CHAR_Y_PIXELS - 1);
    sendCmd(RAMWR);
    for (row = 0, mask = 0x01; row < ST7735_CHAR_Y_PIXELS; row++, mask <<= 1) {
        for (col = 0; col < ST7735_CHAR_X_PIXELS; col++) {
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
void St7735::write(char ch) {
    if (col >= 0 && col <= maxCols && row >= 0 && row <= maxRows) {
        putCh(ch, col * ST7735_CHAR_WIDTH + colOffset, row * ST7735_CHAR_HEIGHT + rowOffset);
    }
    advanceCursor();
}

void St7735::write(char ch, int count) {
    while (count-- > 0) {
        if (col >= 0 && col <= maxCols && row >= 0 && row <= maxRows) {
            putCh(ch, col * ST7735_CHAR_WIDTH + colOffset, row * ST7735_CHAR_HEIGHT + rowOffset);
        }
        advanceCursor();
    }
}

// writes string to display at current cursor position.
void St7735::write(const char *text) {
    for (; *text; text++) // for all non-nul chars
        write(*text); // write the char
}

// writes integer i at current cursor position
void St7735::write(int i) {
    char str[8]; // buffer for string result
    itoa(i, str, 10); // convert to string, base 10
    write(str);
}

void St7735::writeDigit(int dig) {
    dig &= 0x0f;
    char c = (char)(dig > 9 ? 'A' - 10 + dig : '0' + dig);
    write(c);
}

// writes hexadecimal value of integer i at current cursor position
void St7735::writeHex(int i) {
    // char str[8]; // buffer for string result
    // itoa(i, str, 16); // convert to base 16 (hex)
    // WriteString(str, color);

    writeDigit(i >> 12);
    writeDigit(i >> 8);
    writeDigit(i >> 4);
    writeDigit(i);
}

void St7735::writeHex(long i) {
    writeHex((int)(i >> 16));
    write('.');
    writeHex((int)i);
}

void St7735::write(long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

void St7735::write(unsigned long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

