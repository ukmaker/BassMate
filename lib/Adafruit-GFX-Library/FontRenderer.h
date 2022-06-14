#ifndef _ADAFRUIT_FONTRENDERER_H
#define _ADAFRUIT_FONTRENDERER_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include "gfxfont.h"


class Adafruit_GFX_NG;

class FontRenderer {

  public:

  FontRenderer(int16_t w, int16_t h);
  ~FontRenderer() {}

  void setWidth(int16_t w) { _width = w; }
  void setHeight(int16_t h) { _height = h; }

  virtual size_t write(Adafruit_GFX_NG *gfx, uint8_t c)=0;

  virtual void drawChar(Adafruit_GFX_NG *gfx, int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)=0;

  virtual void getTextBounds(const char *string, int16_t x, int16_t y, int16_t *x1,
                     int16_t *y1, uint16_t *w, uint16_t *h);
  virtual void getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y,
                     int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
  virtual void getTextBounds(const String &str, int16_t x, int16_t y, int16_t *x1,
                     int16_t *y1, uint16_t *w, uint16_t *h);
  void setTextSize(uint8_t s);
  void setTextSize(uint8_t sx, uint8_t sy);
  virtual void setFont(const GFXfont *f = NULL)=0;
  const GFXfont *getFont();

  virtual uint16_t getFontHeight()=0;
  void setTextWrap(bool wrap);
  bool getTextWrap();

  void setCursor(int16_t x, int16_t y);
  int16_t getCursorX() const;
  int16_t getCursorY() const;
  void setTextColor(uint16_t c);
  void setTextColor(uint16_t fg, uint16_t bg);
  void cp437(bool);
  virtual void charBounds(unsigned char c, int16_t *x, int16_t *y, int16_t *minx,
                  int16_t *miny, int16_t *maxx, int16_t *maxy)=0;
protected:
  int16_t _width;       ///< Display width as modified by current rotation
  int16_t _height;      ///< Display height as modified by current rotation
  int16_t cursor_x;     ///< x location to start print()ing text
  int16_t cursor_y;     ///< y location to start print()ing text
  uint16_t textcolor;   ///< 16-bit background color for print()
  uint16_t textbgcolor; ///< 16-bit text color for print()
  uint8_t textsize_x;   ///< Desired magnification in X-axis of text to print()
  uint8_t textsize_y;   ///< Desired magnification in Y-axis of text to print()
  uint8_t rotation;     ///< Display rotation (0 thru 3)
  bool wrap;            ///< If set, 'wrap' text at right edge of display
  bool _cp437;          ///< If set, use correct CP437 charset (default is off)
  const GFXfont *gfxFont;     ///< Pointer to special font
};
#endif