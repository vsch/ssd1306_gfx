//---------------------------------------------------------------------------
// GLOBAL DEFINES
#define F_CPU       (16000000L/2)       // run CPU at 8 MHz or 16 MHz
#define LED         5                   // Boarduino LED on PB5

#define PORTB_OUT 0x2E

// ---------------------------------------------------------------------------
// INCLUDES
#include <Arduino.h>
#include <avr/io.h>         // deal with port registers
#include <avr/interrupt.h>  // deal with interrupt calls
#include <util/delay.h>     // used for _delay_ms function
#include <string.h>         // string manipulation routines
#include <avr/sleep.h>      // used for sleep functions
#include <stdlib.h>
#include "ssd1306.h"

#define DISPLAY_TYPE SSD1306_TYPE_OLED_096 /*| SSD1306_INVERTED*/ /*| SSD1306_EXTERNALVCC*/

#include "ssd1306_display.h"

// ---------------------------------------------------------------------------
// MISC ROUTINES
// put into a routine to remove code inlining at cost of timing accuracy
void msDelay(int delay) {
    for (int i = 0; i < delay; i++) _delay_ms(1);
}

// flash the on-board LED at ~ 3 Hz
void FlashLED(uint8_t count) {
    for (; count > 0; count--) {
        setBit(PORTB, LED);      // turn LED on
        msDelay(150);           // wait
        clearBit(PORTB, LED);    // turn LED off
        msDelay(150);           // wait
    }
}

#define SPCR_NORMAL     0x50

uint8_t pageBuffer[DISPLAY_XSIZE];
Ssd1306 tft = Ssd1306(pageBuffer, DISPLAY_TYPE);
int interTestDelay = 2500;

// --------------------------------------------------------------------------- // TEST ROUTINES
// draws 4000 pixels on the screen
void PixelTest(uint8_t alpha) {
    for (int i = 4000; i > 0; i--) // do a whole bunch:
    {
        int x = rand() % tft.maxX; // random x coordinate
        int y = rand() % tft.maxY; // random y coordinate
        tft.setPixel(x, y, WHITE); // draw pixel at x,y
    }
}

// sweeps Line routine through all four quadrants.
void LineTest3() {
    int x, y, x0 = tft.maxX / 2, y0 = tft.maxY / 2;
    for (x = 0; x < tft.maxX - 1; x += 2) tft.line(x0, y0, x, 0, tft.foreground);
    for (y = 0; y < tft.maxY - 1; y += 2) tft.line(x0, y0, tft.maxX - 1, y, tft.foreground);
    for (x = tft.maxX - 1; x > 0; x -= 2) tft.line(x0, y0, x, tft.maxY - 1, tft.foreground);
    for (y = tft.maxY - 1; y > 0; y -= 2) tft.line(x0, y0, 0, y, tft.foreground);
}

void LineTest1() {
    int x, y, x0 = tft.maxX / 2, y0 = tft.maxY / 2;
//    for (x = 0; x < tft.maxX - 1; x += 2) tft.line(x0, y0, x, 0, tft.foreground);
    for (y = 0; y < tft.maxY - 1; y += 2) tft.line(x0, y0, tft.maxX - 1, y, tft.foreground);
//    for (x = tft.maxX - 1; x > 0; x -= 2) tft.line(x0, y0, x, tft.maxY - 1, tft.foreground);
    for (y = tft.maxY - 1; y > 0; y -= 2) tft.line(x0, y0, 0, y, tft.foreground);
}

void LineTest2() {
    int x, y, x0 = tft.maxX / 2, y0 = tft.maxY / 2;
    for (x = 0; x < tft.maxX - 1; x += 2) tft.line(x0, y0, x, 0, tft.foreground);
//    for (y = 0; y < tft.maxY - 1; y += 2) tft.line(x0, y0, tft.maxX - 1, y, tft.foreground);
    for (x = tft.maxX - 1; x > 0; x -= 2) tft.line(x0, y0, x, tft.maxY - 1, tft.foreground);
//    for (y = tft.maxY - 1; y > 0; y -= 2) tft.line(x0, y0, 0, y, tft.foreground);
}

// draw series of concentric circles
void CircleTest(int color, uint8_t alpha) {
    for (int radius = 6; radius < 60; radius += 2)
        tft.circle(tft.maxX / 2, tft.maxY / 2, radius, WHITE);
}

#define rad 40

