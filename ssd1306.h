#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "Arduino.h"
#include <stdlib.h>

#define RGB565(r, g, b) ((color_t)((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) & 0xF8) >> 3))
#define RGB666(r, g, b) ((int32_t)(static_cast<uint8_t>(r) << 16 | static_cast<uint8_t>(g) << 8 | static_cast<uint8_t>(b)))

#define RGB565_TO_RGB888(c) (((int32_t)((c) & 0xF800) << 8) | (((int32_t)((c) & 0x07E0) << 5)) | (((int32_t)((c) & 0x001F) << 1)))

#define RGB565_RED(c) (static_cast<uint8_t>((static_cast<uint16_t>(c) >> 8) & static_cast<uint8_t>(0xF8)))
#define RGB565_GREEN(c) (static_cast<uint8_t>((static_cast<uint16_t>(c) >> 3) & static_cast<uint8_t>(0xFC)))
#define RGB565_BLUE(c) (static_cast<uint8_t>((static_cast<uint16_t>(c) << 3) & static_cast<uint8_t>(0xF8)))

#ifndef clearBit
#define clearBit(x, y) (x &= ~_BV(y))     // equivalent to cbi(x,y)
#endif clearBit

#ifndef setBit
#define setBit(x, y) (x |= _BV(y))        // equivalent to sbi(x,y)
#endif setBit

// DC 8
#define SSD1306_DC_PORT  PORTD
#define SSD1306_DC_BIT   5
#define SSD1306_DC       SSD1306_DC_PORT,SSD1306_DC_BIT

// DC 9
#define SSD1306_RST_PORT PORTB
#define SSD1306_RST_BIT  1
#define SSD1306_RST       SSD1306_RST_PORT,SSD1306_RST_BIT

// DC 10
#define SSD1306_CS_PORT  PORTB
#define SSD1306_CS_BIT   2
#define SSD1306_CS       SSD1306_CS_PORT,SSD1306_CS_BIT

// DC 11
#define SSD1306_MOSI_PORT PORTB
#define SSD1306_MOSI_BIT  3
#define SSD1306_MOSI       SSD1306_MOSI_PORT,SSD1306_MOSI_BIT

// so we can turn off SPI and use these manually to try and read pixels
// DC 12
#define SSD1306_MISO_PORT PORTB
#define SSD1306_MISO_BIT  4
#define SSD1306_MISO       SSD1306_MISO_PORT,SSD1306_MISO_BIT

// DC 13
#define SSD1306_SCK_PORT  PORTB
#define SSD1306_SCK_BIT   5
#define SSD1306_SCK       SSD1306_SCK_PORT,SSD1306_SCK_BIT

// ---------------------------------------------------------------------------
// SSD1306 ROUTINES
#define SSD1306_SETCONTRAST    0x81        // set contrast (2 bytes)
#define SSD1306_DISPLAYALLON   0xA5                   // entire display on
#define SSD1306_DISPLAYALLON_RESUME    0xA4        // display ram buffer
#define SSD1306_DISPLAYOFF     0xAE        // display off
#define SSD1306_DISPLAYON      0xAF        // display on
#define SSD1306_NORMALDISPLAY      0xA6        // inv off
#define SSD1306_INVERTDISPLAY       0xA7        // inv on

#define HSCR_RIGHT  0x26        // horizontal scroll right (7 bytes)
#define HSCR_LEFT   0x27        // horizontal scroll left  (7 bytes)
                                // [1] - 0
                                // [2] - start page# 0..7
                                // [3] - interval frames 0..7: 0 - 5f, 1 - 64f, 2 - 128f, 3 - 256f, 4 - 3f, 5 - 4f, 6 - 25f, 7 - 2f
                                // [4] - end page # 0..7 >= start page
                                // [5] - 0
                                // [6] - 0xFF

#define VSCR_RIGHT  0x29        // vertical & horizontal scroll right (7 bytes)
#define VSCR_LEFT   0x2A        // vertical & horizontal scroll left  (7 bytes)
                                // [1] - 0
                                // [2] - start page# 0..7
                                // [3] - interval frames 0..7: 0 - 5f, 1 - 64f, 2 - 128f, 3 - 256f, 4 - 3f, 5 - 4f, 6 - 25f, 7 - 2f
                                // [4] - end page # 0..7 >= start page
                                // [5] - vertical scrolling offset 0..63

#define SSD1306_DEACTIVATE_SCROLL   0x2E        // deactivate scroll
#define ACT_SCR     0x2E        // activate scroll

#define VSCR_AREA   0xA3        // vertical scroll area (3 bytes)
                                // [1] - no of rows fixed 0..63
                                // [2] - no of rows scrolling 0..64

