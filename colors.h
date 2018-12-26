#ifndef ST7735_COLORS_H
#define ST7735_COLORS_H

// RGB(r,g,b) needs to be defined depending on RGB or BGR order
#ifdef ST7735_IS_BGR(DISPLAY_TYPE)
#define RGB(r, g, b) _RGB(b, g, r)
#else
#define RGB(r, g, b) _RGB(r, g, b)
#endif

// Color constants
#define BLACK   RGB(0,0,0)
#define BLUE    RGB(0,0,255)
#define RED     RGB(255,0,0)
#define GREEN   RGB(0,255,0)
#define LIME    RGB(224,255,0)
#define CYAN    RGB(0,255,255)
#define MAGENTA RGB(255,0,255)
#define YELLOW  RGB(255,255,0)
#define WHITE   RGB(255,255,255)

#if ST7735_SIZE(DISPLAY_TYPE) == ST7735_SIZE_180
// TFT180 values
#define DISPLAY_XSIZE   ST7735_180_XSIZE
#define DISPLAY_YSIZE   ST7735_180_YSIZE
#elif ST7735_SIZE(DISPLAY_TYPE) == ST7735_SIZE_096
// TFT096 values
#define DISPLAY_XSIZE   ST7735_096_XSIZE
#define DISPLAY_YSIZE   ST7735_096_YSIZE
#else
#error "Unsupported Display Type "
#endif

#endif //ST7735_COLORS_H
