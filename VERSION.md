# Version History

[TOC]: #

- [1.30](#130)
- [1.28](#128)
- [1.26](#126)
- [1.24](#124)
- [1.22](#122)
- [1.20](#120)
- [1.18](#118)
- [1.16](#116)
- [1.14](#114)
- [1.13](#113)
- [1.12](#112)
- [1.6](#16)
- [1.5](#15)
- [1.4](#14)
- [1.0](#10)
- [0.1](#01)
- [0.2](#02)


## 1.30

* Add: `printScreen()` method to interfaceManager to dump display as
  ASCII to a stream.

## 1.28

* Add: bit map optimization for paged updates

## 1.26

* Fix: paged updates to be easier to implement on the interface side

## 1.24

* Add: `GFX_PAGED_UPDATES` definition to reduce amount of RAM used for
  the display buffer. This will cause the library to use a buffer as
  small as one page (128 bytes, vs. 128x32 taking 512 bytes).
  `GFX_UPDATE_PAGES` defines number of pages to use for the buffer.
  Defaults to 1. Adds 228 bytes to FLASH memory use.

  To make use of this need to change the way updates are done. They must
  now be done as follows:

  ```c
  gfx_clear_screen();
  for (; gfx_update_page_y1 <= DISPLAY_YSIZE; gfx_start_next_page()) { 
     your_display_update();

     gfx_display();
  }
  ```

  The display will be redrawn for each page, then that page will be sent
  to the display. It will take about two times longer to update the
  display with one-page buffer and 50% longer with a two-page buffer,
  over the time it takes with a full four-page buffer.

* Fix: problem with using TWI interrupt version for initialization. The
  buffer was being overwritten before it was sent. Reverted files back
  to ATmega328p library originals and now including blocking or
  interrupt driven versions depending on `INCLUDE_TWI_INT` macro.

* Fix: found that initializing a single non-zero byte of a multi-byte
  structure causes an image for the full structure to be allocated in
  flash, to be used during C initialization. The display buffer was
  causing 512 bytes of FLASH to be wasted.

## 1.22

* Add: `GFX_NO_LINE_PATTERNS` to remove any line pattern related code
  and save 420 bytes of flash memory.
* Fix: change large font `2`.

## 1.20

* Add: `void gfx_print_display(FILE *stream)` to dump screen as ASCII to
  given stream.

## 1.18

* Add: Progress bar drawing `gfx_progress_bar_to(...)`

* Fix: reduced character rendering by more than x2 by implementing bit
  blit function to copy character data in bytes where possible. Faster
  char processing if starting on byte boundary for y coordinate
  (0,8,16,24) when one column can be copied in one operation.

  This also sped up display of characters with background color not set
  to NONE. Previously it doubled the rendering time. Now takes about 60%
  more.

## 1.16

* rename `ssd1306_` prefix to `gfx_` prefix

* remove `gfx_flush_wrap_chars()` and replace with
  `gfx_start_text_spc_wrap()` and `gfx_end_text_spc_wrap()` to delimit
  output that is wrapped. All else is may be wrapped only by right
  margin.

## 1.14

* Fix: reduced display update by x2, TWI interrupt based update takes 16
  uSec vs 13.5 ms for blocking.

* Fix: reduced char output for display update by optimizing and removing
  unnecessary use of black background when printing screen data, already
  on a black background.

* Add: contrast config option to modify display brightness/contrast.

* Add: `fieldChanged()` callback to `FieldUpdater` to `FieldEditor`
  ability to update external values on value change in the popup (i.e.
  contrast).

## 1.13

* Fix: optimizations, use interrupt driven TWI interface from
  [ATmega328p](https://github.com/goessl/ATmega328P) library.

## 1.12

* Fix: gfx circle and round rect

## 1.6

* Add: small font to allow printing inside single height boxes. ie.
  battery with % charge in it. The battery is assumed to fit into one
  char height and made up of two or three characters.

* Add: proper on white space text wrapping to margins. Handles solid
  runs of non-break text by wrapping where ever to not exceed the right
  margin.

* Add: text bounds now tracks both min/max x/y coordinates where text is
  output. Including cursor moves and text size changes.


## 1.5

* Fixed code to use the more efficient gfx calls and functionality

## 1.4

* Change gfx to self rolled. Had 30668 bytes of FLASH used, now only
  23426\.

* rewrite implementation by directly controlling TWI hardware without
  the Arduino I2C library code to reduce implementation to bare minimum.
  This project only has a single slave (OLED display) and only master to
  slave communication at the maximum allowable speed. No need for
  transactions or switching TWI clock speed.

* rewrite SSD1306 implementation to reduce code size for this
  implementation.

## 1.0

Initial implementation


## 0.1

* Initial somewhat working graphics

## 0.2

* Fix: circle quadrants to not overlap when drawing dashed or inverted
  outline.

* Add: octant flags to rect() to draw selected quadrants (top, right,
  bottom, right) and roundRect() to draw selected octants (top,
  top-right corner, right, bottom-right corner, bottom, bottom-left
  corner, left, top-left corner).