#define SSD1306_MEMORYMODE   0x20  // set addressing mode (2 bytes)
                                // [1] - 0 - horizontal, 1 - vertical, 2 - page addressing, 3 - invalid

#define PAGE_COLS   0x21        // set col start/end for horiz/vertical addressing mode (3 bytes)
                                // [1] - column start (0..127)
                                // [1] - column end (0..127)

#define PAGE_ADDR   0x22        // set page address (3 bytes)  (horizontal or vertical addressing mode)
                                // [1] - page start
                                // [2] - page end

#define LOCOL_START 0x00        // set low column start nibble (or with low nibble 0..f) for page addressing
#define HICOL_START 0x10        // set hi column start nibble (or with high nibble 0..f) for page addressing

#define PAGE_START  0xB0        // set page start for page addressing mode (or with page # 0..7), B0 page 0, B1 page 1, B2 page, ... B7 page 7

#define SSD1306_COMSCANINC     0xC0        // rows scanned normal
#define SSD1306_COMSCANDEC     0xC8        // rows scanned reversed (vertical flip)

#define SSD1306_SEGREMAP_NORMAL      0xA0        // columns scanned normal
#define SSD1306_SEGREMAP_REVERSED    0xA1        // columns scanned reversed (horizontal flip)

#define DISP_OFFS   0xD3        // vertical shift (2 bytes)
                                // [1] - veritcal offset 0..63

#define DISP_CLK_DIV 0xD5       // display clock divide, 2 bytes
                                // [1] - bits[0..3]+1 is divide ratio, default (0), bits[4..7] osc frequency default(8)

#define SET_MULTIPLEX_RATIO  0xA8         // set multiplex ratio 2 bytes
                                // [1] - 15..63,+1 is ratio, 0..14 invalid

#define SSD1306_SETDISPLAYOFFSET 0xD3  // display offset 2 bytes
                                // [1] - 0..63 display offset, default 0

#define SSD1306_SETSTARTLINE   0x40 // 1 byte, bits[0..5] display offset
#define SSD1306_CHARGEPUMP     0x8D // charge pump 2 bytes
                                    // [1] - 0x10 disable, 0x14 enable
#define SSD1306_CHARGEPUMP_ENABLE     0x14
#define SSD1306_CHARGEPUMP_DISABLE    0x10

#define SSD1306_SETCOMPINS 0xDA    // set com pins hardware config 2 bytes
//A[1]=1b,
//A[4]=0b, Sequential COM pin configuration A[4]=1b(RESET), Alternative COM pin configuration
//A[5]=0b(RESET), Disable COM Left/Right remap
//A[5]=1b, Enable COM Left/Right remap

#define SSD1306_SETPRECHARGE  0xD9 // set precharge 2 bytes
//A[3:0] : Phase 1 period of up to 15 DCLK clocks 0 is invalid entry
//(RESET=2h)
//A[7:4] : Phase 2 period of up to 15 DCLK clocks 0 is invalid entry
//(RESET=2h )

#define SSD1306_SETVCOMDETECT 0xDB  // Vcomh deselect level (2bytes)
//| A[6:4] | Hex Byte | V COMH deselect level |
//|--------|-----|-----------------------|
//| 000b   | 00h | ~ 0.65 x VCC          |
//| 010b   | 40h | ~ 0.77 x VCC (RESET)  |
//| 011b   | 60h | ~ 0.83 x VCC          |


// TFT display constants, defaults
#define SSD1306_XSIZE   128
#define SSD1306_YSIZE   64
#define SSD1306_XOFFSET 0
#define SSD1306_YOFFSET 0

#define SSD1306_CHAR_X_PIXELS 5
#define SSD1306_CHAR_Y_PIXELS 7
#define SSD1306_CHAR_WIDTH (SSD1306_CHAR_X_PIXELS+1)
#define SSD1306_CHAR_HEIGHT (SSD1306_CHAR_Y_PIXELS+1)

#define _RGB(r, g, b) ((r) != 0 || (g) != 0 || (b) != 0)
typedef int8_t color_t;
typedef uint8_t alpha_t;

// Color constants
#define RGB_BLACK   _RGB(0,0,0)
#define RGB_WHITE   _RGB(255,255,255)

#define SSD1306_ROT_0     0
#define SSD1306_ROT_90    1
#define SSD1306_ROT_180   2
#define SSD1306_ROT_270   3

#define SSD1306_INVERTED  0x80 // display color is inverted when inversion is off
#define SSD1306_EXTERNALVCC  0x40 // external vcc, no charge pump

#define SSD1306_SIZE(displayType)      ((displayType) & 0x0f)

