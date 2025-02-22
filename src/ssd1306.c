#include "Arduino.h"
#include "ssd1306_gfx.h"
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

#ifndef CONSOLE_DEBUG
uint8_t gfx_send_buffer[TWI_BUFFER_LENGTH];
uint8_t gfx_send_pos;
// CAVEAT: initializing a single byte of a static structure allocates PROGMEM
//    for the full structure to copy to RAM during initialization. In this case
//    it means 512 bytes of FLASH are gone in a flash. Better to do it in the init
//    function.
gfx_display_buffer_t gfx_display_data;// = {0x40}; // Co = 0, D/C = 1

#ifdef INCLUDE_TWI_INT

#define TWI_FLUSH() twiint_flush()
#define TWI_INIT() twiint_init()

#else

#define TWI_FLUSH() ((void)0)
#define TWI_INIT() twi_init()

#endif // INCLUDE_TWI_INT

void gfx_init_display(uint8_t contrast) {
    gfx_send_pos = 0;
    // HACK: get around the byte being const for the rest of its life
    *((uint8_t *) &gfx_display_data.twi_data) = 0x40;  // Co = 0, D/C = 1

    gfx_clear_display();
    // TWI frequency = F_CPU / (2 * twbrValue + 16)
    // 16MHz: 12 - 400000, 10 - 444444, 2 - 8000000
    // 8MHz: 12 - 200000, 10 - 222222, 3 -  2 - 4000000, 1 - 444444
    // twi_init(2);
    TWI_INIT();

    //hardwareReset();            // initialize display controller
    gfx_start_twi_cmd_frame();

    static const uint8_t PROGMEM init1[] = {
            SSD1306_DISPLAYOFF,                // 0xAE
            SSD1306_SETDISPLAYCLOCKDIV, 0x80,  // 0xD5, b0-3 +1 = clock div 1, b4-7 osc freq,
            SET_MULTIPLEX_RATIO,               // 0xA8, next byte bits 0-5, ratio = value+1
    };
    gfx_twi_pgm_byte_list(init1, sizeof(init1));
    gfx_twi_byte((uint8_t) (DISPLAY_YSIZE - 1));          // send ratio

    static const uint8_t PROGMEM init2[] = {
            SSD1306_SETDISPLAYOFFSET,             // 0xD3
            0x0,                                  // no offset
            SSD1306_SETSTARTLINE | 0x0,           // 0x40 | line start bits 0-5, line #0
            SSD1306_CHARGEPUMP                    // 0x8D
    };
    gfx_twi_pgm_byte_list(init2, sizeof(init2));
    gfx_twi_byte(!(SSD1306_CONFIG_FLAGS & SSD1306_SWITCHCAPVCC) ? SSD1306_CHARGEPUMP_DISABLE : SSD1306_CHARGEPUMP_ENABLE);

    static const uint8_t PROGMEM init3[] = {
            SSD1306_MEMORYMODE,                   // 0x20
            0x00,                                 // 0x0, horizontal
            SSD1306_SEGREMAP_REVERSED,            // 0xA1
            SSD1306_COMSCANDEC                    // 0xC8, reversed COM scan reversed
    };
    gfx_twi_pgm_byte_list(init3, sizeof(init3));

#if DISPLAY_XSIZE == 128
#if DISPLAY_YSIZE == 32

    static const uint8_t PROGMEM init4a[] = {
            SSD1306_SETCOMPINS,                 // 0xDA, b4 - 0 normal seq id config, 1 - alternative com id config, b5 - 0 disable com left/right remap, 1 - enable remap
            0x02,                               // sequential, left/right remap disabled
            SSD1306_SETCONTRAST,                // 0x81
    };
    gfx_twi_pgm_byte_list(init4a, sizeof(init4a));
    gfx_twi_byte(contrast);

#elif DISPLAY_YSIZE == 64

    static const uint8_t PROGMEM init4b[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x12,                               // alternative com pins, left/right disabled
            SSD1306_SETCONTRAST                 // 0x81
    };
    gfx_twiByteList(init4b, sizeof(init4b));
    gfx_twiByte((configFlags & SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);

#else
#error "Unsupported display size"
#endif
#elif DISPLAY_XSIZE == 96 && DISPLAY_YSIZE == 16

    static const uint8_t PROGMEM init4c[] = {
            SSD1306_SETCOMPINS,                 // 0xDA
            0x2,                                // sequential, left/right remap disabled, Ada 0x12
            SSD1306_SETCONTRAST                 // 0x81
    };
    gfx_twiByteList(init4c, sizeof(init4c));
    gfx_twiByteList((configFlags & SSD1306_EXTERNALVCC) ? 0x10 : 0xAF);

#else
#error "Unsupported display size"
#endif

    gfx_twi_byte(SSD1306_SETPRECHARGE);         // 0xd9
    gfx_twi_byte(!(SSD1306_CONFIG_FLAGS & SSD1306_SWITCHCAPVCC) ? 0x22 : 0xF1); // 0x22 on restart
    static const uint8_t PROGMEM init5[] = {
            SSD1306_SETVCOMDETECT,               // 0xDB
            0x40,                                // restart default 0x40, 0x20 => 0.65 Vcc
            SSD1306_DISPLAYALLON_RESUME,         // 0xA4
            SSD1306_NORMALDISPLAY,               // 0xA6
            SSD1306_DEACTIVATE_SCROLL,
//            SSD1306_DISPLAYALLON,                // 0xA5 all on for test
            SSD1306_DISPLAYON // Main screen turn on
    };
    gfx_twi_pgm_byte_list(init5, sizeof(init5));
    gfx_end_twi_frame();
}

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
uint32_t gfx_send_time = 0;
uint16_t gfx_send_bytes = 0;
uint8_t gfx_send_errors;

#define RESET_SERIAL_DEBUG_GFX_TWI_STATS() \
    gfx_send_time = 0; \
    gfx_send_bytes = 0; \
    gfx_send_errors = 0

#define START_SERIAL_DEBUG_GFX_TWI_STATS() \
        uint32_t start = micros()

#define RESTART_SERIAL_DEBUG_GFX_TWI_STATS() \
        start = micros()

#define END_SERIAL_DEBUG_GFX_TWI_STATS(bytes_sent) \
        gfx_send_time += micros() - start; \
        gfx_send_bytes += (bytes_sent)

#define PRINTF_SERIAL_DEBUG_GFX_TWI_STATS(...) printf_P(__VA_ARGS__)

#else
#define RESET_SERIAL_DEBUG_GFX_TWI_STATS()                  ((void)0)
#define START_SERIAL_DEBUG_GFX_TWI_STATS()                  ((void)0)
#define END_SERIAL_DEBUG_GFX_TWI_STATS(bytes_sent)          ((void)0)
#define PRINTF_SERIAL_DEBUG_GFX_TWI_STATS(...)              ((void)0)
#endif // SERIAL_DEBUG_GFX_TWI_STATS

void gfx_start_twi_cmd_frame() {
    // start accumulating commands
    RESET_SERIAL_DEBUG_GFX_TWI_STATS();

    // not flushing the last transmission was causing initialization commands not to work with int, overwriting buffer while it is being sent
    START_SERIAL_DEBUG_GFX_TWI_STATS();
    TWI_FLUSH();
    END_SERIAL_DEBUG_GFX_TWI_STATS(0);

    gfx_send_pos = 0;
    gfx_twi_byte((uint8_t) 0x00); // Co = 0, D/C = 0
}

void gfx_twi_byte(uint8_t byte) {
    if (gfx_send_pos >= TWI_BUFFER_LENGTH) {
        // buffer is full, send it and wait for completion
        START_SERIAL_DEBUG_GFX_TWI_STATS();

#ifdef INCLUDE_TWI_INT
        twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), gfx_send_buffer, gfx_send_pos);
        // have to wait for it to send, or will overwrite the buffer before it is sent when the current byte is buffered
        twiint_flush();
#else
        twi_writeToSlave(DISPLAY_ADDRESS, gfx_send_buffer, gfx_send_pos);
#endif // INCLUDE_TWI_INT

        END_SERIAL_DEBUG_GFX_TWI_STATS(gfx_send_pos);
        gfx_send_pos = 1; // keep the type command/data
    }
    gfx_send_buffer[gfx_send_pos++] = byte;
}

