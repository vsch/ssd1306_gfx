/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x32 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Arduino.h>
#include <stdlib.h>
#include "ssd1306_display.h"
#include "ssd1306.h"
#include "ssd1306_gfx.h"
#include "Ssd1306Display.h"

Ssd1306Display display;

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#ifdef SERIAL_DEBUG
extern "C" {
void serial_printC(char c) {
    Serial.print(c);
}
void serial_print(const char *s) {
    Serial.print(s);
}
void serial_println(const char *s) {
    Serial.println(s);
}
void serial_printPgm(PGM_P s) {
    Serial.print((const __FlashStringHelper *) s);
}
void serial_printPgmln(PGM_P s) {
    Serial.println((const __FlashStringHelper *) s);
}
};
#endif

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
        {0b00000000, 0b11000000,
         0b00000001, 0b11000000,
         0b00000001, 0b11000000,
         0b00000011, 0b11100000,
         0b11110011, 0b11100000,
         0b11111110, 0b11111000,
         0b01111110, 0b11111111,
         0b00110011, 0b10011111,
         0b00011111, 0b11111100,
         0b00001101, 0b01110000,
         0b00011011, 0b10100000,
         0b00111111, 0b11100000,
         0b00111111, 0b11110000,
         0b01111100, 0b11110000,
         0b01110000, 0b01110000,
         0b00000000, 0b00110000};

