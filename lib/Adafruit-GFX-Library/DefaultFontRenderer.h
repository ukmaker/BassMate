#ifndef _ADAFRUIT_DEFAULTFONTRENDERER_H
#define _ADAFRUIT_DEFAULTFONTRENDERER_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include "FontRenderer.h"

class Adafruit_GFX_NG;

class DefaultFontRenderer : public FontRenderer {

    public:

    DefaultFontRenderer(int16_t w, int16_t h);
    ~DefaultFontRenderer() {}

    virtual size_t write(Adafruit_GFX_NG *gfx, uint8_t c);
    virtual void drawChar(Adafruit_GFX_NG *gfx, int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y);
                            

virtual void setFont(const GFXfont *f);
virtual uint16_t getFontHeight();
virtual void charBounds(unsigned char c, int16_t *x, int16_t *y,
                              int16_t *minx, int16_t *miny, int16_t *maxx,
                              int16_t *maxy);
};

#endif      // _ADAFRUIT_DEFAULTFONTRENDERER_H