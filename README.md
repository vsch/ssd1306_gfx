# Minimalist SSD1306 Display Library

Reduce footprint to under 9k for basic sample and make it compatible
with Adafruit_SSD1306 for graphics API as a fallback.

Really the reduced graphics should be used for better efficiency.

### Versions

#### 0.1

* Initial somewhat working graphics

#### 0.2

* Fix: circle quadrants to not overlap when drawing dashed or inverted
  outline.
* Add: octant flags to rect() to draw selected quadrants (top, right,
  bottom, right) and roundRect() to draw selected octants (top,
  top-right corner, right, bottom-right corner, bottom, bottom-left
  corner, left, top-left corner).


### TODO

* [ ] Add C++ Ssd1306_Display class wrapper with all inline functions
      for mainline API.



