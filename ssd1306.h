#ifndef _SSD1306_H_
#define _SSD1306_H_

// 128x64 display constants
#define SSD1306_096_XSIZE   128
#define SSD1306_096_YSIZE   64
#define SSD1306_096_XOFFSET 0
#define SSD1306_096_YOFFSET 0

// 128x32 display constants
#define SSD1306_091_XSIZE   128
#define SSD1306_091_YSIZE   32
#define SSD1306_091_XOFFSET 0
#define SSD1306_091_YOFFSET 0

#define SSD1306_CHAR_X_PIXELS 5
#define SSD1306_CHAR_Y_PIXELS 7
#define SSD1306_CHAR_WIDTH (SSD1306_CHAR_X_PIXELS+1)
#define SSD1306_CHAR_HEIGHT (SSD1306_CHAR_Y_PIXELS+1)

#ifndef SSD1306_OLED_TYPE
#define SSD1306_OLED_TYPE 91
#endif

#if SSD1306_OLED_TYPE == 96
#define DISPLAY_XSIZE   SSD1306_096_XSIZE
#define DISPLAY_YSIZE   SSD1306_096_YSIZE
#define DISPLAY_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#elif SSD1306_OLED_TYPE == 91
#define DISPLAY_XSIZE   SSD1306_091_XSIZE
#define DISPLAY_YSIZE   SSD1306_091_YSIZE
#define DISPLAY_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#else
#error "Unsupported Display Type "
#endif

#define CHAR_WIDTH SSD1306_CHAR_WIDTH
#define CHAR_HEIGHT SSD1306_CHAR_HEIGHT

#ifndef clearBit
#define clearBit(x, y) (x &= ~_BV(y))     // equivalent to cbi(x,y)
#endif clearBit

#ifndef setBit
#define setBit(x, y) (x |= _BV(y))        // equivalent to sbi(x,y)
#endif setBit

typedef uint8_t color_t;

// Color constants
#define SSD1306_COLOR_BLACK     0  // black or off
#define SSD1306_COLOR_WHITE     1  // white or on
#define SSD1306_COLOR_INVERT    2  // invert
#define SSD1306_COLOR_NONE      3  // leave as is

#define SSD1306_ROT_0     0
#define SSD1306_ROT_90    1
#define SSD1306_ROT_180   2
#define SSD1306_ROT_270   3

// display flags
#define SSD1306_FLAG_NONE  0
#define SSD1306_FLAG_DOUBLE_LINE_THICKNESS      0x01
#define SSD1306_FLAG_TEXT_DOUBLE_WIDE           0x02
#define SSD1306_FLAG_TEXT_DOUBLE_HEIGHT         0x04
#define SSD1306_FLAG_TEXT_WRAP                  0x08

#define SSD1306_FLAG_FONT_DOUBLE_SIZE (SSD1306_FLAG_FONT_DOUBLE_WIDE | SSD1306_FLAG_FONT_DOUBLE_HEIGHT)

#define SSD1306_BITS_DASH_NONE           0b1
#define SSD1306_SIZE_DASH_NONE           1
#define SSD1306_BITS_DOT                 0b01
#define SSD1306_SIZE_DOT                 2
#define SSD1306_BITS_SPARSE_DOT          0b001
#define SSD1306_SIZE_SPARSE_DOT          3
#define SSD1306_BITS_DASH                0b0011
#define SSD1306_SIZE_DASH                4
#define SSD1306_BITS_LONG_DASH           0b00111
#define SSD1306_SIZE_LONG_DASH           5
#define SSD1306_BITS_DASH_DOT            0b01011
#define SSD1306_SIZE_DASH_DOT            5

