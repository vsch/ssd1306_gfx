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
#include "tft18.h"

// ---------------------------------------------------------------------------
// MISC ROUTINES
// put into a routine to remove code inlining at cost of timing accuracy
void msDelay(int delay) {
    for (int i = 0; i < delay; i++) _delay_ms(1);
}

// flash the on-board LED at ~ 3 Hz
void FlashLED(byte count) {
    for (; count > 0; count--) {
        setBit(PORTB, LED);      // turn LED on
        msDelay(150);           // wait
        clearBit(PORTB, LED);    // turn LED off
        msDelay(150);           // wait
    }
}

#define SPCR_NORMAL     0x50
#define CS_DELAY        2.0

Tft18 tft = Tft18();
int interTestDelay = 500;

// --------------------------------------------------------------------------- // TEST ROUTINES
// draws 4000 pixels on the screen
void PixelTest(byte alpha) {
    tft.clearScreen();
    for (int i = 4000; i > 0; i--) // do a whole bunch:
    {
        int x = rand() % tft.maxX; // random x coordinate
        int y = rand() % tft.maxY; // random y coordinate
        tft.setPixel(x, y, YELLOW, alpha); // draw pixel at x,y
    }
    msDelay(interTestDelay);
}

// sweeps Line routine through all four quadrants.
void LineTest(color_t color, byte alpha) {
//    tft.clearScreen();
    color_t background = tft.background;
    tft.background = color;
    tft.alpha = alpha;
    int x, y, x0 = tft.maxX / 2, y0 = tft.maxY / 2;
    for (x = 0; x < tft.maxX - 1; x += 2) tft.line(x0, y0, x, 0, YELLOW);
    for (y = 0; y < tft.maxY - 1; y += 2) tft.line(x0, y0, tft.maxX - 1, y, CYAN);
    for (x = tft.maxX - 1; x > 0; x -= 2) tft.line(x0, y0, x, tft.maxY - 1, YELLOW);
    for (y = tft.maxY - 1; y > 0; y -= 2) tft.line(x0, y0, 0, y, CYAN);
    tft.background = background;
    msDelay(interTestDelay);
}

// draw series of concentric circles
void CircleTest(int color, byte alpha) {
    tft.clearScreen();
    tft.alpha = alpha;

    for (int radius = 6; radius < 60; radius += 2)
        tft.circle(tft.maxX / 2, tft.maxY / 2, radius, YELLOW);
    msDelay(interTestDelay);
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

    tft.background = RGB565(0, 0, 0);
    tft.alpha = 128;
    tft.fillCircle(cx, cy, rad * 3 / 2, RGB565(255, 255, 255));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx, cy, rad, RGB565(128, 128, 128));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx, cy, rad / 2, RGB565(0, 0, 0));
    msDelay(interTestDelay / 4);

    tft.alpha = 128;
    tft.fillCircle(cx1, cy1, rad, RGB565(255, 0, 0));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx2, cy2, rad, RGB565(0, 255, 0));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx3, cy3, rad, RGB565(0, 0, 255));
    msDelay(interTestDelay / 4);

    tft.alpha = 196;
    tft.fillCircle(cx1, cy1, rad / 2, RGB565(255, 0, 0));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx2, cy2, rad / 2, RGB565(0, 255, 0));
    msDelay(interTestDelay / 4);
    tft.fillCircle(cx3, cy3, rad / 2, RGB565(0, 0, 255));
    msDelay(interTestDelay / 4);
}

// Fills screen with characters
void FillChars() {
    tft.clearScreen();
    int chars = tft.maxCol * tft.maxRow;
    int xMargin = (tft.maxX - tft.maxCol * TFT_CHAR_WIDTH) / 2;
    int yMargin = (tft.maxY - tft.maxRow * TFT_CHAR_HEIGHT) / 2;

    for (int i = 0; i < chars; i++) {
        int x = i % tft.maxCol;
        int y = i / tft.maxCol;
        char ascii = static_cast<char>((i % 96) + 32);
        tft.putCh(ascii, x * TFT_CHAR_WIDTH + xMargin, y * TFT_CHAR_HEIGHT + yMargin);
    }
    msDelay(interTestDelay);
}

uint8_t red = 31, green = 0, blue = 0, state = 0;

void RainbowFill() {
    uint16_t color = RGB565(red, green, blue);
    tft.alpha = 255;

    for (int i = 0; i < tft.maxY; i++) {
        tft.hLine(0, tft.maxX, i, color);
        // test gama
//        setGama(1);
//        HLine(0, 32, i, color, 255);
//        setGama(2);
//        HLine(32, 64, i, color, 255);
//        setGama(4);
//        HLine(64, 96, i, color, 255);
//        setGama(8);
//        HLine(96, 128, i, color, 255);

        switch (state) {
            default:
            case 0:
                green += 2;
                if (green == 64) {
                    green = 63;
                    state = 1;
                }
                break;
            case 1:
                red -= 2;
                if (red >= 254) {
                    red = 0;
                    state = 2;
                }
                break;
            case 2:
                blue += 1;
                if (blue == 32) {
                    blue = 31;
                    state = 3;
                }
                break;
            case 3:
                green -= 2;
                if (green >= 254) {
                    green = 0;
                    state = 4;
                }
                break;
            case 4:
                red += 1;
                if (red == 32) {
                    red = 31;
                    state = 5;
                }
                break;
            case 5:
                blue -= 1;
                if (blue >= 254) {
                    blue = 0;
                    state = 0;
                }
                break;
        }

        color = red << 11 | green << 5 | blue;
    }
    tft.setGama(1);
}

#define SHOW_TIMING

#ifdef SHOW_TIMING
uint32_t timer = 0;

