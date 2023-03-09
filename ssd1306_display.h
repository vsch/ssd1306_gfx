#ifndef _SSD1306_DISPLAY_H
#define _SSD1306_DISPLAY_H

#define SSD1306_OLED_TYPE 91
#define SSD1306_CONFIG_FLAGS SSD1306_SWITCHCAPVCC

#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
//#define SERIAL_DEBUG_GFX

#ifdef __cplusplus
extern "C" {
#endif

extern void serial_print(const char *s);
extern void serial_println(const char *s);
extern void serial_printPgm(const char *s);
extern void serial_printPgmln(const char *s);
extern void serial_printC(char c);

#ifdef __cplusplus
}
#endif

#endif

#endif //_SSD1306_DISPLAY_H