void ColorCircleTest() {
    int cx = tft.maxX / 2;
    int cy = tft.maxY / 2;

    int cx1 = cx;
    int cx2 = cx - rad / 2;
    int cx3 = cx + rad / 2;
    int cy1 = cy - (433 * rad) / 750; // sqrt(3)/3 * rad
    int cy2 = cy + (433 * rad) / 1500; // sqrt(3)/6 * rad
    int cy3 = cy2;

    tft.fillCircle(cx, cy, rad * 3 / 2, RGB(255, 255, 255));
    tft.fillCircle(cx, cy, rad, RGB(128, 128, 128));
    tft.fillCircle(cx, cy, rad / 2, RGB(0, 0, 0));
}

// Fills screen with characters
void FillChars() {
    Serial.print("Fill chars maxCols: ");
    Serial.print(tft.maxCols);
    Serial.print(" maxRows: ");
    Serial.println(tft.maxRows);

    int chars = tft.maxCols * tft.maxRows;
    int xMargin = (tft.maxX - tft.maxCols * CHAR_WIDTH) / 2;
    int yMargin = (tft.maxY - tft.maxRows * CHAR_HEIGHT) / 2;

    for (int i = 0; i < chars; i++) {
        int x = i % tft.maxCols;
        int y = i / tft.maxCols;
        char ascii = static_cast<char>((i % 96) + 32);
        tft.putCh(ascii, x * CHAR_WIDTH + xMargin, y * CHAR_HEIGHT + yMargin);
    }
}

#define SHOW_TIMING

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
    while (Serial.available()) {
        // drain port
        Serial.read();
    }
}

void SetupPorts() {
    DDRB = PORTB_OUT; // 0010.1110; set B1, B2-B3, B5 as outputs
    DDRC = 0x00; // 0000.0000; set PORTC as inputs
    DDRD = 0x70; // 0111.0000; set PORTD 4,5,6 as output
    PORTD = 0;
    setBit(SSD1306_RST_PORT, SSD1306_RST_BIT); // start with TFT reset line inactive high
    setBit(SSD1306_CS_PORT, SSD1306_CS_BIT);  // deselect TFT CS
    clearBit(SSD1306_SCK_PORT, SSD1306_SCK_BIT);  // TFT SCK Low
}

// --------------------------------------------------------------------------- // MAIN PROGRAM
void setup() {
    Serial.begin(57600);

    SetupPorts();                               // use PortB for LCD interface
    FlashLED(1);                                // indicate program start

    Serial.println("Setup ports");
    tft.openSPI();                              // start communication to TFT
    Serial.println("Started spi");
    tft.initDisplay(SSD1306_ROT_0);                              // initialize TFT controller
    Serial.println("Initialized display");
}

void loop() {
    long id, id2, id3;

//*
    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            tft.foreground = WHITE;
            tft.background = BLACK;
        } else {
            tft.foreground = BLACK;
            tft.background = WHITE;
        }

        for (int rot = SSD1306_ROT_0; rot < (SSD1306_ROT_270 + 1); rot++) {
            tft.clearScreen();
            tft.setOrientation(static_cast<uint8_t>(rot % (SSD1306_ROT_270 + 1)));

            tft.startUpdate();
            while (tft.nextPage()) {
                FillChars();                            // show full screen of ASCII chars
            }
            msDelay(interTestDelay);
        }
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            tft.foreground = WHITE;
            tft.background = BLACK;
        } else {
            tft.foreground = BLACK;
            tft.background = WHITE;
        }

        for (int rot = SSD1306_ROT_0; rot < (SSD1306_ROT_270 + 1); rot++) {
            tft.clearScreen();
            tft.setOrientation(static_cast<uint8_t>(rot % (SSD1306_ROT_270 + 1)));

            tft.startUpdate();
            while (tft.nextPage()) {
                int x, y, x0 = tft.maxX / 2, y0 = tft.maxY / 2;
                tft.fillRect(0, 0, x0 - 1, y0 - 1, tft.foreground);
                tft.fillRect(x0, y0, tft.maxX - 1, tft.maxY - 1, tft.foreground);
            }
            msDelay(interTestDelay);
        }
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            tft.foreground = WHITE;
            tft.background = BLACK;
        } else {
            tft.foreground = BLACK;
            tft.background = WHITE;
        }

        for (int rot = SSD1306_ROT_0; rot < (SSD1306_ROT_270 + 1); rot++) {
            tft.clearScreen();
            tft.setOrientation(static_cast<uint8_t>(rot % (SSD1306_ROT_270 + 1)));

            tft.startUpdate();
            while (tft.nextPage()) {
                LineTest1();
            }
            msDelay(interTestDelay);
        }
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            tft.foreground = WHITE;
            tft.background = BLACK;
        } else {
            tft.foreground = BLACK;
            tft.background = WHITE;
        }

        for (int rot = SSD1306_ROT_0; rot < (SSD1306_ROT_270 + 1); rot++) {
            tft.clearScreen();
            tft.setOrientation(static_cast<uint8_t>(rot % (SSD1306_ROT_270 + 1)));

            tft.startUpdate();
            while (tft.nextPage()) {
                LineTest2();
            }
            msDelay(interTestDelay);
        }
    }

