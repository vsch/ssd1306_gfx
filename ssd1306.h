#ifndef _SSD1306_H_
#define _SSD1306_H_

#ifndef clearBit
#define clearBit(x, y) (x &= ~_BV(y))     // equivalent to cbi(x,y)
#endif clearBit

#ifndef setBit
#define setBit(x, y) (x |= _BV(y))        // equivalent to sbi(x,y)
#endif setBit

#ifdef __cplusplus
extern "C" {
#endif
#include "twi.h"

extern uint8_t ssd1306_sendBuffer[TWI_BUFFER_LENGTH];
extern uint8_t ssd1306_sendPos;

// initialization
// config flag values
#define SSD1306_SWITCHCAPVCC    0x01 ///< Gen. display voltage from 3.3V, otherwise external vcc is used

extern void ssd1306_initDisplay();

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
extern void ssd1306_display();

#ifdef __cplusplus
}
#endif
// helper functions

#endif // _SSD1306_H_
