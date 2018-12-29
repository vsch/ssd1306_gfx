#ifndef _SSD1306_COLORS_H
#define _SSD1306_COLORS_H

// RGB(r,g,b) needs to be defined depending on RGB or BGR order
#define RGB(r, g, b) _RGB(b, g, r)

// Color constants
#define BLACK   RGB(0,0,0)
#define WHITE   RGB(255,255,255)

#if SSD1306_SIZE(DISPLAY_TYPE) == SSD1306_SIZE_096
// TFT180 values
#define DISPLAY_XSIZE   SSD1306_096_XSIZE
#define DISPLAY_YSIZE   SSD1306_096_YSIZE
#elif SSD1306_SIZE(DISPLAY_TYPE) == SSD1306_SIZE_091
// TFT096 values
#define DISPLAY_XSIZE   SSD1306_091_XSIZE
#define DISPLAY_YSIZE   SSD1306_091_YSIZE
#else
#error "Unsupported Display Type "
#endif

#define CHAR_WIDTH SSD1306_CHAR_WIDTH
#define CHAR_HEIGHT SSD1306_CHAR_HEIGHT

#endif //_SSD1306_COLORS_H