void gfx_end_twi_frame() {
    // send the accumulated buffer
    START_SERIAL_DEBUG_GFX_TWI_STATS();

#ifdef INCLUDE_TWI_INT
    twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), gfx_send_buffer, gfx_send_pos);
#else
    twi_writeToSlave(DISPLAY_ADDRESS, gfx_send_buffer, gfx_send_pos);
#endif // INCLUDE_TWI_INT

    END_SERIAL_DEBUG_GFX_TWI_STATS(gfx_send_pos);

    PRINTF_SERIAL_DEBUG_GFX_TWI_STATS(PSTR("%8ld: TWI %d command bytes in %ld usec %d\n"), start / 1000L, gfx_send_bytes, gfx_send_time, gfx_send_errors);
}

void gfx_starting_next_update() {
    // to prevent buffer being modified while it is still sending
    START_SERIAL_DEBUG_GFX_TWI_STATS();
    TWI_FLUSH();
    END_SERIAL_DEBUG_GFX_TWI_STATS(0);
    PRINTF_SERIAL_DEBUG_GFX_TWI_STATS(PSTR("%8ld: TWI %d next update in %ld usec %d\n"), start / 1000L, gfx_send_bytes, gfx_send_time, gfx_send_errors);
}

void gfx_display() {
    // get any output or they won't display
    gfx_flush_wrap_chars();

    gfx_start_twi_cmd_frame();
#ifdef GFX_PAGED_UPDATES
    gfx_twi_byte(SSD1306_PAGEADDR);
    gfx_twi_byte(gfx_update_page_y0 >> 3);
    gfx_twi_byte(gfx_update_page_y1 >> 3);
    gfx_twi_byte(SSD1306_COLUMNADDR);
    gfx_twi_byte(0);
    gfx_twi_byte(DISPLAY_XSIZE - 1);
    gfx_end_twi_frame();
#else
    static const uint8_t dlist1[] PROGMEM = {
            SSD1306_PAGEADDR,       // 0x22
            0,                      // Page start address, bits 0-2
            0x07,                   // Page end address, bits 0-2
            SSD1306_COLUMNADDR, 0, DISPLAY_XSIZE - 1 }; // 0x21 - Column start address, end address
    gfx_twi_pgm_byte_list(dlist1, sizeof(dlist1));
    gfx_end_twi_frame();
#endif // GFX_PAGED_UPDATES

    RESET_SERIAL_DEBUG_GFX_TWI_STATS();
    START_SERIAL_DEBUG_GFX_TWI_STATS();
#ifdef INCLUDE_TWI_INT
    twiint_start(TWI_ADDRESS_W(DISPLAY_ADDRESS), &gfx_display_data, sizeof(gfx_display_data));
#else
    twi_writeToSlave(DISPLAY_ADDRESS, &gfx_display_data, sizeof(gfx_display_data));
#endif // INCLUDE_TWI_INT
    END_SERIAL_DEBUG_GFX_TWI_STATS(sizeof(gfx_display_data));
    PRINTF_SERIAL_DEBUG_GFX_TWI_STATS(PSTR("%8ld: TWI pg %d-%d, %d data bytes in %ld usec %d\n")
                                      , start / 1000L
                                      , (uint16_t) (Y_COORD_DISPLAY_Y0 >> 3)
                                      , (uint16_t) (Y_COORD_DISPLAY_Y1 >> 3)
                                      , gfx_send_bytes
                                      , gfx_send_time
                                      , gfx_send_errors);
}

