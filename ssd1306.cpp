#include "ssd1306.h"

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

#include <Arduino.h>
#include <avr/io.h>         // deal with port registers
#include <avr/interrupt.h>  // deal with interrupt calls
#include <avr/pgmspace.h>   // put character data into progmem

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

void Ssd1306::openSPI() {
    SPCR = SPCR_NORMAL;       // SPI enabled as Master, Mode0 at 4 MHz
    setBit(SPSR, SPI2X);      // double the SPI rate: 4-->8 MHz
    clearBit(SSD1306_CS_PORT, SSD1306_CS_BIT);  // select TFT CS

    clearBit(SSD1306_MOSI_PORT, SSD1306_MOSI_BIT); // disable pull up for PB3 MOSI when in input mode
}

void Ssd1306::closeSPI() {
    SPCR = 0x00;                        // clear SPI enable bit
    setBit(SSD1306_CS_PORT, SSD1306_CS_BIT);    // deselect TFT CS
}

Ssd1306::Ssd1306(uint8_t *pageBuffer, uint8_t typeFlags) {
    buffer = pageBuffer;
    rotation = SSD1306_ROT_0;
    pageStart = 0;
    flags = typeFlags;

    foreground = RGB_WHITE;
    background = RGB_BLACK;
}

void Ssd1306::setOrientation(uint8_t rot) {
    switch (SSD1306_SIZE(flags)) {
        default:
        case SSD1306_SIZE_096:
            setOrientation(rot, SSD1306_096_XSIZE, SSD1306_096_YSIZE, SSD1306_096_XOFFSET, SSD1306_096_YOFFSET);
            break;

        case SSD1306_SIZE_091:
            setOrientation(rot, SSD1306_091_XSIZE, SSD1306_091_YSIZE, SSD1306_091_XOFFSET, SSD1306_091_YOFFSET);
            break;
    }
}

void Ssd1306::initDisplay(uint8_t rot) {
    switch (SSD1306_SIZE(flags)) {
        default:
        case SSD1306_SIZE_096:
            initDisplay(rot, SSD1306_096_XSIZE, SSD1306_096_YSIZE, SSD1306_096_XOFFSET, SSD1306_096_YOFFSET,
                        (flags & (uint8_t) (SSD1306_INVERTED)) != 0);
            break;

        case SSD1306_SIZE_091:
            initDisplay(rot, SSD1306_091_XSIZE, SSD1306_091_YSIZE, SSD1306_091_XOFFSET, SSD1306_091_YOFFSET,
                        (flags & (uint8_t) (SSD1306_INVERTED)) != 0);
            break;
    }
}

void Ssd1306::setInverted() {
    sendCmd((uint8_t) ((flags & SSD1306_INVERTED) ? SSD1306_NORMALDISPLAY : SSD1306_INVERTDISPLAY));
}

void Ssd1306::clearInverted() {
    sendCmd((uint8_t) ((flags & SSD1306_INVERTED) ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY));
}

// Set the display orientation to 0,90,180,or 270 degrees
void Ssd1306::setOrientation(uint8_t rot, uint8_t xSize, uint8_t ySize, uint8_t xOffset, uint8_t yOffset) {
    switch (rot) {
        case SSD1306_ROT_90:
        case SSD1306_ROT_270:
            rotation = rot;
            maxX = ySize;
            maxY = xSize;
            break;

        default:
            rot = SSD1306_ROT_0;

        case SSD1306_ROT_180:
        case SSD1306_ROT_0:
            rotation = rot;
            maxX = xSize;
            maxY = ySize;
            break;
    }

    sizeX = xSize;  // non-rotated size
    sizeY = ySize;
    charOffset(0, 0);
}

