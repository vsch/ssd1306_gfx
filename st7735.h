#ifndef _TFT18_H_
#define _TFT18_H_

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
#define ST7735_DC_PORT  PORTD
#define ST7735_DC_BIT   5
#define ST7735_DC       ST7735_DC_PORT,ST7735_DC_BIT

// DC 9
#define ST7735_RST_PORT PORTB
#define ST7735_RST_BIT  1
#define ST7735_RST       ST7735_RST_PORT,ST7735_RST_BIT

// DC 10
#define ST7735_CS_PORT  PORTB
#define ST7735_CS_BIT   2
#define ST7735_CS       ST7735_CS_PORT,ST7735_CS_BIT

// DC 11
#define ST7735_MOSI_PORT PORTB
#define ST7735_MOSI_BIT  3
#define ST7735_MOSI       ST7735_MOSI_PORT,ST7735_MOSI_BIT

// so we can turn off SPI and use these manually to try and read pixels
// DC 12
#define ST7735_MISO_PORT PORTB
#define ST7735_MISO_BIT  4
#define ST7735_MISO       ST7735_MISO_PORT,ST7735_MISO_BIT

// DC 13
#define ST7735_SCK_PORT  PORTB
#define ST7735_SCK_BIT   5
#define ST7735_SCK       ST7735_SCK_PORT,ST7735_SCK_BIT

// ---------------------------------------------------------------------------
// ST7735 ROUTINES
#define SWRESET     0x01        // software reset
#define RDDID       0x04        // read id
#define SLPIN       0x10        // sleep in
#define SLPOUT      0x11        // sleep out
#define INVOFF      0x20        // inv off
#define INVON       0x21        // inv on
#define GAMSET      0x26        // gama set
#define DISPOFF     0x28        // display off
#define DISPON      0x29        // display on
#define CASET       0x2A        // column address set
#define RASET       0x2B        // row address set
#define RAMWR       0x2C        // RAM write
#define RAMRD       0x2E        // RAM read
#define MADCTL      0x36        // axis control
#define IDMOFF      0x38        // idle off
#define IDMON       0x39        // idle on
#define COLMOD      0x3A        // color mode

#define RDDID1      0xDA        // read id1
#define RDDID2      0xDB        // read id2
#define RDDID3      0xDC        // read id3

// panel commands do not work (probably EXTC is open so only display functions available)
#define WRID2       0xD1        // write id2
#define WRID3       0xD2        // write id3

#define RDDCOLMOD   0x0C        // read pixel format

// TFT display constants, defaults
#define ST7735_XSIZE   128
#define ST7735_YSIZE   160
#define ST7735_XOFFSET 0
#define ST7735_YOFFSET 0

#define ST7735_CHAR_X_PIXELS 5
#define ST7735_CHAR_Y_PIXELS 7
#define ST7735_CHAR_WIDTH (ST7735_CHAR_X_PIXELS+1)
#define ST7735_CHAR_HEIGHT (ST7735_CHAR_Y_PIXELS+1)

// mode 5 = 16bit pixels (RGB565)
// mode 6 = 18bit pixels (RGB666)
#define COLMOD_PARAM 5

// 4 LSB's one of selects GC0 - GC3
#define GAMSET_PARAM 1

#if COLMOD_PARAM == 5
#define _RGB(r, g, b) RGB565(r, g, b)
typedef uint16_t color_t;
#else
#define _RGB(r, g, b) RGB666(r, g, b)
typedef uint32_t color_t;
#endif

typedef uint8_t alpha_t;

// Color constants
#define RGB_BLACK   _RGB(0,0,0)
#define RGB_WHITE   _RGB(255,255,255)

#define ST7735_ROT_0     0
#define ST7735_ROT_90    1
#define ST7735_ROT_180   2
#define ST7735_ROT_270   3

#define ST7735_INVERTED  0x80 // display color is inverted when inversion is off
#define ST7735_BGR       0x40 // BGR color order

