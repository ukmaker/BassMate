#ifndef SIMPLEGUI_ADAFRUIT_FONTRENDERER_H
#define SIMPLEGUI_ADAFRUIT_FONTRENDERER_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif
#include "Core/Align.h"
#include "FontRenderer.h"
#include "gfxfont.h"
#include "Hardware/DMA.h"

namespace simplegui {

class AdafruitFontRenderer : public FontRenderer {
public:
    AdafruitFontRenderer(const GFXfont* font, Adafruit_GFX* gfx, int16_t w, int16_t h);
    ~AdafruitFontRenderer() { }

    /**********************************************************************/
    /*!
      @brief   Set a clipping window for text
      @note    Affects calls to write() only, and hence also to print()
               Calls to drawChar() are not affected
    */
    /**********************************************************************/
    void setTextWindow(int16_t x, int16_t y, int16_t w, int16_t h);
    void setVAlign(VAlign align);
    void setHAlign(HAlign align);
    void writeAligned(const char* string);
    void removeTextWindow();
    uint8_t getCharWidth(const char c);
    bool isSpace(const char c);
    bool isNewline(const char c);
    bool isEnd(const char c);
    bool fitLine(const char** p, int* w);
    /**
     * Return true if some characters were consumed. False means we're at the end of the string
     * At exit, start points to the start of the word, end points to the first character
     * of the next word - i.e. skips over any trailing whitespace
     * wordWidth is the width of non-whitespace
     * spaceWidth is the width of any trailing whitespace
     **/
    bool getWordWidth(const char** start, const char** end, int* wordWidth, int* spaceWidth);

    virtual void getTextBounds(const char* string, int16_t x, int16_t y,
        int16_t* x1, int16_t* y1, uint16_t* w,
        uint16_t* h);
    virtual void getTextBounds(const __FlashStringHelper* s, int16_t x, int16_t y,
        int16_t* x1, int16_t* y1, uint16_t* w,
        uint16_t* h);

    using Print::write;
    virtual size_t write(uint8_t c);

    virtual void drawChar(int16_t x, int16_t y,
        unsigned char c, uint16_t color, uint16_t bg,
        uint8_t size_x, uint8_t size_y);

    virtual void setFont(const GFXfont* f);
    virtual uint16_t getFontHeight();
    virtual void charBounds(unsigned char c, int16_t* x, int16_t* y,
        int16_t* minx, int16_t* miny, int16_t* maxx,
        int16_t* maxy);

    void setDMA(DMA* stmdma);

protected:
    bool textWindowed; ///< Should text windowing be applied
    int16_t textX; ///< Left side of text window
    int16_t textW; ///< Width of text window
    int16_t textY; ///< Top of text window
    int16_t textH; ///< Height of text window
    int8_t ymin, ymax; // Max and min y-offsets from zero for any character in the current font
    DMA* _stmdma;
    VAlign _valign;
    HAlign _halign;
    bool _prepared;
};
}
#endif // _ADAFRUIT_DefaultFontRenderer_NG_H