#ifdef __cplusplus
extern "C" {
#endif

// initialization
// config flag values
#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V
extern void ssd1306_initDisplay(uint8_t configFlags);

// internally used functions for communication to SSD1306 via TWI
// command transaction markers
extern void ssd1306_startTwiCmdFrame();
extern void ssd1306_startTwiDataFrame();
extern void ssd1306_endTwiFrame();

extern void ssd1306_sendCmd(uint8_t cmd);

// sending operations
extern void ssd1306_twiByte(uint8_t byte);
extern void ssd1306_twiRepeatedByte(uint8_t byte, uint16_t count);
extern void ssd1306_twiPgmByteList(const uint8_t *bytes, uint16_t count);
extern void ssd1306_twiByteList(uint8_t *bytes, uint16_t count);

// display functions
extern void ssd1306_setInverted();
extern void ssd1306_clearInverted();
extern void ssd1306_displayOff();
extern void ssd1306_displayOn();
extern void ssd1306_setContrast(uint8_t contrast);
void ssd1306_clearDisplay();
void ssd1306_display();

// graphics functions/variables
extern uint8_t ssd1306_sendBuffer[TWI_BUFFER_LENGTH];
extern uint8_t ssd1306_sendPos;

extern uint8_t ssd1306_flags;               // option flags

extern int8_t ssd1306_cY; // cursor
extern int8_t ssd1306_cX; // cursor
extern int8_t ssd1306_cXRight; // right margin for text wrapping, normally DISPLAY_XSIZE

extern color_t ssd1306_foreColor;
extern color_t ssd1306_backColor;

extern uint8_t ssd1306_dashBits;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashSize;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

extern uint8_t ssd1306_displayBuffer[DISPLAY_YSIZE][DISPLAY_XSIZE / 8];      // display buffer

// user display operations
// primitives used in library
extern uint8_t ssd1306_nextDashPixelColor();
extern uint8_t ssd1306_nextDashBit();
extern void setPixel(int8_t x, int8_t y, color_t color);
extern void ssd1306_drawCircleOctants(int8_t cx, int8_t cy, int8_t x, int8_t y, uint8_t octs);
extern void ssd1306_fillCircleOctants(int8_t cx, int8_t cy, int8_t x, int8_t y, uint8_t dummy);
extern void ssd1306_circleQuadrant(int8_t radius, uint8_t quads);

extern void ssd1306_clearDisplay();
extern void ssd1306_clipCursor();

extern void ssd1306_hLine(int8_t x0, int8_t y0, int8_t x1, color_t color);
extern void ssd1306_vLine(int8_t x0, int8_t y0, int8_t y1, color_t color);
extern void ssd1306_hLineTo(int8_t y1);            // draw horizontal line using foreground
extern void ssd1306_vLineTo(int8_t x1);            // draw vertical line using foreground
extern void ssd1306_lineTo(int8_t x1, int8_t y1); // draw line using foreground
extern void ssd1306_rect(int8_t x1, int8_t y1);   // draw rectangle outline in foreground, fill in background

extern void ssd1306_circle(int8_t radius);         // draw circle centered on cursor, outline in foreground, fill in background
extern void ssd1306_roundRect(int8_t x1, int8_t y1, int8_t r);   // draw rounded rectangle outline in foreground, fill in background


#ifdef SSD1306_FANCY
extern void ssd1306_ellipse(int8_t width, int8_t height);         // draw ellipse centered on cursor, outline in foreground, fill in background
extern void ssd1306_putCh(char ch, int x, int y);
extern void ssd1306_writeChar(char ch);
extern void ssd1306_writeRepeatedChars(char ch, int count);
extern void ssd1306_writeText(const char *text);
extern void ssd1306_writePgmText(const char *text);
extern void ssd1306_writeLong(long i);
extern void ssd1306_writeULong(unsigned long i);
extern void ssd1306_writeULongRadix(unsigned long i, uint8_t radix);
extern void ssd1306_writeInt(int i);
extern void ssd1306_writeUInt(unsigned int i);
extern void ssd1306_writeUIntRadix(unsigned int i, uint8_t radix);
extern void ssd1306_writeDigit(uint8_t dig);

extern uint8_t ssd1306_writeTextSubStr(const char *text, uint8_t count);
#endif

#ifdef __cplusplus
}
#endif
// helper functions

#endif // _SSD1306_H_
