//
// Created by Vladimir Schneider on 2023-03-07.
//

#ifndef SSD1306_SSD1306_CMD_H
#define SSD1306_SSD1306_CMD_H
// SSD1306 ROUTINES
#define SSD1306_SETCONTRAST                             0x81        // set contrast (2 bytes)
#define SSD1306_DISPLAYALLON                            0xA5        // entire display on, ignore ram buffer
#define SSD1306_DISPLAYALLON_RESUME                     0xA4  // display ram buffer
#define SSD1306_DISPLAYOFF                              0xAE        // display off
#define SSD1306_DISPLAYON                               0xAF        // display on
#define SSD1306_NORMALDISPLAY                           0xA6        // inv off
#define SSD1306_INVERTDISPLAY                           0xA7        // inv on

#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26        // horizontal scroll right (7 bytes)
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27        // horizontal scroll left  (7 bytes)
// [1] - 0
// [2] - start page# 0..7
// [3] - interval frames 0..7: 0 - 5f, 1 - 64f, 2 - 128f, 3 - 256f, 4 - 3f, 5 - 4f, 6 - 25f, 7 - 2f
// [4] - end page # 0..7 >= start page
// [5] - 0
// [6] - 0xFF

#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29        // vertical & horizontal scroll right (7 bytes)
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A        // vertical & horizontal scroll left  (7 bytes)
// [1] - 0
// [2] - start page# 0..7
// [3] - interval frames 0..7: 0 - 5f, 1 - 64f, 2 - 128f, 3 - 256f, 4 - 3f, 5 - 4f, 6 - 25f, 7 - 2f
// [4] - end page # 0..7 >= start page
// [5] - vertical scrolling offset 0..63

#define SSD1306_DEACTIVATE_SCROLL                       0x2E        // deactivate scroll
#define SSD1306_ACTIVATE_SCROLL                         0x2F        // activate scroll

#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3        // vertical scroll area (3 bytes)
// [1] - no of rows fixed 0..63
// [2] - no of rows scrolling 0..64

#define SSD1306_MEMORYMODE                              0x20  // set addressing mode (2 bytes)
// [1] - 0 - horizontal, 1 - vertical, 2 - page addressing, 3 - invalid

#define SSD1306_COLUMNADDR                              0x21        // set col start/end for horiz/vertical addressing mode (3 bytes)
// [1] - column start (0..127)
// [1] - column end (0..127)

#define SSD1306_PAGEADDR                                0x22        // set page address (3 bytes)  (horizontal or vertical addressing mode)
// [1] - page start
// [2] - page end

#define SSD1306_SETLOWCOLUMN                            0x00        // set low column start nibble (or with low nibble 0..f) for page addressing
#define SSD1306_SETHIGHCOLUMN                           0x10        // set hi column start nibble (or with high nibble 0..f) for page addressing

#define PAGE_START                                      0xB0        // set page start for page addressing mode (or with page # 0..7), B0 page 0, B1 page 1, B2 page, ... B7 page 7

#define SSD1306_COMSCANINC                              0xC0        // rows scanned normal
#define SSD1306_COMSCANDEC                              0xC8        // rows scanned reversed (vertical flip)

#define SSD1306_SEGREMAP_NORMAL                         0xA0        // columns scanned normal
#define SSD1306_SEGREMAP_REVERSED                       0xA1        // columns scanned reversed (horizontal flip)

#define DISP_OFFS                                       0xD3        // vertical shift (2 bytes)
// [1] - vertical offset 0..63

#define SSD1306_SETDISPLAYCLOCKDIV                      0xD5       // display clock divide, 2 bytes
// [1] - bits[0..3]+1 is divide ratio, default (0), bits[4..7] osc frequency default(8)

#define SET_MULTIPLEX_RATIO                             0xA8         // set multiplex ratio 2 bytes
// [1] - 15..63,+1 is ratio, 0..14 invalid

#define SSD1306_SETDISPLAYOFFSET                        0xD3  // display offset 2 bytes
// [1] - 0..63 display offset, default 0

#define SSD1306_SETSTARTLINE                            0x40 // 1 byte, bits[0..5] display offset
#define SSD1306_CHARGEPUMP                              0x8D // charge pump 2 bytes
// [1] - 0x10 disable, 0x14 enable
#define SSD1306_CHARGEPUMP_ENABLE                       0x14
#define SSD1306_CHARGEPUMP_DISABLE                      0x10

#define SSD1306_SETCOMPINS                              0xDA    // set com pins hardware config 2 bytes
//A[1]=1b,
//A[4]=0b, Sequential COM pin configuration A[4]=1b(RESET), Alternative COM pin configuration
//A[5]=0b(RESET), Disable COM Left/Right remap
//A[5]=1b, Enable COM Left/Right remap

#define SSD1306_SETPRECHARGE                            0xD9 // set precharge 2 bytes
//A[3:0] : Phase 1 period of up to 15 DCLK clocks 0 is invalid entry
//(RESET=2h)
//A[7:4] : Phase 2 period of up to 15 DCLK clocks 0 is invalid entry
//(RESET=2h )

#define SSD1306_SETVCOMDETECT                           0xDB  // Vcomh deselect level (2bytes)
//| A[6:4] | Hex Byte | V COMH deselect level |
//|--------|-----|-----------------------|
//| 000b   | 00h | ~ 0.65 x VCC          |
//| 010b   | 40h | ~ 0.77 x VCC (RESET)  |
//| 011b   | 60h | ~ 0.83 x VCC          |
#define SSD1306_NOP                                     0xE3  // do nothing

#define SSD1306_SETBLINKING_FADEOUT                     0x23  // do nothing
//| A[5:4] | Hex Byte | Blinking/Fadeout |
//|--------|----------|------------------|
//| 00b    | 00h      | Disabled (RESET) |
//| 10b    | 40h      | Enable fadeout   |
//| 11b    | 60h      | Enable blinking  |
//
//| A[3:0] | Hex Byte | Time interval for each fade step |
//|--------|----------|----------------------------------|
//| 0000b  | 0h       | 8 Frames                         |
//| 0001b  | 1h       | 16 Frames                        |
//| 0010b  | 2h       | 24 Frames                        |
//| ...                                                |||
//| 1111b  | 0Fh      | 128 Frames                       |


#endif //SSD1306_SSD1306_CMD_H