// OLED 0.96 128 x 64
#define SSD1306_SIZE_096  0x00
// OLED 0.91  128 x 32
#define SSD1306_SIZE_091  0x01

// OLED 0.96 128 x 64
#define SSD1306_TYPE_OLED_096  (SSD1306_SIZE_096)
// OLED 0.91  128 x 32
#define SSD1306_TYPE_OLED_091  (SSD1306_SIZE_091)

// 128x64 TFT display constants
#define SSD1306_096_XSIZE   SSD1306_XSIZE
#define SSD1306_096_YSIZE   SSD1306_YSIZE
#define SSD1306_096_XOFFSET SSD1306_XOFFSET
#define SSD1306_096_YOFFSET SSD1306_YOFFSET

// TFT096 values
#define SSD1306_091_XSIZE   128
#define SSD1306_091_YSIZE   32
#define SSD1306_091_XOFFSET 0
#define SSD1306_091_YOFFSET 0

class Ssd1306
{
public:
    uint8_t flags;

    uint8_t maxX;
    uint8_t maxY;

    color_t foreground;
    color_t background;

    uint8_t rotation;
    uint8_t sizeX;
    uint8_t sizeY;

    uint8_t colOffset;      // char column pixel offset
    uint8_t rowOffset;      // char row pixel offset
    int16_t col;            // current column
    int16_t row;            // current row
    uint8_t maxCols;
    uint8_t maxRows;

    uint8_t pageStart;      // current refresh yStart of page
    uint8_t *buffer;        // page buffer for paged drawing

    // with type conversions
    explicit Ssd1306(uint8_t *pageBuffer, uint8_t typeFlags = 0);
    void setOrientation(uint8_t rot);
    void initDisplay(uint8_t rot);
    void setInverted();
    void clearInverted();

    // raw, no type conversions
    void initDisplay(uint8_t rot, uint8_t xSize, uint8_t ySize, uint8_t xOffset, uint8_t yOffset, int8_t inversion);
    void setOrientation(uint8_t rot, uint8_t xSize, uint8_t ySize, uint8_t xOffset, uint8_t yOffset);
    void actualCoords(int &x, int &y);
    bool isInPage(int x0, int x1, int y0, int y1);
    bool isInPageTrimX(int &x0, int &x1, int y0, int y1);
    bool isInPageTrimY(int x0, int x1, int &y0, int &y1);
    bool isInPageTrimXY(int &x0, int &x1, int &y0, int &y1);

    void startPage(uint8_t page);
    void updatePage();

    void startUpdate();
    bool nextPage();

    bool isBGR();
    color_t rgb(uint8_t r, uint8_t g, uint8_t b);

    void hardwareReset();

    void startCmd(uint8_t cmd);
    void endCmd();

    void sendByte(uint8_t byte);
    void sendBytes(uint8_t byte, uint16_t count);
    void sendBytes(uint8_t *bytes, uint16_t count);
    void sendCmd(uint8_t cmd);
    void sendCmdList(const uint8_t *p, uint8_t count);
//    void sendWord(uint16_t word);

//    void sendColor(color_t color);
//    void sendColors(color_t color, uint16_t count);
//    void send565(color_t color, uint16_t count);
    void sendSetAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

    void clearScreen();

    void setPixel(int x, int y, color_t color);
//    void sendPixels(color_t color, uint16_t count);

    void hLine(int x0, int x1, int y, color_t color);
    void vLine(int x, int y0, int y1, color_t color);
    void line(int x0, int y0, int x1, int y1, color_t color);
    void rect(int x0, int y0, int x1, int y1, color_t color);
    void fillRect(int x0, int y0, int x1, int y1, color_t color);

    void circleQuadrant(int cx, int cy, int radius, uint8_t quads, color_t color);
    void circle(int cx, int cy, int radius, color_t color);
    void roundRect(int x0, int y0, int x1, int y1, int r, color_t color);
    void fillCircle(int cx, int cy, int radius, color_t color);
    void ellipse(int cx, int cy, int width, int height, color_t color);
    void fillEllipse(int cx, int cy, int width, int height, color_t color);

    void charOffset(uint8_t x, uint8_t y);
    void gotoCharXY(int col, int row);
    void advanceCursor();
    void putCh(char ch, int x, int y);
    void write(char ch);
    void write(char ch, int count);
    void write(const char *text);
    void write(long i);
    void write(unsigned long i);
    void writeHex(long i);
    void writeHex(int i);
    void writeDigit(int dig);
    void write(int i);

    void openSPI();
    void closeSPI();

    static int intSqrt(long val);
};

// helper functions
// calculate integer value of square root

#endif // _SSD1306_H_
