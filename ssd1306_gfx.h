//
// Created by Vladimir Schneider on 2023-03-08.
//

#ifndef SSD1306_SSD1306_GFX_H
#define SSD1306_SSD1306_GFX_H

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

#define SSD1306_FLAG_TEXT_DOUBLE_SIZE (SSD1306_FLAG_TEXT_DOUBLE_WIDTH | SSD1306_FLAG_TEXT_DOUBLE_HEIGHT)

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

// display flags
#define SSD1306_FLAG_NONE  0
#define SSD1306_FLAG_TEXT_BORDER                0x01  // add left and top text borders in background color
#define SSD1306_FLAG_TEXT_DOUBLE_WIDTH          0x02
#define SSD1306_FLAG_TEXT_DOUBLE_HEIGHT         0x04
#define SSD1306_FLAG_TEXT_WRAP                  0x08
#define SSD1306_FLAG_TEXT_WRAPPED               0x10  // text was wrapped, so now we skip spaces until non-space appears
#define SSD1306_FLAG_CONTINUED_PATH             0x20  // cursor position represents a filled pixel that start of lineTo should not draw
#define SSD1306_FLAG_BORDER_TEXT_LINE           0x40  // set when char needs top border because it is the first line of text
#define SSD1306_FLAG_BORDER_TEXT_CHAR           0x80  // set when char needs left border because it is first in line


#ifdef __cplusplus
extern "C" {
#endif

// display functions
extern void ssd1306_clearDisplay();

// graphics functions/variables
extern uint8_t ssd1306_sendPos;

extern uint8_t ssd1306_flags;           // option flags
extern uint8_t ssd1306_cSizeX;          // char width updated when text size flags change
extern uint8_t ssd1306_cSizeY;          // char height updated when text size flags change

extern int16_t ssd1306_cX; // cursor
extern int16_t ssd1306_cLeft; // left margin for text wrapping, normally 0, cX will be set to this on wrapping
extern int16_t ssd1306_cRight; // right margin for text wrapping, normally DISPLAY_XSIZE
extern int8_t ssd1306_cY; // cursor

extern color_t ssd1306_foreColor;
extern color_t ssd1306_gapColor;            // dash gap color
extern color_t ssd1306_backColor;

extern uint8_t ssd1306_dashBits;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashSize;            // solid/dash/dot pattern for line outlines (not text)
extern uint8_t ssd1306_dashOffset;          // solid/dash/dot pattern for line outlines (not text)

extern uint8_t ssd1306_displayBuffer[DISPLAY_YSIZE/8][DISPLAY_XSIZE];      // display buffer

// user display operations
// primitives used in library
extern uint8_t ssd1306_nextDashPixelColor();
extern uint8_t ssd1306_nextDashBit();
extern void ssd1306_setPixel(int16_t x, int8_t y, color_t color);
extern void ssd1306_drawCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t octs);
extern void ssd1306_fillCircleOctants(int16_t cx, int8_t cy, int16_t x, int8_t y, uint8_t dummy);
extern void ssd1306_circleQuadrant(int8_t radius, uint8_t quads);

typedef void (*circleOctants)(int16_t, int8_t, int16_t, int8_t, uint8_t);
extern void ssd1306_circleOctants(int8_t r, uint8_t asQuadrants, uint8_t octs, circleOctants drawOctants);

extern void ssd1306_clearDisplay();

extern void ssd1306_hLine(int16_t x0, int8_t y0, int16_t x1, color_t color);
extern void ssd1306_vLine(int16_t x0, int8_t y0, int8_t y1, color_t color);
extern void ssd1306_hLineTo(int16_t x1);            // draw horizontal line using foreground
extern void ssd1306_vLineTo(int8_t y1);            // draw vertical line using foreground
extern void ssd1306_lineTo(int16_t x1, int8_t y1); // draw line using foreground
extern void ssd1306_rect(int16_t x1, int8_t y1);   // draw rectangle outline in foreground, fill in background

extern void ssd1306_circle(int8_t radius);         // draw circle centered on cursor, outline in foreground, fill in background
extern void ssd1306_roundRect(int16_t x1, int8_t y1, int8_t r);   // draw rounded rectangle outline in foreground, fill in background

#ifdef SSD1306_FANCY
extern void ssd1306_ellipse(int8_t width, int8_t height);         // draw ellipse centered on cursor, outline in foreground, fill in background
#endif

extern void ssd1306_updateCharSize();
extern void ssd1306_setTextFlags(uint8_t flags); // double wide and/or double height
extern uint8_t ssd1306_isCharVisible();
extern bool ssd1306_putCh(char ch);

extern void ssd1306_writeChar(char ch);
extern void ssd1306_writeRepeatedChars(char ch, uint8_t count);
extern void ssd1306_writeText(const char *str);
extern void ssd1306_writePgmText(PGM_P str);
extern void ssd1306_writeInt32Pad(int32_t i, char ch, uint8_t pad);
extern void ssd1306_writeInt32(int32_t i);
extern void ssd1306_writeUInt32(uint32_t i);
extern void ssd1306_writeUInt32Pad(uint32_t i, uint8_t radix, char ch, uint8_t pad);
extern void ssd1306_writeInt16Pad(int16_t i, char ch, uint8_t pad);
extern void ssd1306_writeInt16(int16_t i);
extern void ssd1306_writeUInt16(uint16_t i);
extern void ssd1306_writeUInt16Pad(uint16_t i, uint8_t radix, char ch, uint8_t pad);
extern void ssd1306_writeInt8Pad(int8_t i, char ch, uint8_t pad);
extern void ssd1306_writeInt8(int8_t i);
extern void ssd1306_writeUInt8(uint8_t i);
extern void ssd1306_writeUInt8Pad(uint8_t i, uint8_t radix, char ch, uint8_t pad);
extern void ssd1306_writeDigit(uint8_t dig);

#ifdef CONSOLE_DEBUG
extern void display();
#endif

#ifdef __cplusplus
}
#endif

#endif //SSD1306_SSD1306_GFX_H