uint32_t time() {
    cli();
    uint32_t val = timer;
    sei();
    return val;
}

ISR(TIMER0_COMPA_vect) {//timer0 interrupt 2kHz toggles pin 8
//generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
        timer++;
}

#endif

void SetupPorts() {
    DDRB = PORTB_OUT; // 0010.1111; set B1, B2-B3, B5 as outputs
    DDRC = 0x00; // 0000.0000; set PORTC as inputs
    DDRD = 0x70; // 0000.0000; set PORTD 5,6 as output
    PORTD = 0;
    setBit(TFT_RST_PORT, TFT_RST_BIT); // start with TFT reset line inactive high
    setBit(TFT_CS_PORT, TFT_CS_BIT);  // deselect TFT CS
    clearBit(TFT_SCK_PORT, TFT_SCK_BIT);  // TFT SCK Low
}

// --------------------------------------------------------------------------- // MAIN PROGRAM
int main() {
#ifdef SHOW_TIMING
    cli();

    //set timer0 interrupt at 1kHz at 8MHz or 2kHz at 16MHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0 = 0;//initialize counter value to 0

    // set compare match register for 1khz increments
    OCR0A = 62;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    sei();
#endif

    SetupPorts();                               // use PortB for LCD interface
    FlashLED(1);                                // indicate program start
    tft.openSPI();                                  // start communication to TFT
    tft.initDisplay();                              // initialize TFT controller
    long id, id2, id3;

    for (int rot = TFT_ROT_0; rot <= TFT_ROT_270 * 4; rot++) {
        tft.setOrientation(rot % (TFT_ROT_270 + 1));

        tft.foreground = CYAN;
        FillChars();                            // show full screen of ASCII chars
        tft.clearScreen();
    }

    for (int rot = TFT_ROT_0; rot <= TFT_ROT_270; rot++) {
        tft.setOrientation(rot);

        tft.foreground = CYAN;
        tft.clearScreen();
/*
    CircleTest(0, 255);
    CircleTest(0, 128);
    CircleTest(RGB565(255, 0, 0), 128);
    CircleTest(RGB565(0, 255, 0), 128);
    CircleTest(RGB565(0, 0, 255), 128);

//*/
        LineTest(BLACK, 255);
        LineTest(BLACK, 128);
        LineTest(RGB565(255, 0, 0), 128);
        LineTest(RGB565(0, 255, 0), 128);
        LineTest(RGB565(0, 0, 255), 128);
//*/

//*
        tft.clearScreen();
        ColorCircleTest();

//*/

//*
        tft.alpha = 255;
        tft.foreground = YELLOW;
        tft.background = BLACK;
        tft.fillEllipse(60, 75, 100, 50, BLACK);   // erase an oval in center
        tft.ellipse(60, 75, 100, 50, LIME);        // outline the oval in green

        const char *str = "Hello, World!";            // text to display
        tft.gotoCharXY(4, 9);                         // position text cursor
        tft.foreground = YELLOW;
        tft.background = BLACK;
        tft.write(str);                       // display text inside oval
        // tft.writeHex((int)(id >> 16), YELLOW);
        // tft.write(' ', YELLOW);
        // tft.writeHex((int)id, YELLOW);
        msDelay(interTestDelay);

        id = 0xF0E1D2C3; //ReadID();

        tft.alpha = 255;
        tft.fillEllipse(60, 75, 100, 50, BLACK);   // erase an oval in center
        tft.ellipse(60, 75, 100, 50, LIME);        // outline the oval in green

        tft.gotoCharXY(5, 9);                                 // position text cursor
        tft.foreground = YELLOW;
        tft.background = BLACK;
        tft.writeHex((int)(id >> 16));
        tft.write(' ');
        tft.writeHex((int)id);
        msDelay(interTestDelay);
//*/
    }

    tft.foreground = RGB565(255, 255, 128);
    tft.background = RGB565(32, 32, 160);
    tft.alpha = 255;
    tft.setOrientation(TFT_ROT_180);

    int totalLines = 2;
    int totalColumns = 15;
    int col0 = (tft.maxX / TFT_CHAR_WIDTH - totalColumns) / 2;
    int line0 = (tft.maxY / TFT_CHAR_HEIGHT - totalLines) / 2;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (uint8_t iter = 0;; iter++) {
#ifdef SHOW_TIMING
        uint32_t start = time();
#endif
        RainbowFill();

#ifdef SHOW_TIMING
    tft.fillRect(col0 * TFT_CHAR_WIDTH - 1, line0 * TFT_CHAR_HEIGHT - 1,
                 (col0 + totalColumns) * TFT_CHAR_WIDTH, (line0 + totalLines) * TFT_CHAR_HEIGHT, tft.background);
#else
    tft.fillRect(col * TFT_CHAR_WIDTH - 1, line * TFT_CHAR_HEIGHT - 1, (col + 15) * TFT_CHAR_WIDTH, (line + 12) * TFT_CHAR_HEIGHT, tft.background);
#endif

        int line = line0;
        int col = col0;

#ifdef SHOW_TIMING
        tft.gotoCharXY(col, line++);                                 // position text cursor
        uint32_t end = time();
        tft.write(" TIME ");
        tft.write(end - start);
#endif

        tft.gotoCharXY(col, line++);                                 // position text cursor
        tft.write("---------------");

//        tft.sendCmd((iter & 1) ? IDMON : IDMOFF);   // take display out of sleep mode
        msDelay(1000);
//        tft.sendCmd(IDMOFF);   // take display out of sleep mode
    }
#pragma clang diagnostic pop

    tft.closeSPI();                                 // close communication with TFT
    FlashLED(3);                                // indicate program end
}
