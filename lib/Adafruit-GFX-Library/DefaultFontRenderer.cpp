#include "Adafruit_GFX_NG.h"
#include "DefaultFontRenderer.h"
#include "adafruit_pgmspace.h"
#include "glcdfont.c"

    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).  To replace previously-drawn text when using a custom
    // font, use the getTextBounds() function to determine the smallest
    // rectangle encompassing a string, erase the area with fillRect(),
    // then draw new text.  This WILL infortunately 'blink' the text, but
    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
    // only creates a new set of problems.  Have an idea to work around
    // this (a canvas object type for MCUs that can afford the RAM and
    // displays supporting setAddrWindow() and pushColors()), but haven't
    // implemented this yet.


DefaultFontRenderer::DefaultFontRenderer(int16_t w, int16_t h) : FontRenderer(w, h) {

}
// Draw a character
/**************************************************************************/
/*!
   @brief   Draw a single character
    @param    x   Bottom left corner x coordinate
    @param    y   Bottom left corner y coordinate
    @param    c   The 8-bit font-indexed character (likely ascii)
    @param    color 16-bit 5-6-5 Color to draw chraracter with
    @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
   no background)
    @param    size_x  Font magnification level in X-axis, 1 is 'original' size
    @param    size_y  Font magnification level in Y-axis, 1 is 'original' size
*/
/**************************************************************************/
void DefaultFontRenderer::drawChar(Adafruit_GFX_NG *gfx, int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y) {

    if(!gfxFont) {

        if ((x >= _width) ||              // Clip right
            (y >= _height) ||             // Clip bottom
            ((x + 6 * size_x - 1) < 0) || // Clip left
            ((y + 8 * size_y - 1) < 0))   // Clip top
            return;

        if (!_cp437 && (c >= 176))
            c++; // Handle 'classic' charset behavior

        gfx->startWrite();
        for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
            uint8_t line = pgm_read_byte(&font[c * 5 + i]);
            for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                if (size_x == 1 && size_y == 1)
                gfx->writePixel(x + i, y + j, color);
                else
                gfx->writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y,
                                color);
            } else if (bg != color) {
                if (size_x == 1 && size_y == 1)
                gfx->writePixel(x + i, y + j, bg);
                else
                gfx->writeFillRect(x + i * size_x, y + j * size_y, size_x, size_y, bg);
            }
            }
        }
        if (bg != color) { // If opaque, draw vertical line for last column
            if (size_x == 1 && size_y == 1)
            gfx->writeFastVLine(x + 5, y, 8, bg);
            else
            gfx->writeFillRect(x + 5 * size_x, y, size_x, 8 * size_y, bg);
        }
        gfx->endWrite();
    } else {
        // Character is assumed previously filtered by write() to eliminate
        // newlines, returns, non-printable characters, etc.  Calling
        // drawChar() directly with 'bad' characters of font may cause mayhem!

        c -= (uint8_t)pgm_read_byte(&gfxFont->first);
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
        uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

        uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
        uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
        int8_t xo = pgm_read_byte(&glyph->xOffset),
            yo = pgm_read_byte(&glyph->yOffset);
        uint8_t xx, yy, bits = 0, bit = 0;
        int16_t xo16 = 0, yo16 = 0;

        if (size_x > 1 || size_y > 1) {
        xo16 = xo;
        yo16 = yo;
        }

        // Todo: Add character clipping here

        gfx->startWrite();
        for (yy = 0; yy < h; yy++) {
        for (xx = 0; xx < w; xx++) {
            if (!(bit++ & 7)) {
            bits = pgm_read_byte(&bitmap[bo++]);
            }
            if (bits & 0x80) {
            if (size_x == 1 && size_y == 1) {
                gfx->writePixel(x + xo + xx, y + yo + yy, color);
            } else {
                gfx->writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                            size_x, size_y, color);
            }
            }
            bits <<= 1;
        }
        }
        gfx->endWrite();
    }
}
/**************************************************************************/
/*!
    @brief  Print one byte/character of data, used to support print()
    @param  c  The 8-bit ascii character to write
    @note If text windowing is enabled then any character which exceeds the
    bounds will not be printed at all - the whole character is clipped.
/**************************************************************************/
size_t DefaultFontRenderer::write(Adafruit_GFX_NG *gfx, uint8_t c) {
  int16_t x0, y0, xw, yh;

    if(!gfxFont) {
        if (c == '\n') {              // Newline?
            cursor_x = x0;              // Reset x to zero,
            cursor_y += textsize_y * 8; // advance y one line
        } else if (c != '\r') {       // Ignore carriage returns
            if (wrap && ((cursor_x + textsize_x * 6) > xw)) { // Off right?
            cursor_x = x0;                                  // Reset x to zero,
            cursor_y += textsize_y * 8;                     // advance y one line
            }
            
            drawChar(gfx, cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                        textsize_y);
            
            cursor_x += textsize_x * 6; // Advance x one char
        }
    } else {
        if (c == '\n') {
            cursor_x = x0;
            cursor_y +=
                (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        } else if (c != '\r') {
            uint8_t first = pgm_read_byte(&gfxFont->first);
            if ((c >= first) && (c <= (uint8_t)pgm_read_byte(&gfxFont->last))) {
            GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
            uint8_t w = pgm_read_byte(&glyph->width),
                    h = pgm_read_byte(&glyph->height);
            if ((w > 0) && (h > 0)) { // Is there an associated bitmap?
                int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset); // sic
                uint16_t cw = textsize_x * (xo + w);
                if (wrap && ((cursor_x + cw) > xw)) {
                cursor_x = x0;
                cursor_y += (int16_t)textsize_y *
                            (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
                }
                
                drawChar(gfx, cursor_x, cursor_y, c, textcolor, textbgcolor, textsize_x,
                            textsize_y);
                
            }
            cursor_x +=
                (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
            }
        }
    }
    return 1;
}
/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void DefaultFontRenderer::setFont(const GFXfont *f) {
  if (f) {          // Font struct pointer passed in?
    if (!gfxFont) { // And no current font struct?
      // Switching from classic to new font behavior.
      // Move cursor pos down 6 pixels so it's on baseline.
      cursor_y += 6;
    }
  } else if (gfxFont) { // NULL passed.  Current font struct defined?
    // Switching from new to classic font behavior.
    // Move cursor pos up 6 pixels so it's at top-left of char.
    cursor_y -= 6;
  }
  gfxFont = (GFXfont *)f;
}



/**************************************************************************/
/*!
    @brief  Helper to determine size of a character with current font/size.
            Broke this out as it's used by both the PROGMEM- and RAM-resident
            getTextBounds() functions.
    @param  c     The ASCII character in question
    @param  x     Pointer to x location of character. Value is modified by
                  this function to advance to next character.
    @param  y     Pointer to y location of character. Value is modified by
                  this function to advance to next character.
    @param  minx  Pointer to minimum X coordinate, passed in to AND returned
                  by this function -- this is used to incrementally build a
                  bounding rectangle for a string.
    @param  miny  Pointer to minimum Y coord, passed in AND returned.
    @param  maxx  Pointer to maximum X coord, passed in AND returned.
    @param  maxy  Pointer to maximum Y coord, passed in AND returned.
*/
/**************************************************************************/
void DefaultFontRenderer::charBounds(unsigned char c, int16_t *x, int16_t *y,
                              int16_t *minx, int16_t *miny, int16_t *maxx,
                              int16_t *maxy) {

  if (gfxFont) {

    if (c == '\n') { // Newline?
      *x = 0;        // Reset x to zero, advance y by one line
      *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
    } else if (c != '\r') { // Not a carriage return; is normal char
      uint8_t first = pgm_read_byte(&gfxFont->first),
              last = pgm_read_byte(&gfxFont->last);
      if ((c >= first) && (c <= last)) { // Char present in this font?
        GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
        uint8_t gw = pgm_read_byte(&glyph->width),
                gh = pgm_read_byte(&glyph->height),
                xa = pgm_read_byte(&glyph->xAdvance);
        int8_t xo = pgm_read_byte(&glyph->xOffset),
               yo = pgm_read_byte(&glyph->yOffset);
        if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > _width)) {
          *x = 0; // Reset x to zero, advance y by one line
          *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
                x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + gw * tsx - 1,
                y2 = y1 + gh * tsy - 1;
        if (x1 < *minx)
          *minx = x1;
        if (y1 < *miny)
          *miny = y1;
        if (x2 > *maxx)
          *maxx = x2;
        if (y2 > *maxy)
          *maxy = y2;
        *x += xa * tsx;
      }
    }

  } else { // Default font

    if (c == '\n') {        // Newline?
      *x = 0;               // Reset x to zero,
      *y += textsize_y * 8; // advance y one line
      // min/max x/y unchaged -- that waits for next 'normal' character
    } else if (c != '\r') { // Normal char; ignore carriage returns
      if (wrap && ((*x + textsize_x * 6) > _width)) { // Off right?
        *x = 0;                                       // Reset x to zero,
        *y += textsize_y * 8;                         // advance y one line
      }
      int x2 = *x + textsize_x * 6 - 1, // Lower-right pixel of char
          y2 = *y + textsize_y * 8 - 1;
      if (x2 > *maxx)
        *maxx = x2; // Track max x, y
      if (y2 > *maxy)
        *maxy = y2;
      if (*x < *minx)
        *minx = *x; // Track min x, y
      if (*y < *miny)
        *miny = *y;
      *x += textsize_x * 6; // Advance x one char
    }
  }
}

/**************************************************************************/
/*!
    @brief  Helper to determine the height in pixels of the current font
    @returns The height of the font
/**************************************************************************/
uint16_t DefaultFontRenderer::getFontHeight() {
  if (!gfxFont) { // 'Classic' built-in font
    return textsize_y * 8;
  } else {
    return (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
  }
}