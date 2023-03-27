#ifndef _SSD1306_DISPLAY_H
#define _SSD1306_DISPLAY_H

#include <stdint.h>

#define SSD1306_OLED_TYPE 91
#define SSD1306_CONFIG_FLAGS SSD1306_SWITCHCAPVCC

// this adds about 650 bytes to flash use and 7 bytes of RAM, but more than doubles character output to screen
#ifndef SSD1306_NO_BIT_BLIT
#define SSD1306_BIT_BLIT
#endif

#ifndef CONSOLE_DEBUG

// adds 646 bytes of FLASH use to add smoother double size chars for 0-9,m,l, and .
//#define SSD1306_LARGE_FONT

// adds 372 bytes of FLASH use
//#define SSD1306_SMALL_FONT

#ifdef __cplusplus
typedef const __FlashStringHelper *PGM_STR;
#endif

#else

#define SSD1306_SMALL_FONT
#define SSD1306_LARGE_FONT

typedef const char *PGM_P;
typedef const char *PGM_STR;
#define PSTR(s)  s
//#define SSD1306_SWITCHCAPVCC 1

#endif

typedef int16_t coord_x;
typedef int8_t coord_y;

#endif //_SSD1306_DISPLAY_H
