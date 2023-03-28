#ifndef _SSD1306_H_
#define _SSD1306_H_

#ifndef clearBit
#define clearBit(x, y) (x &= ~_BV(y))     // equivalent to cbi(x,y)
#endif // clearBit

#ifndef setBit
#define setBit(x, y) (x |= _BV(y))        // equivalent to sbi(x,y)
#endif // setBit

#ifdef __cplusplus
extern "C" {
#endif

#include "twi.h"

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif //TWI_BUFFER_LENGTH

extern uint8_t gfx_send_buffer[TWI_BUFFER_LENGTH];
extern uint8_t gfx_send_pos;

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
extern uint32_t gfx_send_time;
extern uint16_t gfx_send_bytes;
#endif

// config flag values
#ifndef SSD1306_SWITCHCAPVCC
#define SSD1306_SWITCHCAPVCC    0x01 ///< Gen. display voltage from 3.3V, otherwise external vcc is used
#endif

// internally used functions for communication to SSD1306 via TWI
// command transaction markers
extern void gfx_start_twi_cmd_frame();
extern void gfx_end_twi_frame();
extern void gfx_send_cmd(uint8_t cmd);

// sending operations
extern void gfx_twi_byte(uint8_t byte);
extern void gfx_twi_pgm_byte_list(const uint8_t *bytes, uint16_t count);

#ifdef __cplusplus
}
#endif

#endif // _SSD1306_H_