// actually send command and wait for completion
void gfx_send_cmd(uint8_t cmd) {
    gfx_start_twi_cmd_frame();
    gfx_twi_byte(cmd);
    gfx_end_twi_frame();
}

void gfx_twi_pgm_byte_list(const uint8_t *bytes, uint16_t count) {
    while (count-- > 0) {
        gfx_twi_byte(pgm_read_byte(bytes++));
    }
}

// Display functions
void gfx_set_inverted() {
    gfx_send_cmd(SSD1306_INVERTDISPLAY);
}

void gfx_clear_inverted() {
    gfx_send_cmd(SSD1306_NORMALDISPLAY);
}

void gfx_display_off() {
    gfx_send_cmd(SSD1306_DISPLAYOFF);
    TWI_FLUSH(); // wait for it to complete
}

void gfx_display_on() {
    gfx_send_cmd(SSD1306_DISPLAYON);
}

void gfx_set_contrast(uint8_t contrast) {
    gfx_start_twi_cmd_frame();
    gfx_twi_byte(SSD1306_SETCONTRAST);
    gfx_twi_byte(contrast);
    gfx_end_twi_frame();
}

#else // CONSOLE_DEBUG

void gfx_starting_next_update() {
}

void gfx_init_display(uint8_t contrast) {
}

// Display functions
void gfx_set_inverted() {
}

void gfx_clear_inverted() {
}

void gfx_display_off() {
}

void gfx_display_on() {
}

void gfx_set_contrast(uint8_t contrast) {
}

#endif // CONSOLE_DEBUG