void
Ssd1306::initDisplay(uint8_t rot, uint8_t xSize, uint8_t ySize, uint8_t xOffset, uint8_t yOffset, int8_t inversion) {
    Serial.print("initDisplay rot: ");
    Serial.print(rot);

    Serial.print(", xSize: ");
    Serial.print(xSize);

    Serial.print(", ySize: ");
    Serial.print(ySize);

    Serial.print(", inversion: ");
    Serial.println(inversion);

    hardwareReset();            // initialize display controller
    static const uint8_t PROGMEM init1[] = {
            SSD1306_DISPLAYOFF,
            DISP_CLK_DIV, 0x80,  // default after reset???
            SET_MULTIPLEX_RATIO,
    };

    sendCmdList(init1, sizeof(init1));
    sendByte(static_cast<uint8_t>(ySize - 1));          // send ratio

    static const uint8_t PROGMEM init2[] = {
            SSD1306_SETDISPLAYOFFSET,             // 0xD3
            0x0,                                  // no offset
            SSD1306_SETSTARTLINE | 0x0,           // line #0
            SSD1306_CHARGEPUMP                    // 0x8D
    };
    sendCmdList(init2, sizeof(init2));

    sendByte((flags & SSD1306_EXTERNALVCC) ? SSD1306_CHARGEPUMP_DISABLE : SSD1306_CHARGEPUMP_ENABLE);

    static const uint8_t PROGMEM init3[] = {
            SSD1306_MEMORYMODE,                   // 0x20
            0x00,                                 // 0x0, horizontal
            SSD1306_SEGREMAP_REVERSED,            // 0xA1
            SSD1306_COMSCANDEC                    // 0xC8
    };
    sendCmdList(init3, sizeof(init3));

    if (xSize == 128) {
        if ((ySize == 32)) {
            static const uint8_t PROGMEM init4a[] = {
                    SSD1306_SETCOMPINS,                 // 0xDA
                    0x02,                               // sequential, left/right remap disabled
                    SSD1306_SETCONTRAST,                // 0x81
                    0x8F};
            sendCmdList(init4a, sizeof(init4a));
        } else if (ySize == 64) {
            static const uint8_t PROGMEM init4b[] = {
                    SSD1306_SETCOMPINS,                 // 0xDA
                    0x12,                               // alternative com pins, left/right disabled
                    SSD1306_SETCONTRAST};              // 0x81
            sendCmdList(init4b, sizeof(init4b));
            sendByte((flags & SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
        } else {
            // not valid
        }
    } else if (xSize == 96 && ySize == 16) {
        static const uint8_t PROGMEM init4c[] = {
                SSD1306_SETCOMPINS,                 // 0xDA
                0x2,    // ada x12
                SSD1306_SETCONTRAST};              // 0x81
        sendCmdList(init4c, sizeof(init4c));
        sendByte((flags & SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);
    } else {
        // Other screen varieties -- TBD
    }

    sendByte(SSD1306_SETPRECHARGE); // 0xd9
    sendByte((flags & SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

    sendByte((uint8_t) (inversion ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY));

    static const uint8_t PROGMEM init5[] = {
            SSD1306_SETVCOMDETECT,               // 0xDB
            0x40,                                // reset default
            SSD1306_DISPLAYALLON_RESUME,         // 0xA4
//            SSD1306_NORMALDISPLAY,               // 0xA6
            SSD1306_DEACTIVATE_SCROLL,
//            SSD1306_DISPLAYALLON,                   // all on for test
            SSD1306_DISPLAYON // Main screen turn on
    };
    sendCmdList(init5, sizeof(init5));
    endCmd();
    Serial.println();

    setOrientation(rot, xSize, ySize, xOffset, yOffset);
    _delay_ms(100);

    clearScreen();

//    sendSetAddrWindow(0, 0, xSize - 1, ySize - 1);
//    uint16_t count = static_cast<uint16_t>(xSize * ySize / 8);
//    uint16_t i = count / 1;
//    for (; i-- > 0;) {
//        SPDR = static_cast<uint8_t>(i);            // initiate transfer
//        while (!(SPSR & 0x80)); // wait for transfer to complete
//    }

//    sendCmd(SSD1306_DISPLAYON);            // take display out of sleep mode
    //_delay_ms(100);             // wait 100mS for reset to finish
//    sendCmd(SSD1306_DISPLAYALLON);        // set all on to test
//    sendCmd(SSD1306_NORMALDISPLAY);
//    startCmd(SSD1306_MEMORYMODE);
//    sendByte(0);                // horizontal addressing mode
//    endCmd();
}

void Ssd1306::sendByte(uint8_t byte) {
//    Serial.print(" 0x");
//    Serial.print(byte, 16);
    SPDR = byte;            // initiate transfer
    while (!(SPSR & 0x80)); // wait for transfer to complete
}

void Ssd1306::sendBytes(uint8_t byte, uint16_t count) {
//    Serial.print(" 0x");
//    Serial.print(byte, 16);
//    Serial.print(" count: ");
//    Serial.println(count);
    while (count-- > 0) {
        SPDR = byte;            // initiate transfer
        while (!(SPSR & 0x80)); // wait for transfer to complete
    }
}

void Ssd1306::sendBytes(uint8_t *bytes, uint16_t count) {
    while (count-- > 0) {
        SPDR = *bytes++;            // initiate transfer
        while (!(SPSR & 0x80)); // wait for transfer to complete
    }
}

void Ssd1306::startCmd(uint8_t cmd) {
    clearBit(SSD1306_DC_PORT, SSD1306_DC_BIT);  // B1=DC; 0=command, 1=data
    sendByte(cmd);
}

void Ssd1306::endCmd() {
    setBit(SSD1306_DC_PORT, SSD1306_DC_BIT); // return DC high
}

void Ssd1306::sendCmd(uint8_t cmd) {
    startCmd(cmd);
    endCmd();
}

void Ssd1306::sendCmdList(const uint8_t *p, uint8_t count) {
    clearBit(SSD1306_DC_PORT, SSD1306_DC_BIT);  // B1=DC; 0=command, 1=data
    while (count--) {
        sendByte(pgm_read_byte(p++));
    }
}

//void Ssd1306::sendWord(uint16_t word) {
//    SPDR = static_cast<uint8_t>(word >> 8); // write hi uint8_t
//    while (!(SPSR & 0x80)); // wait for transfer to complete
//    SPDR = static_cast<uint8_t>(word & 0xFF); // write lo uint8_t
//    while (!(SPSR & 0x80)); // wait for transfer to complete
//}
//
//void Ssd1306::sendColor(color_t color) {
//    SPDR = static_cast<uint8_t>(color >> 8); // write hi uint8_t
//    while (!(SPSR & 0x80)); // wait for transfer to complete
//    SPDR = static_cast<uint8_t>(color & 0xFF); // write lo uint8_t
//    while (!(SPSR & 0x80)); // wait for transfer to complete
//}
//
//void Ssd1306::sendColors(color_t color, uint16_t count) {
//    uint8_t byte1 = static_cast<uint8_t>(color >> 8); // write hi uint8_t
//    uint8_t byte2 = static_cast<uint8_t>(color & 0xFF); // write low uint8_t
//    while (count--) {
//        SPDR = byte1; // write hi uint8_t
//        while (!(SPSR & 0x80)); // wait for transfer to complete
//        SPDR = byte2; // write lo uint8_t
//        while (!(SPSR & 0x80)); // wait for transfer to complete
//    }
//}
//
//// send 16-bit pixel data to the controller
//// note: inlined spi xfer for optimization
//void Ssd1306::send565(color_t color, uint16_t count) {
////    sendCmd(RAMWR);
//    sendColors(color, count);
//}

void Ssd1306::hardwareReset() {
    _delay_ms(1);                                       // 1ms is enough
    clearBit(SSD1306_RST_PORT, SSD1306_RST_BIT);        // pull PB0 (digital 8) low
    _delay_ms(10);                                      // 10ms is enough
    setBit(SSD1306_RST_PORT, SSD1306_RST_BIT);          // return PB0 high
}

void Ssd1306::sendSetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
//    Serial.print("setAddrWindow x: ");
//    Serial.print(static_cast<uint8_t>(x0));
//    Serial.print("-");
//    Serial.print(static_cast<uint8_t>(x1));
//    Serial.print(" pages: ");
//    Serial.print(static_cast<uint8_t>(y0 / 8));
//    Serial.print("-");
//    Serial.print(static_cast<uint8_t>(y1 / 8));
//    Serial.print(": ");

    startCmd(PAGE_COLS);      // set column range (x0,x1)
    sendByte(static_cast<uint8_t>(x0));
    sendByte(static_cast<uint8_t>(x1));
    startCmd(PAGE_ADDR);      // set row start (y0,y1)
    sendByte(static_cast<uint8_t>(y0 / 8));
    sendByte(static_cast<uint8_t>(y1 / 8));
    endCmd();

//    Serial.println();
}

void Ssd1306::clearScreen() {
//    Serial.print("clearScreen ");
    sendSetAddrWindow(0, 0, sizeX - 1, sizeY - 1); // set window to entire display
    uint16_t count = static_cast<uint16_t>(sizeX * sizeY / 8);
    sendBytes((uint8_t) (background ? 0xff : 0), count);
}

void Ssd1306::startPage(uint8_t page) {
    pageStart = static_cast<uint8_t>((page & 7) << 3);
    memset(buffer, 0, sizeX);
}

void Ssd1306::updatePage() {
//    Serial.print("updatePage ");
    sendSetAddrWindow(0, pageStart, sizeX - 1, pageStart + 7);
    sendBytes(buffer, sizeX);
}

void Ssd1306::startUpdate() {
    pageStart = static_cast<uint8_t>(-8);
}

bool Ssd1306::nextPage() {
//    Serial.print("nextPage: ");
//    Serial.print(pageStart);
//    uint8_t nextStart = static_cast<uint8_t>(pageStart + 8);
//    Serial.print(" -> ");
//    Serial.println(nextStart);

    if (pageStart < sizeY) {
        updatePage();
    }

    pageStart += 8;

    if (pageStart < sizeY) {
        memset(buffer, background ? 0xff : 0, sizeX);
        return true;
    }
    return false;
}

bool Ssd1306::isBGR() {
    return false;
}

color_t Ssd1306::rgb(uint8_t r, uint8_t g, uint8_t b) {
    return _RGB(r, g, b);
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
void Ssd1306::setPixel(int x, int y, color_t color) {
    actualCoords(x, y);

    if (x >= 0 && x < sizeX && y >= pageStart && y < pageStart + 8) {
        uint8_t mask = static_cast<uint8_t>(1 << (y - pageStart));

        if (color) {
            buffer[x] |= mask;
        } else {
            buffer[x] &= ~mask;
        }
    }
}

void Ssd1306::actualCoords(int &x, int &y) {
    int aX, aY;

    switch (rotation) {
        default:
        case SSD1306_ROT_0:
            return;

        case SSD1306_ROT_90:
            aX = maxY - 1 - y;
            aY = x;
            break;

        case SSD1306_ROT_270:
            aX = y;
            aY = maxX - 1 - x;
            break;

        case SSD1306_ROT_180:
            aX = maxX - 1 - x;
            aY = maxY - 1 - y;
            break;
    }

    x = aX;
    y = aY;
}

bool Ssd1306::isInPage(int x0, int x1, int y0, int y1) {
    if (x0 > x1) {
        int t = x0;
        x0 = x1;
        x1 = t;
    }

    if (y0 > y1) {
        int t = y0;
        y0 = y1;
        y1 = t;
    }

    switch (rotation) {
        default:
        case SSD1306_ROT_0:
            return y1 >= pageStart && y0 < pageStart + 8;
        case SSD1306_ROT_180:
            return maxY - 1 - y0 >= pageStart && maxY - 1 - y1 < pageStart + 8;
        case SSD1306_ROT_90:
            return x1 >= pageStart && x0 < pageStart + 8;
        case SSD1306_ROT_270:
            return maxX - 1 - x0 >= pageStart && maxX - 1 - x1 < pageStart + 8;
    }
}

bool Ssd1306::isInPageTrimX(int &x0, int &x1, int y0, int y1) {
    switch (rotation) {
        case SSD1306_ROT_90:
            if (!(x1 >= pageStart && x0 < pageStart + 8)) return false;
            if (x1 < pageStart) x1 = pageStart;
            if (x0 > pageStart + 7) x0 = pageStart + 7;
            return true;

        case SSD1306_ROT_270:
            if (!(maxX - 1 - x0 >= pageStart && maxX - 1 - x1 < pageStart + 8)) return false;
            if (maxX - 1 - x0 < pageStart) x0 = maxX - 1 - pageStart;
            if (maxX - 1 - x1 > pageStart + 7) x1 = maxX - 1 - (pageStart + 7);
            return true;

        default:
        case SSD1306_ROT_0:
            return y1 >= pageStart && y0 < pageStart + 8;

        case SSD1306_ROT_180:
            return maxY - 1 - y0 >= pageStart && maxY - 1 - y1 < pageStart + 8;
    }
}

bool Ssd1306::isInPageTrimY(int x0, int x1, int &y0, int &y1) {
    switch (rotation) {
        case SSD1306_ROT_90:
            return x1 >= pageStart && x0 < pageStart + 8;
        case SSD1306_ROT_270:
            return maxX - 1 - x0 >= pageStart && maxX - 1 - x1 < pageStart + 8;

        default:
        case SSD1306_ROT_0:
            if (!(y1 >= pageStart && y0 < pageStart + 8)) return false;
            if (y1 < pageStart) y1 = pageStart;
            if (y0 > pageStart + 7) y0 = pageStart + 7;
            return true;

        case SSD1306_ROT_180:
            if (!(maxY - 1 - y0 >= pageStart && maxY - 1 - y1 < pageStart + 8)) return false;
            if (maxY - 1 - y0 < pageStart) y0 = maxY - 1 - pageStart;
            if (maxY - 1 - y1 > pageStart + 7) y1 = maxY - 1 - (pageStart + 7);
            return true;
    }
}

bool Ssd1306::isInPageTrimXY(int &x0, int &x1, int &y0, int &y1) {
    switch (rotation) {
        case SSD1306_ROT_90:
            if (!(x1 >= pageStart && x0 < pageStart + 8)) return false;
            if (x1 < pageStart) x1 = pageStart;
            if (x0 > pageStart + 7) x0 = pageStart + 7;
            return true;

        case SSD1306_ROT_270:
            if (!(maxX - 1 - x0 >= pageStart && maxX - 1 - x1 < pageStart + 8)) return false;
            if (maxX - 1 - x0 < pageStart) x0 = maxX - 1 - pageStart;
            if (maxX - 1 - x1 > pageStart + 7) x1 = maxX - 1 - (pageStart + 7);
            return true;

        default:
        case SSD1306_ROT_0:
            if (!(y1 >= pageStart && y0 < pageStart + 8)) return false;
            if (y1 < pageStart) y1 = pageStart;
            if (y0 > pageStart + 7) y0 = pageStart + 7;
            return true;

        case SSD1306_ROT_180:
            if (!(maxY - 1 - y0 >= pageStart && maxY - 1 - y1 < pageStart + 8)) return false;
            if (maxY - 1 - y0 < pageStart) y0 = maxY - 1 - pageStart;
            if (maxY - 1 - y1 > pageStart + 7) y1 = maxY - 1 - (pageStart + 7);
            return true;
    }
}

void Ssd1306::hLine(int x0, int x1, int y, color_t color) {
    if (isInPageTrimX(x0, x1, y, y)) {
        while (x0 <= x1) {
            setPixel(x0++, y, color);
        }
    }
}

// draws a vertical line in given color
void Ssd1306::vLine(int x, int y0, int y1, color_t color) {
    if (isInPageTrimY(x, x, y0, y1)) {
        while (y0 <= y1) {
            setPixel(x, y0++, color);
        }
    }
}

// an elegant implementation of the Bresenham algorithm, with alpha
void Ssd1306::line(int x0, int y0, int x1, int y1, color_t color) {
    if (isInPage(x0, x1, y0, y1)) {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;

        int xs = min(x0, x1);
        int ys = min(y0, y1);

        int err = (dx > dy ? dx : -dy) / 2, e2;
        for (;;) {
            setPixel(x0, y0, color);
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
void Ssd1306::rect(int x0, int y0, int x1, int y1, color_t color) {
    hLine(x0, x1, y0, color);
    hLine(x0, x1, y1, color);
    vLine(x0, y0, y1, color);
    vLine(x1, y0, y1, color);
}

void Ssd1306::fillRect(int x0, int y0, int x1, int y1, color_t color) {
//    sendSetAddrWindow(x0, y0, x1, y1);
//    uint16_t area = static_cast<uint16_t>((x1 - x0 + 1) * (y1 - y0 + 1));
//    send565(color, area);
    if (isInPageTrimXY(x0, x1, y0, y1)) {
        while (y0 <= y1) {
            hLine(x0, x1, y0++, color);
        }
    }
}

int Ssd1306::intSqrt(long val) {
    long mulMask = 0x0008000;
    int retVal = 0;
    if (val > 0) {
        while (mulMask != 0) {
            retVal |= mulMask;
            if (((uint32_t) retVal * retVal) > val)
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
void Ssd1306::circleQuadrant(int cx, int cy, int radius, uint8_t quads, color_t color) {
    int x, xEnd = (707 * radius) / 1000 + 1;
    long r2 = (long) radius * radius;
    for (x = 0; x < xEnd; x++) {
        int y = intSqrt(r2 - x * x);
        if (quads & 0x01) {
            setPixel(cx + x, cy + y, color);   // lower right
            setPixel(cx + y, cy + x, color);
        }
        if (quads & 0x02) {
            setPixel(cx + x, cy - y, color);  // upper right
            setPixel(cx + y, cy - x, color);
        }
        if (quads & 0x04) {
            setPixel(cx - x, cy + y, color);  // lower left
            setPixel(cx - y, cy + x, color);
        }
        if (quads & 0x08) {
            setPixel(cx - x, cy - y, color);  // upper left
            setPixel(cx - y, cy - x, color);
        }
    }
}

// draws circle at x,y with given radius & color
void Ssd1306::circle(int cx, int cy, int radius, color_t color) {
    circleQuadrant(cx, cy, radius, 0x0F, color); // do all 4 quadrants
}

// draws a rounded rectangle with corner radius r.
// coordinates: top left = x0,y0; bottom right = x1,y1
void Ssd1306::roundRect(int x0, int y0, int x1, int y1, int r, color_t color) {
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
void Ssd1306::fillCircle(int cx, int cy, int radius, color_t color) {
    long r2 = radius * radius;
    for (int x = 0; x <= radius; x++) {
        int y = intSqrt(r2 - x * x);
        int y0 = cy - y;
        int y1 = cy + y;
        vLine(cx + x, y0, y1, color);
        vLine(cx - x, y0, y1, color);
    }
}

// draws an ellipse of given width & height
// two-part Bresenham method
// note: slight discontinuity between parts on some (narrow) ellipses.
void Ssd1306::ellipse(int cx, int cy, int width, int height, color_t color) {
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
void Ssd1306::fillEllipse(int cx, int cy, int width, int height, color_t color) {
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

void Ssd1306::charOffset(uint8_t x, uint8_t y) {
    colOffset = static_cast<uint8_t>(x);
    rowOffset = static_cast<uint8_t>(y);
    maxCols = static_cast<uint8_t>((maxX - x) / SSD1306_CHAR_WIDTH);
    maxRows = static_cast<uint8_t>((maxY - y) / SSD1306_CHAR_HEIGHT);
}

// position cursor on character x,y grid, where 0<x<20, 0<y<19.
void Ssd1306::gotoCharXY(int col, int row) {
    this->col = static_cast<int16_t>(col);
    this->row = static_cast<int16_t>(row);
    colOffset = 0;
    rowOffset = 0;
}

// moves character cursor to next position, assuming portrait orientation
void Ssd1306::advanceCursor() {
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
void Ssd1306::putCh(char ch, int x, int y) {
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
void Ssd1306::write(char ch) {
    if (col >= 0 && col <= maxCols && row >= 0 && row <= maxRows) {
        putCh(ch, col * SSD1306_CHAR_WIDTH + colOffset, row * SSD1306_CHAR_HEIGHT + rowOffset);
    }
    advanceCursor();
}

void Ssd1306::write(char ch, int count) {
    while (count-- > 0) {
        if (col >= 0 && col <= maxCols && row >= 0 && row <= maxRows) {
            putCh(ch, col * SSD1306_CHAR_WIDTH + colOffset, row * SSD1306_CHAR_HEIGHT + rowOffset);
        }
        advanceCursor();
    }
}

// writes string to display at current cursor position.
void Ssd1306::write(const char *text) {
    while (*text) {
        write(*text++);
    }
}

// writes integer i at current cursor position
void Ssd1306::write(int i) {
    char str[8];
    itoa(i, str, 10);
    write(str);
}

void Ssd1306::writeDigit(int dig) {
    dig &= 0x0f;
    char c = (char) (dig > 9 ? 'A' - 10 + dig : '0' + dig);
    write(c);
}

// writes hexadecimal value of integer i at current cursor position
void Ssd1306::writeHex(int i) {
    writeDigit(i >> 12);
    writeDigit(i >> 8);
    writeDigit(i >> 4);
    writeDigit(i);
}

void Ssd1306::writeHex(long i) {
    writeHex((int) (i >> 16));
    write('.');
    writeHex((int) i);
}

void Ssd1306::write(long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

void Ssd1306::write(unsigned long i) {
    char buff[11];
    ltoa(i, buff, 10);
    write(buff);
}