//*
    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            tft.foreground = WHITE;
            tft.background = BLACK;
        } else {
            tft.foreground = BLACK;
            tft.background = WHITE;
        }

        for (int rot = SSD1306_ROT_0; rot < SSD1306_ROT_270 + 1; rot++) {
            tft.setOrientation(static_cast<uint8_t>(rot));

            tft.clearScreen();
            tft.foreground = WHITE;

            tft.startUpdate();
            while (tft.nextPage()) {
                ColorCircleTest();
            }
            msDelay(interTestDelay / 4);

            tft.foreground = WHITE;
            tft.background = BLACK;

            tft.startUpdate();
            while (tft.nextPage()) {
                tft.fillEllipse(tft.maxX / 2, tft.maxY / 2, 100, 50, BLACK);   // erase an oval in center
                tft.ellipse(tft.maxX / 2, tft.maxY / 2, 100, 50, WHITE);        // outline the oval in green

                const char *str = "Hello, World!";            // text to display
                tft.gotoCharXY(4, 3);                         // position text cursor
                tft.charOffset(3, 5);
                tft.foreground = WHITE;
                tft.background = BLACK;
                tft.write(str);                       // display text inside oval
            }

            msDelay(interTestDelay);

            id = 0xF0E1D2C3; //ReadID();

            tft.startUpdate();
            while (tft.nextPage()) {
                tft.fillEllipse(tft.maxX / 2, tft.maxY / 2, 100, 50, BLACK);   // erase an oval in center
                tft.ellipse(tft.maxX / 2, tft.maxY / 2, 100, 50, WHITE);        // outline the oval in green

                tft.gotoCharXY(6, 3);                                 // position text cursor
                tft.charOffset(3, 5);
                tft.foreground = WHITE;
                tft.background = BLACK;
                tft.writeHex((int) (id >> 16));
                tft.write(' ');
                tft.writeHex((int) id);
            }
            msDelay(interTestDelay);
        }
    }
//*/

    tft.setOrientation(SSD1306_ROT_0);
    tft.foreground = WHITE;
    tft.background = BLACK;

    int totalLines = 2;
    int totalColumns = 15;
    int col0 = (tft.maxCols - totalColumns) / 2;
    int line0 = (tft.maxRows - totalLines) / 2;

    Serial.print("maxCols: ");
    Serial.print(tft.maxCols);
    Serial.print("maxRows: ");
    Serial.println(tft.maxRows);

//*
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifdef SHOW_TIMING
    unsigned long end = micros();
    unsigned long prevStart = end;
    unsigned long start = end;
#endif
    for (uint8_t iter = 0;; iter++) {
#ifdef SHOW_TIMING
        start = prevStart;
        prevStart = end;
#endif

        tft.startUpdate();
        while (tft.nextPage()) {
#ifdef SHOW_TIMING
            tft.fillRect(col0 * SSD1306_CHAR_WIDTH - 1, line0 * SSD1306_CHAR_HEIGHT - 1,
                         (col0 + totalColumns) * SSD1306_CHAR_WIDTH, (line0 + totalLines) * SSD1306_CHAR_HEIGHT,
                         tft.background);
#else
            tft.fillRect(col * SSD1306_CHAR_WIDTH - 1, line * SSD1306_CHAR_HEIGHT - 1, (col + 15) * SSD1306_CHAR_WIDTH, (line + 12) * SSD1306_CHAR_HEIGHT, st7735.background);
#endif

            int line = line0;
            int col = col0;

#ifdef SHOW_TIMING
            tft.gotoCharXY(col, line++);                                 // position text cursor
            tft.write(" TIME ");
            tft.write(end - start);
#endif

            tft.gotoCharXY(col, line++);                                 // position text cursor
            tft.write("---------------");
        }

        end = micros();

//        msDelay(1000);
    }
#pragma clang diagnostic pop
//*/
}
