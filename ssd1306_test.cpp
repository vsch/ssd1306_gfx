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
#include "ssd1306_gfx.h"

#define DISPLAY_TYPE SSD1306_TYPE_OLED_091 /*| SSD1306_INVERTED*/ /*| SSD1306_EXTERNALVCC*/

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
int interTestDelay = 2500;

// --------------------------------------------------------------------------- // TEST ROUTINES
// draws 250 pixels on the screen
void PixelTest(uint8_t alpha) {
    for (int i = 250; i > 0; i--) // do a whole bunch:
    {
        int x = rand() % DISPLAY_XSIZE; // random x coordinate
        int y = rand() % DISPLAY_YSIZE; // random y coordinate
        ssd1306_setPixel(x, y, SSD1306_COLOR_WHITE); // draw pixel at x,y
    }
}

// sweeps Line routine through all four quadrants.
void LineTest3() {
    int x, y, x0 = DISPLAY_XSIZE / 2, y0 = DISPLAY_YSIZE / 2;
    ssd1306_foreColor = SSD1306_COLOR_WHITE;

    for (x = 0; x < DISPLAY_XSIZE - 1; x += 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(x, 0);
    }
    for (y = 0; y < DISPLAY_YSIZE - 1; y += 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(DISPLAY_XSIZE - 1, y);
    }
    for (x = DISPLAY_XSIZE - 1; x > 0; x -= 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(x, DISPLAY_YSIZE - 1);
    }
    for (y = DISPLAY_YSIZE - 1; y > 0; y -= 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(0, y);
    }
}

void LineTest1() {
    int x, y, x0 = DISPLAY_XSIZE / 2, y0 = DISPLAY_YSIZE / 2;

    //for (x = 0; x < DISPLAY_XSIZE - 1; x += 2) { ssd1306_cX = x0, ssd1306_cY = y0; ssd1306_lineTo( x, 0); }
    for (y = 0; y < DISPLAY_YSIZE - 1; y += 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(DISPLAY_XSIZE - 1, y);
    }
    //for (x = DISPLAY_XSIZE - 1; x > 0; x -= 2) { ssd1306_cX = x0, ssd1306_cY = y0; ssd1306_lineTo( x, DISPLAY_YSIZE - 1); }
    for (y = DISPLAY_YSIZE - 1; y > 0; y -= 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(0, y);
    }
}

void LineTest2() {
    int x, y, x0 = DISPLAY_XSIZE / 2, y0 = DISPLAY_YSIZE / 2;

    for (x = 0; x < DISPLAY_XSIZE - 1; x += 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(x, 0);
    }
    //for (y = 0; y < DISPLAY_YSIZE - 1; y += 2) { ssd1306_cX = x0, ssd1306_cY = y0; ssd1306_lineTo( DISPLAY_XSIZE - 1, y); }
    for (x = DISPLAY_XSIZE - 1; x > 0; x -= 2) {
        ssd1306_cX = x0, ssd1306_cY = y0;
        ssd1306_lineTo(x, DISPLAY_YSIZE - 1);
    }
    //for (y = DISPLAY_YSIZE - 1; y > 0; y -= 2) { ssd1306_cX = x0, ssd1306_cY = y0; ssd1306_lineTo( 0, y); }
}

// draw series of concentric circles
void CircleTest() {

    ssd1306_foreColor = SSD1306_COLOR_WHITE;
    ssd1306_cX = DISPLAY_XSIZE / 2;
    ssd1306_cY = DISPLAY_YSIZE / 2;

    for (int radius = 6; radius < 60; radius += 2) {
        ssd1306_circle(radius);
    }
}

#define rad 40