#define ST7735_SIZE(displayType)      ((displayType) & 0x0f)
#define ST7735_IS_BGR(displayType)      ((displayType) & (ST7735_BGR))

// TFT 1.8 128 x 160
#define ST7735_SIZE_180  0x00
// TFT 1.8  80 x 160
#define ST7735_SIZE_096  0x01

// TFT 1.8 128 x 160
#define ST7735_TYPE_180  (ST7735_SIZE_180)
// OLED 096  80 x 160
#define ST7735_TYPE_OLED_096  ((ST7735_SIZE_096) | (ST7735_INVERTED) | (ST7735_BGR))

// 1.8" TFT display constants
#define ST7735_180_XSIZE   ST7735_XSIZE
#define ST7735_180_YSIZE   ST7735_YSIZE
#define ST7735_180_XOFFSET ST7735_XOFFSET
#define ST7735_180_YOFFSET ST7735_YOFFSET

// TFT096 values
#define ST7735_096_XSIZE   80
#define ST7735_096_YSIZE   160
#define ST7735_096_XOFFSET 26
#define ST7735_096_YOFFSET 1

class St7735
{
public:
    uint8_t flags;
    uint16_t maxX;
    uint16_t maxY;
    uint8_t offsetX;
    uint8_t offsetY;

    color_t foreground;
    color_t background;
    alpha_t alpha;

    uint8_t colOffset;      // char column pixel offset
    uint8_t rowOffset;      // char row pixel offset
    int16_t col;            // current column
    int16_t row;            // current row
    uint8_t maxCols;
    uint8_t maxRows;

    // with type conversions
    explicit St7735(uint8_t typeFlags = 0);
    void setOrientation(uint8_t rot);
    void initDisplay(uint8_t rot);
    void setInverted();
    void clearInverted();

    // raw, no type conversions
    void initDisplay(uint8_t rot, uint16_t xSize, uint16_t ySize, uint8_t xOffset, uint8_t yOffset, int8_t inversion);
    void setOrientation(uint8_t rot, uint16_t xSize, uint16_t ySize, uint8_t xOffset, uint8_t yOffset);

    bool isBGR();
    color_t rgb(uint8_t r, uint8_t g, uint8_t b);

    void setGama(uint8_t gama);
    void sendByte(uint8_t byte);
    void sendCmd(uint8_t cmd);
    void sendWord(uint16_t word);
    void sendColor(color_t color);
    void sendColors(color_t color, uint16_t count);
    void send565(color_t color, uint16_t count);
    void hardwareReset();
    void sendSetAddrWindow(int x0, int y0, int x1, int y1);
    void clearScreen();
    void setPixel(int x, int y, color_t color, alpha_t alpha);
    void sendPixels(color_t color, alpha_t alpha, uint16_t count);
    void hLine(int x0, int x1, int y, color_t color);
    void vLine(int x, int y0, int y1, color_t color);
    void line(int x0, int y0, int x1, int y1, color_t color);
    void rect(int x0, int y0, int x1, int y1, color_t color);
    void fillRect(int x0, int y0, int x1, int y1, color_t color);

    void circleQuadrant(int xPos, int yPos, int radius, uint8_t quads, color_t color);
    void circle(int xPos, int yPos, int radius, color_t color);
    void roundRect(int x0, int y0, int x1, int y1, int r, color_t color);
    void fillCircle(int xPos, int yPos, int radius, color_t color);
    void ellipse(int x0, int y0, int width, int height, color_t color);
    void fillEllipse(int xPos, int yPos, int width, int height, color_t color);
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
    static uint8_t blend(uint8_t comp1, uint8_t comp2, alpha_t alpha);
    static uint16_t blendColors(color_t c1, color_t c2, alpha_t alpha);
};

// helper functions
// calculate integer value of square root

#endif // _TFT18_H_