void testdrawline() {
    int16_t i;

    display.clearScreen(); // Clear display buffer

    for (i = 0; i < DISPLAY_XSIZE; i += 4) {
        display.drawLine(0, 0, i, DISPLAY_YSIZE - 1, SSD1306_WHITE);
        display.display(); // Update screen with each newly-drawn line
        delay(1);
    }
    for (i = 0; i < DISPLAY_YSIZE; i += 4) {
        display.drawLine(0, 0, DISPLAY_XSIZE - 1, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearScreen();

    for (i = 0; i < DISPLAY_XSIZE; i += 4) {
        display.drawLine(0, DISPLAY_YSIZE - 1, i, 0, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = DISPLAY_YSIZE - 1; i >= 0; i -= 4) {
        display.drawLine(0, DISPLAY_YSIZE - 1, DISPLAY_XSIZE - 1, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearScreen();

    for (i = DISPLAY_XSIZE - 1; i >= 0; i -= 4) {
        display.drawLine(DISPLAY_XSIZE - 1, DISPLAY_YSIZE - 1, i, 0, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = DISPLAY_YSIZE - 1; i >= 0; i -= 4) {
        display.drawLine(DISPLAY_XSIZE - 1, DISPLAY_YSIZE - 1, 0, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    delay(250);

    display.clearScreen();

    for (i = 0; i < DISPLAY_YSIZE; i += 4) {
        display.drawLine(DISPLAY_XSIZE - 1, 0, 0, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }
    for (i = 0; i < DISPLAY_XSIZE; i += 4) {
        display.drawLine(DISPLAY_XSIZE - 1, 0, i, DISPLAY_YSIZE - 1, SSD1306_WHITE);
        display.display();
        delay(1);
    }

    delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
    display.clearScreen();

    for (int16_t i = 0; i < DISPLAY_YSIZE / 2; i += 2) {
        display.drawRect(i, i, DISPLAY_XSIZE - 2 * i, DISPLAY_YSIZE - 2 * i, SSD1306_WHITE);
        display.display(); // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(2000);
}

void testfillrect(void) {
    display.clearScreen();

    for (int16_t i = 0; i < DISPLAY_YSIZE / 2; i += 3) {
        // The INVERSE color is used so rectangles alternate white/black
        display.fillRect(i, i, DISPLAY_XSIZE - i * 2, DISPLAY_YSIZE - i * 2, SSD1306_INVERSE);
        display.display(); // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(2000);
}

void testdrawcircle(void) {
    display.clearScreen();

    for (int16_t i = 0; i < max(DISPLAY_XSIZE, DISPLAY_YSIZE) / 2; i += 2) {
        display.drawCircle(DISPLAY_XSIZE / 2, DISPLAY_YSIZE / 2, i, SSD1306_WHITE);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testfillcircle(void) {
    display.clearScreen();

    for (int16_t i = max(DISPLAY_XSIZE, DISPLAY_YSIZE) / 2; i > 0; i -= 3) {
        // The INVERSE color is used so circles alternate white/black
        display.fillCircle(DISPLAY_XSIZE / 2, DISPLAY_YSIZE / 2, i, SSD1306_INVERSE);
        display.display(); // Update screen with each newly-drawn circle
        delay(1);
    }

    delay(2000);
}

void testdrawroundrect(void) {
    display.clearScreen();

    for (int16_t i = 0; i < DISPLAY_YSIZE / 2 - 2; i += 2) {
        display.drawRoundRect(i, i, DISPLAY_XSIZE - 2 * i, DISPLAY_YSIZE - 2 * i,
                              DISPLAY_YSIZE / 4, SSD1306_WHITE);
        display.display();
        delay(1);
    }

    delay(2000);
}

void testfillroundrect(void) {
    display.clearScreen();

    for (int16_t i = 0; i < DISPLAY_YSIZE / 2 - 2; i += 2) {
        // The INVERSE color is used so round-rects alternate white/black
        display.fillRoundRect(i, i, DISPLAY_XSIZE - 2 * i, DISPLAY_YSIZE - 2 * i,
                              DISPLAY_YSIZE / 4, SSD1306_INVERSE);
        display.display();
        delay(1);
    }

    delay(2000);
}

//void testdrawtriangle(void) {
//    display.clearDisplay();
//
//    for(int16_t i=0; i<max(DISPLAY_XSIZE,DISPLAY_YSIZE)/2; i+=5) {
//        display.drawTriangle(
//                DISPLAY_XSIZE/2  , DISPLAY_YSIZE/2-i,
//                DISPLAY_XSIZE/2-i, DISPLAY_YSIZE/2+i,
//                DISPLAY_XSIZE/2+i, DISPLAY_YSIZE/2+i, SSD1306_WHITE);
//        display.display();
//        delay(1);
//    }
//
//    delay(2000);
//}
//
//void testfilltriangle(void) {
//    display.clearDisplay();
//
//    for(int16_t i=max(DISPLAY_XSIZE,DISPLAY_YSIZE)/2; i>0; i-=5) {
//        // The INVERSE color is used so triangles alternate white/black
//        display.fillTriangle(
//                DISPLAY_XSIZE/2  , DISPLAY_YSIZE/2-i,
//                DISPLAY_XSIZE/2-i, DISPLAY_YSIZE/2+i,
//                DISPLAY_XSIZE/2+i, DISPLAY_YSIZE/2+i, SSD1306_INVERSE);
//        display.display();
//        delay(1);
//    }
//
//    delay(2000);
//}

void testdrawchar(void) {
    display.clearScreen();

    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.setCursor(0, 0);     // Start at top-left corner
//    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    // Not all the characters will fit on the display. This is normal.
    // Library will draw what it can and the rest will be clipped.
    for (int16_t i = 0; i < 128; i++) {
        if (i == '\n') display.write(' ');
        else display.write(i);
    }

    display.display();
    delay(2000);
}

void testdrawstyles(void) {
    display.clearScreen();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);             // Start at top-left corner
    display.println(F("Hello, world!"));

    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
    display.println(3.141592);

    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.print(F("0x"));
    display.println(0xDEADBEEF, HEX);

    display.display();
    delay(2000);
}

/*
void testscrolltext(void) {
    display.clearDisplay();

    display.setTextSize(2); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("scroll"));
    display.display();      // Show initial text
    delay(100);

    // Scroll in various directions, pausing in-between:
    display.startscrollright(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrolldiagright(0x00, 0x07);
    delay(2000);
    display.startscrolldiagleft(0x00, 0x07);
    delay(2000);
    display.stopscroll();
    delay(1000);
}
*/

/*
void testdrawbitmap(void) {
    display.clearDisplay();

    display.drawBitmap(
            (DISPLAY_XSIZE - LOGO_WIDTH) / 2,
            (DISPLAY_YSIZE - LOGO_HEIGHT) / 2,
            logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    delay(1000);
}
*/

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
    int8_t f, icons[NUMFLAKES][3];

    // Initialize 'snowflake' positions
    for (f = 0; f < NUMFLAKES; f++) {
        icons[f][XPOS] = random(1 - LOGO_WIDTH, DISPLAY_XSIZE);
        icons[f][YPOS] = -LOGO_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
        Serial.print(F("x: "));
        Serial.print(icons[f][XPOS], DEC);
        Serial.print(F(" y: "));
        Serial.print(icons[f][YPOS], DEC);
        Serial.print(F(" dy: "));
        Serial.println(icons[f][DELTAY], DEC);
    }

    for (;;) { // Loop forever...
        display.clearScreen(); // Clear the display buffer

        // Draw each snowflake:
        for (f = 0; f < NUMFLAKES; f++) {
            display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
        }

        display.display(); // Show the display buffer on the screen
        delay(200);        // Pause for 1/10 second

        // Then update coordinates of each flake...
        for (f = 0; f < NUMFLAKES; f++) {
            icons[f][YPOS] += icons[f][DELTAY];
            // If snowflake is off the bottom of the screen...
            if (icons[f][YPOS] >= DISPLAY_YSIZE) {
                // Reinitialize to a random position, just off the top
                icons[f][XPOS] = random(1 - LOGO_WIDTH, DISPLAY_XSIZE);
                icons[f][YPOS] = -LOGO_HEIGHT;
                icons[f][DELTAY] = random(1, 6);
            }
        }
    }
}

void setup() {
#ifdef SERIAL_DEBUG
    Serial.begin(57600);
#endif

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }

/*
    // Clear the buffer
    display.clearScreen();

    // Draw a single pixel in white
    display.drawPixel(10, 10, SSD1306_WHITE);

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();
    delay(2000);
*/
    // display.display() is NOT necessary after every single drawing command,
    // unless that's what you want...rather, you can batch up a bunch of
    // drawing operations and then update the screen all at once by calling
    // display.display(). These examples demonstrate both approaches...

/*
    testdrawline();      // Draw many lines

    testdrawrect();      // Draw rectangles (outlines)

    testfillrect();      // Draw rectangles (filled)

    testdrawcircle();    // Draw circles (outlines)

    testfillcircle();    // Draw circles (filled)

    testdrawroundrect(); // Draw rounded rectangles (outlines)

    testfillroundrect(); // Draw rounded rectangles (filled)
*/

/*
    testdrawtriangle();  // Draw triangles (outlines)

    testfilltriangle();  // Draw triangles (filled)
*/

    testdrawchar();      // Draw characters of the default font

    testdrawstyles();    // Draw 'stylized' characters

/*
    testscrolltext();    // Draw scrolling text

    testdrawbitmap();    // Draw a small bitmap image
*/
/*

    // Invert and restore display, pausing in-between
    display.setInverted();
    delay(1000);
    display.clearInverted();
    delay(1000);

    testanimate(logo_bmp, LOGO_WIDTH, LOGO_HEIGHT); // Animate bitmaps
*/
}

void loop() {
}