void ColorCircleTest() {
    int cx = DISPLAY_XSIZE / 2;
    int cy = DISPLAY_YSIZE / 2;

    int cx1 = cx;
    int cx2 = cx - rad / 2;
    int cx3 = cx + rad / 2;
    int cy1 = cy - (433 * rad) / 750; // sqrt(3)/3 * rad
    int cy2 = cy + (433 * rad) / 1500; // sqrt(3)/6 * rad
    int cy3 = cy2;

    ssd1306_cX = cx;
    ssd1306_cY = cy;
    ssd1306_dashBits = SSD1306_BITS_DASH_NONE;
    ssd1306_dashSize = SSD1306_SIZE_DASH_NONE;
    ssd1306_foreColor = SSD1306_COLOR_WHITE;
    ssd1306_gapColor = SSD1306_COLOR_BLACK;
    ssd1306_backColor = SSD1306_COLOR_WHITE;
    ssd1306_circle(rad);

    ssd1306_dashBits = SSD1306_BITS_DASH;
    ssd1306_dashSize = SSD1306_SIZE_DASH;
    ssd1306_foreColor = SSD1306_COLOR_INVERT;
    ssd1306_gapColor = SSD1306_COLOR_NONE;
    ssd1306_backColor = SSD1306_COLOR_INVERT;
    ssd1306_circle(rad * 3 / 2);

    ssd1306_dashBits = SSD1306_BITS_DASH_DOT;
    ssd1306_dashSize = SSD1306_SIZE_DASH_DOT;
    ssd1306_foreColor = SSD1306_COLOR_INVERT;
    ssd1306_gapColor = SSD1306_COLOR_NONE;
    ssd1306_backColor = SSD1306_COLOR_INVERT;
    ssd1306_circle(rad / 2);
}

// Fills screen with characters
void FillChars() {
    uint8_t ssd1306_maxCols = DISPLAY_XSIZE / CHAR_WIDTH;
    uint8_t ssd1306_maxRows = DISPLAY_YSIZE / CHAR_HEIGHT;

    Serial.print("Fill chars maxCols: ");
    Serial.print(ssd1306_maxCols);
    Serial.print(" maxRows: ");
    Serial.println(ssd1306_maxRows);

    int chars = ssd1306_maxCols * ssd1306_maxRows;
    int xMargin = (DISPLAY_XSIZE - ssd1306_maxCols * CHAR_WIDTH) / 2;
    int yMargin = (DISPLAY_YSIZE - ssd1306_maxRows * CHAR_HEIGHT) / 2;

    for (int i = 0; i < chars; i++) {
        int x = i % ssd1306_maxCols;
        int y = i / ssd1306_maxCols;
        char ascii = static_cast<char>((i % 96) + 32);
        ssd1306_cX = x * CHAR_WIDTH + xMargin;
        ssd1306_cY = y * CHAR_HEIGHT + yMargin;
        ssd1306_putCh(ascii);
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
}

// --------------------------------------------------------------------------- // MAIN PROGRAM
void setup() {
    Serial.begin(57600);

    SetupPorts();                               // use PortB for LCD interface
    FlashLED(1);                                // indicate program start

    Serial.println("Setup ports");
    Serial.println("Started spi");
    ssd1306_initDisplay(0);                              // initialize TFT controller
    Serial.println("Initialized display");
}

void loop() {
    long id, id2, id3;

//*
    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            ssd1306_foreColor = SSD1306_COLOR_WHITE;
            ssd1306_backColor = SSD1306_COLOR_BLACK;
        } else {
            ssd1306_foreColor = SSD1306_COLOR_BLACK;
            ssd1306_backColor = SSD1306_COLOR_WHITE;
        }

        ssd1306_clearDisplay();
        FillChars();                            // show full screen of ASCII chars
        msDelay(interTestDelay);
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            ssd1306_foreColor = SSD1306_COLOR_WHITE;
            ssd1306_backColor = SSD1306_COLOR_BLACK;
        } else {
            ssd1306_foreColor = SSD1306_COLOR_BLACK;
            ssd1306_backColor = SSD1306_COLOR_WHITE;
        }

        ssd1306_clearDisplay();
        int x, y, x0 = DISPLAY_XSIZE / 2, y0 = DISPLAY_YSIZE / 2;
        ssd1306_cX = 0;
        ssd1306_cY = 0;
        ssd1306_rect(x0 - 1, y0 - 1);
        ssd1306_cX = x0;
        ssd1306_cY = y0;
        ssd1306_rect(DISPLAY_XSIZE - 1, DISPLAY_YSIZE - 1);
        msDelay(interTestDelay);
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            ssd1306_foreColor = SSD1306_COLOR_WHITE;
            ssd1306_backColor = SSD1306_COLOR_BLACK;
        } else {
            ssd1306_foreColor = SSD1306_COLOR_BLACK;
            ssd1306_backColor = SSD1306_COLOR_WHITE;
        }

        ssd1306_clearDisplay();
        LineTest1();
        msDelay(interTestDelay);
    }

    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            ssd1306_foreColor = SSD1306_COLOR_WHITE;
            ssd1306_backColor = SSD1306_COLOR_BLACK;
        } else {
            ssd1306_foreColor = SSD1306_COLOR_BLACK;
            ssd1306_backColor = SSD1306_COLOR_WHITE;
        }

        ssd1306_clearDisplay();
        LineTest2();
        msDelay(interTestDelay);
    }

