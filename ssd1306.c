#include "Arduino.h"
#include "ssd1306.h"
#include "ssd1306_display.h"
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

#ifndef CONSOLE_DEBUG
uint8_t ssd1306_send_buffer[TWI_BUFFER_LENGTH];
uint8_t ssd1306_send_pos;
ssd1306_display_buffer_t ssd1306_display_data = { 0x40 };  // Co = 0, D/C = 1

#define SSD1306_TWINT_COMMANDS              0x01
#define SSD1306_TWINT_COMMANDS_AFTER_INIT   0x02
#define SSD1306_TWINT_DATA                  0x04
#define SSD1306_TWINT_DATA_AFTER_FIRST      0x08
static uint8_t ssd1306_twint;

void ssd1306_init_display() {
    ssd1306_send_pos = 0;
    ssd1306_twint = SSD1306_TWINT_COMMANDS_AFTER_INIT | SSD1306_TWINT_DATA;

    ssd1306_clear_screen();
    // TWI frequency = F_CPU / (2 * twbrValue + 16)
    // 16MHz: 12 - 400000, 10 - 444444, 2 - 8000000
    // 8MHz: 12 - 200000, 10 - 222222, 3 -  2 - 4000000, 1 - 444444
    // twi_init(2);
    twi_init();

    //hardwareReset();            // initialize display controller
    ssd1306_start_twi_cmd_frame();

    static const uint8_t PROGMEM init1[] = {
            SSD1306_DISPLAYOFF,                // 0xAE
            SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // 0xD5, b0-3 +1 = clock div 1, b4-7 osc freq,
            SET_MULTIPLEX_RATIO,               // 0xA8, next byte bits 0-5, ratio = value+1
    };
    ssd1306_twi_pgm_byte_list(init1, sizeof(init1));
    ssd1306_twi_byte((uint8_t) (DISPLAY_YSIZE - 1));          // send ratio

    static const uint8_t PROGMEM init2[] = {
            SSD1306_SETDISPLAYOFFSET,             // 0xD3
            0x0,                                  // no offset
            SSD1306_SETSTARTLINE | 0x0,           // 0x40 | line start bits 0-5, line #0
            SSD1306_CHARGEPUMP                    // 0x8D
    };
    ssd1306_twi_pgm_byte_list(init2, sizeof(init2));
    ssd1306_twi_byte(!(SSD1306_CONFIG_FLAGS & SSD1306_SWITCHCAPVCC) ? SSD1306_CHARGEPUMP_DISABLE : SSD1306_CHARGEPUMP_ENABLE);

    static const uint8_t PROGMEM init3[] = {
            SSD1306_MEMORYMODE,                   // 0x20
            0x00,                                 // 0x0, horizontal
            SSD1306_SEGREMAP_REVERSED,            // 0xA1
            SSD1306_COMSCANDEC                    // 0xC8, reversed COM scan reversed
    };
    ssd1306_twi_pgm_byte_list(init3, sizeof(init3));

#if DISPLAY_XSIZE == 128
#if DISPLAY_YSIZE == 32

    static const uint8_t PROGMEM init4a[] = {
            SSD1306_SETCOMPINS,                 // 0xDA, b4 - 0 normal seq pin config, 1 - alternative com pin config, b5 - 0 disable com left/right remap, 1 - enable remap
            0x02,                               // sequential, left/right remap disabled
            SSD1306_SETCONTRAST,                // 0x81
            0x30
    };
    ssd1306_twi_pgm_byte_list(init4a, sizeof(init4a));

#elif DISPLAY_YSIZE == 64

    static const uint8_t PROGMEM init4b[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x12,                               // alternative com pins, left/right disabled
            SSD1306_SETCONTRAST                 // 0x81
    };
    ssd1306_twiByteList(init4b, sizeof(init4b));
    ssd1306_twiByte((configFlags & SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);

#else
#error "Unsupported display size"
#endif
#elif DISPLAY_XSIZE == 96 && DISPLAY_YSIZE == 16

    static const uint8_t PROGMEM init4c[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x2,                                // sequential, left/right remap disabled, Ada 0x12
            SSD1306_SETCONTRAST                 // 0x81
    };
    ssd1306_twiByteList(init4c, sizeof(init4c));
    ssd1306_twiByteList((configFlags & SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);

#else
#error "Unsupported display size"
#endif

    ssd1306_twi_byte(SSD1306_SETPRECHARGE);         // 0xd9
    ssd1306_twi_byte(!(SSD1306_CONFIG_FLAGS & SSD1306_SWITCHCAPVCC) ? 0x22 : 0xF1); // 0x22 on reset
    static const uint8_t PROGMEM init5[] = {
            SSD1306_SETVCOMDETECT,               // 0xDB
            0x40,                                // reset default 0x40, 0x20 => 0.65 Vcc
            SSD1306_DISPLAYALLON_RESUME,         // 0xA4
            SSD1306_NORMALDISPLAY,               // 0xA6
            SSD1306_DEACTIVATE_SCROLL,
//            SSD1306_DISPLAYALLON,                // 0xA5 all on for test
            SSD1306_DISPLAYON // Main screen turn on
    };
    ssd1306_twi_pgm_byte_list(init5, sizeof(init5));
    ssd1306_end_twi_frame();

    if (ssd1306_twint & SSD1306_TWINT_COMMANDS_AFTER_INIT) {
        ssd1306_twint |= SSD1306_TWINT_COMMANDS;
    }
}

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
uint32_t ssd1306_send_time = 0;
uint16_t ssd1306_send_bytes = 0;
uint8_t ssd1306_send_errors;
#endif

void ssd1306_start_twi_cmd_frame() {
    // start accumulating commands
#ifdef SERIAL_DEBUG_GFX_TWI_STATS
    ssd1306_send_time = 0;
    ssd1306_send_bytes = 0;
    ssd1306_send_errors = 0;
#endif

    ssd1306_send_pos = 0;
    ssd1306_twi_byte((uint8_t) 0x00); // Co = 0, D/C = 0
}

void ssd1306_twi_byte(uint8_t byte) {
    if (ssd1306_send_pos >= TWI_BUFFER_LENGTH) {
        // buffer is full, send it and wait for completion
        uint8_t result = 0;
#ifdef SERIAL_DEBUG_GFX_TWI_STATS
        uint32_t start = micros();
#endif

        if (ssd1306_twint & SSD1306_TWINT_COMMANDS) {
            twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), ssd1306_send_buffer, ssd1306_send_pos);
        } else {
            result = twi_writeToSlave(DISPLAY_ADDRESS, ssd1306_send_buffer, ssd1306_send_pos);
#ifdef SERIAL_DEBUG_GFX_TWI_STATS
            if (result) ssd1306_send_errors++;
#endif
        }

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
        ssd1306_send_time += micros() - start;
        ssd1306_send_bytes += ssd1306_send_pos;
#endif
        ssd1306_send_pos = 1; // keep the type command/data
    }
    ssd1306_send_buffer[ssd1306_send_pos++] = byte;
}

