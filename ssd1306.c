#include "ssd1306_display.h"
#include "Arduino.h"

#include "twi.h"
#include "ssd1306.h"
#include "ssd1306_gfx.h"
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

uint8_t ssd1306_sendBuffer[TWI_BUFFER_LENGTH];

void ssd1306_initDisplay(uint8_t configFlags) {
    ssd1306_sendPos = 0;
    ssd1306_clearDisplay();
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

    ssd1306_twiByte(SSD1306_SETPRECHARGE); // 0xd9
    ssd1306_twiByte((configFlags & SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
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
    ssd1306_sendCmd(SSD1306_INVERTDISPLAY);
}

void ssd1306_clearInverted() {
    ssd1306_sendCmd(SSD1306_NORMALDISPLAY);
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
    ssd1306_endTwiFrame();
}

