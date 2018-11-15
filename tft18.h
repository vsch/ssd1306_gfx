#ifndef _TFT18_H_
#define _TFT18_H_

#include "Arduino.h"
#include <stdlib.h>

// Include header file that defines the fonts loaded and the pins to be used
#include "User_Setup.h"

#define RGB565(r, g, b) ((uint16_t)((((r) & 0xF8) << 8) | ((int)((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3)))

#define RGB666(r, g, b) ((uint8_t)(r) << 16 | (uint8_t)(g) << 8 | (uint8)(b))

#define RGB565_TO_RGB888(c) (((long)((c) & 0xF800) << 8) | (((long)((c) & 0x07E0) << 5)) | (((long)((c) & 0x001F) << 1)))

#define RGB565_RED(c) ((uint8_t)(c >> 8) & (uint8_t)0xF8)
#define RGB565_GREEN(c) ((uint8_t)(c >> 3) & (uint8_t)0xFC)
#define RGB565_BLUE(c) ((uint8_t)(c << 3) & (uint8_t)0xF8)

#ifndef clearBit
#define clearBit(x, y) (x &= ~_BV(y))     // equivalent to cbi(x,y)
#endif clearBit

#ifndef setBit
#define setBit(x, y) (x |= _BV(y))        // equivalent to sbi(x,y)
#endif setBit

// DC 8
#define TFT_DC_PORT  PORTD
#define TFT_DC_BIT   5
#define TFT_DC       TFT_DC_PORT,TFT_DC_BIT

// DC 9
#define TFT_RST_PORT PORTB
#define TFT_RST_BIT  1
#define TFT_RST       TFT_RST_PORT,TFT_RST_BIT

// DC 10
#define TFT_CS_PORT  PORTB
#define TFT_CS_BIT   2
#define TFT_CS       TFT_CS_PORT,TFT_CS_BIT

// DC 11
#define TFT_MOSI_PORT PORTB
#define TFT_MOSI_BIT  3
#define TFT_MOSI       TFT_MOSI_PORT,TFT_MOSI_BIT

// so we can turn off SPI and use these manually to try and read pixels
// DC 12
#define TFT_MISO_PORT PORTB
#define TFT_MISO_BIT  4
#define TFT_MISO       TFT_MISO_PORT,TFT_MISO_BIT

// DC 13
#define TFT_SCK_PORT  PORTB
#define TFT_SCK_BIT   5
#define TFT_SCK       TFT_SCK_PORT,TFT_SCK_BIT

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

// 1.8" TFT display constants
#define TFT_XSIZE   128
#define TFT_YSIZE   160

#define TFT_CHAR_X_PIXELS 5
#define TFT_CHAR_Y_PIXELS 7
#define TFT_CHAR_WIDTH (TFT_CHAR_X_PIXELS+1)
#define TFT_CHAR_HEIGHT (TFT_CHAR_Y_PIXELS+1)

// mode 5 = 16bit pixels (RGB565)
// mode 6 = 18bit pixels (RGB666)
#define COLMOD_PARAM 5

// 4 LSB's one of selects GC0 - GC3
#define GAMSET_PARAM 1

#if COLMOD_PARAM == 5
#define RGB(r, g, b) RGB565(r,g,b)
#else
#define RGB(r,g,b) RGB666(r,g,b)
#endif

typedef uint16_t color_t;
typedef uint8_t alpha_t;

// Color constants
#define BLACK   RGB(0,0,0)
#define BLUE    RGB(0,0,255)
#define RED     RGB(255,0,0)
#define GREEN   RGB(0,255,0)
#define LIME    RGB(128,255,0)
#define CYAN    RGB(0,255,255)
#define MAGENTA RGB(255,0,255)
#define YELLOW  RGB(255,255,0)
#define WHITE   RGB(255,255,255)

#define TFT_ROT_0     0
#define TFT_ROT_90    1
#define TFT_ROT_180   2
#define TFT_ROT_270   3

class Tft18
{
public:
    int maxX;
    int maxY;

    color_t foreground;
    color_t background;
    alpha_t alpha;

    int colOffset;      // max columns
    int rowOffset;      // max rows
    uint8_t col;        // current column
    uint8_t row;        // current row
    uint8_t maxCol;
    uint8_t maxRow;

    explicit Tft18();
    void setOrientation(int rot);
    void setGama(uint8_t gama);
    void sendByte(uint8_t byte);
    void sendCmd(uint8_t cmd);
    void sendWord(uint16_t word);
    void sendColor(color_t color);
    void sendColors(color_t color, uint16_t count);
    void send565(color_t color, uint16_t count);
    void hardwareReset();
    void initDisplay(uint8_t rot = TFT_ROT_0);
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
    void charOffset(int x, int y);
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