//*
    for (int c = 0; c < 2; c++) {
        if (c == 0) {
            ssd1306_foreColor = SSD1306_COLOR_WHITE;
            ssd1306_backColor = SSD1306_COLOR_BLACK;
        } else {
            ssd1306_foreColor = SSD1306_COLOR_BLACK;
            ssd1306_backColor = SSD1306_COLOR_WHITE;
        }

        ssd1306_clearDisplay();
        ssd1306_foreColor = SSD1306_COLOR_WHITE;

        ColorCircleTest();
        msDelay(interTestDelay / 4);

        ssd1306_foreColor = SSD1306_COLOR_WHITE;
        ssd1306_backColor = SSD1306_COLOR_BLACK;

        const char *str = "Hello, World!";            // text to display
        ssd1306_cX = 4 * ssd1306_cSizeX + 3;
        ssd1306_cX = 3 * ssd1306_cSizeY + 5;
        ssd1306_foreColor = SSD1306_COLOR_WHITE;
        ssd1306_backColor = SSD1306_COLOR_BLACK;
        ssd1306_writeText(str);                       // display text inside oval

        msDelay(interTestDelay);

        id = 0xF0E1D2C3; //ReadID();

        msDelay(interTestDelay);
    }
//*/

    ssd1306_foreColor = SSD1306_COLOR_WHITE;
    ssd1306_backColor = SSD1306_COLOR_BLACK;

    int totalLines = 2;
    int totalColumns = 14;
    int ssd1306_maxCols = DISPLAY_XSIZE / CHAR_WIDTH;
    int col0 = (ssd1306_maxCols - totalColumns) / 2;
    int ssd1306_maxRows = DISPLAY_YSIZE / CHAR_HEIGHT;
    int line0 = (ssd1306_maxRows - totalLines) / 2;

    Serial.print("maxCols: ");
    Serial.print(ssd1306_maxCols);
    Serial.print(" maxRows: ");
    Serial.println(ssd1306_maxRows);

//*
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#ifdef SHOW_TIMING
    unsigned long end = micros();
    unsigned long prevStart = end;
    unsigned long start = end;
#endif
    for (
            uint8_t iter = 0;; iter++) {
#ifdef SHOW_TIMING
        start = prevStart;
        prevStart = end;
#endif

#ifdef SHOW_TIMING
        ssd1306_cX = col0 * ssd1306_cSizeX - 1;
        ssd1306_cY = line0 * ssd1306_cSizeY - 1;
        ssd1306_rect((col0 + totalColumns) * SSD1306_CHAR_WIDTH, (line0 + totalLines) * SSD1306_CHAR_HEIGHT);
#else
        ssd1306_fillRect(col * SSD1306_CHAR_WIDTH - 1, line * SSD1306_CHAR_HEIGHT - 1, (col + 15) * SSD1306_CHAR_WIDTH, (line + 12) * SSD1306_CHAR_HEIGHT, st7735.background);
#endif

        int line = line0;
        int col = col0;

#ifdef SHOW_TIMING
        ssd1306_cX = col * ssd1306_cSizeX;
        ssd1306_cY = line++ * ssd1306_cSizeY;                                 // position text cursor
        ssd1306_writeText(" TIME ");
        ssd1306_writeUInt32(end - start);
#endif
        ssd1306_cX = col * ssd1306_cSizeX;
        ssd1306_cY = line++ * ssd1306_cSizeY;                                 // position text cursor
        ssd1306_writeText("--------------");
    }

    end = micros();

//        msDelay(1000);
}

#pragma clang diagnostic pop

