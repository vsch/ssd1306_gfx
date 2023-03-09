///**************************************************************************
// This is an example for our Monochrome OLEDs based on SSD1306 drivers
//
// Pick one up today in the adafruit shop!
// ------> http://www.adafruit.com/category/63_98
//
// This example is for a 128x32 pixel display using I2C to communicate
// 3 pins are required to interface (two I2C and one reset).
//
// Adafruit invests time and resources providing this open
// source code, please support Adafruit and open-source
// hardware by purchasing products from Adafruit!
//
// Written by Limor Fried/Ladyada for Adafruit Industries,
// with contributions from the open source community.
// BSD license, check license.txt for more information
// All text above, and the splash screen below must be
// included in any redistribution.
// **************************************************************************/
//
//#include <Arduino.h>
//#include <avr/io.h>         // deal with port registers
//#include <avr/interrupt.h>  // deal with interrupt calls
//#include <util/delay.h>     // used for _delay_ms function
//#include <string.h>         // string manipulation routines
//#include <avr/sleep.h>      // used for sleep functions
//#include <stdlib.h>
//#include <Wire.h>
//#include "ssd1306.h"
//
//#define DISPLAY_TYPE SSD1306_TYPE_OLED_091 /*| SSD1306_INVERTED*/ /*| SSD1306_EXTERNALVCC*/
//
//#include "ssd1306_display.h"
//
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels
//
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//// The pins for I2C are defined by the Wire-library.
//// On an arduino UNO:       A4(SDA), A5(SCL)
//// On an arduino MEGA 2560: 20(SDA), 21(SCL)
//// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
//uint8_t pageBuffer[DISPLAY_XSIZE];
//Ssd1306 display = Ssd1306(pageBuffer, DISPLAY_TYPE);
//
//void testdrawstyles(void) {
//    display.clearDisplay();
//
//    display.setTextSize(1);             // Normal 1:1 pixel scale
//    display.setTextColor(SSD1306_WHITE);        // Draw white text
//    display.setCursor(0,0);             // Start at top-left corner
//    display.println(F("Hello, world!"));
//
//    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
//    display.println(3.141592);
//
//    display.setTextSize(2);             // Draw 2X-scale text
//    display.setTextColor(SSD1306_WHITE);
//    display.print(F("0x")); display.println(0xDEADBEEF, HEX);
//
//    display.display();
//    delay(2000);
//}
//
//void setup() {
//    Serial.begin(9600);
//
//    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//#include <Arduino.h>
//#include <avr/io.h>         // deal with port registers
//#include <avr/interrupt.h>  // deal with interrupt calls
//#include <util/delay.h>     // used for _delay_ms function
//#include <string.h>         // string manipulation routines
//#include <avr/sleep.h>      // used for sleep functions
//#include <stdlib.h>
//#include <Wire.h>
//#include "ssd1306.h"
//
//#define DISPLAY_TYPE SSD1306_TYPE_OLED_091 /*| SSD1306_INVERTED*/ /*| SSD1306_EXTERNALVCC*/
//
//#include "ssd1306_display.h"
//
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels
//
//// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//// The pins for I2C are defined by the Wire-library.
//// On an arduino UNO:       A4(SDA), A5(SCL)
//// On an arduino MEGA 2560: 20(SDA), 21(SCL)
//// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
//    uint8_t pageBuffer[DISPLAY_XSIZE];
//    Ssd1306 display = Ssd1306(pageBuffer, DISPLAY_TYPE);
//
//    display.initDisplay(SSD1306_ROT_0);                              // initialize TFT controller
//
//    // Clear the buffer
//    display.clearDisplay();
//
//    testdrawstyles();    // Draw 'stylized' characters
//
//    // Invert and restore display, pausing in-between
//    display.setInverted();
//    delay(1000);
//    display.clearInverted();
//    delay(1000);
//}
//
//void loop() {
//}
//