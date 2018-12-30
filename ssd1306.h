#ifndef _SSD1306_H_
#define _SSD1306_H_

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
    int8_t charCol;         // current column
    int8_t charRow;         // current row
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
    void hLineRaw(int x0, int x1, int y, color_t color);
    void vLineRaw(int x, int y0, int y1, color_t color);
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
    void charCenterOffset();
    void gotoCharXY(int col, int row);
    void advanceCursor();
    void putCh(char ch, int x, int y);
    void write(char ch);
    void write(char ch, int count);
    void write(const char *text);
    void write(const __FlashStringHelper *text);
    void write(long i);
    void write(unsigned long i);
    void writeHex(long i);
    void writeHex(int i);
    void writeDigit(int dig);
    void write(int i);

    uint8_t write(const char *text, uint8_t count);
    void write(const char *buff, uint8_t pos, char c);
    void write(unsigned long i, uint8_t pos, char c);
    void write(long i, uint8_t pos, char c);

    void openSPI();
    void closeSPI();

    static int intSqrt(long val);
};

// helper functions
// calculate integer value of square root

#endif // _SSD1306_H_