void ssd1306_end_twi_frame() {
    // send the accumulated buffer
    uint8_t result = 0;
#ifdef SERIAL_DEBUG_GFX_TWI_STATS
    uint32_t start = micros();
#endif

    if (ssd1306_twint & SSD1306_TWINT_COMMANDS) {
        twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), ssd1306_send_buffer, ssd1306_send_pos);
    } else {
        result = twi_writeToSlave(DISPLAY_ADDRESS, ssd1306_send_buffer, ssd1306_send_pos);
#ifdef SERIAL_DEBUG_GFX_TWI_STATS
        if (result) ssd1306_send_errors++;
#endif
    }

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
    ssd1306_send_time += micros() - start;
    ssd1306_send_bytes += ssd1306_send_pos;
    printf("%8ld: TWI %d bytes in %ld usec %d\n", start / 1000L, ssd1306_send_bytes, ssd1306_send_time, ssd1306_send_errors);
#endif
}

void ssd1306_display() {
    // get any output or they won't display
    ssd1306_flush_wrap_chars();

    ssd1306_start_twi_cmd_frame();
    static const uint8_t dlist1[] PROGMEM = {
            SSD1306_PAGEADDR,       // 0x22
            0,                      // Page start address, bits 0-2
            0x07,                   // Page end address, bits 0-2 (not really, but works here)
            SSD1306_COLUMNADDR, 0 }; // 0x21 - Column start address
    ssd1306_twi_pgm_byte_list(dlist1, sizeof(dlist1));
    ssd1306_twi_byte(DISPLAY_XSIZE - 1); // Column end address
    ssd1306_end_twi_frame();

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
    uint32_t start = micros();
#endif

    twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), &ssd1306_display_data, sizeof(ssd1306_display_data));
}

// actually send command and wait for completion
void ssd1306_send_cmd(uint8_t cmd) {
    ssd1306_start_twi_cmd_frame();
    ssd1306_twi_byte(cmd);
    ssd1306_end_twi_frame();
}

void ssd1306_twi_pgm_byte_list(const uint8_t *bytes, uint16_t count) {
    while (count-- > 0) {
        ssd1306_twi_byte(pgm_read_byte(bytes++));
    }
}
// Display functions
void ssd1306_set_inverted() {
    ssd1306_send_cmd(SSD1306_INVERTDISPLAY);
}

void ssd1306_clear_inverted() {
    ssd1306_send_cmd(SSD1306_NORMALDISPLAY);
}

void ssd1306_display_off() {
    ssd1306_send_cmd(SSD1306_DISPLAYOFF);
}

void ssd1306_display_on() {
    ssd1306_send_cmd(SSD1306_DISPLAYON);
}

void ssd1306_set_contrast(uint8_t contrast) {
    ssd1306_start_twi_cmd_frame();
    ssd1306_twi_byte(SSD1306_SETCONTRAST);
    ssd1306_twi_byte(contrast);
    ssd1306_end_twi_frame();
}
#else

void ssd1306_init_display() {
}

// Display functions
void ssd1306_set_inverted() {
}

void ssd1306_clear_inverted() {
}

void ssd1306_display_off() {
}

void ssd1306_display_on() {
}

void ssd1306_set_contrast(uint8_t contrast) {
}

#endif

