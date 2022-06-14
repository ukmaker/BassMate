#include "DefaultFontRenderer_NG.h"

#include "Adafruit_SPITFT_NG.h"
#include "adafruit_pgmspace.h"

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

DefaultFontRenderer_NG::DefaultFontRenderer_NG(const GFXfont *font, int16_t w,
                                               int16_t h)
    : FontRenderer(w, h) {
  setFont(font);
  textWindowed = false;
  textX = textY = textW = textH = 0;
  _stmdma = nullptr;
  _valign = TOP;
  _halign = LEFT;
  _prepared = false;
}

void DefaultFontRenderer_NG::setVAlign(VAlign align) { _valign = align; }

void DefaultFontRenderer_NG::setHAlign(HAlign align) { _halign = align; }

/**
 * work out where the drawing position needs to start
 * in order to fit the text in the window and satisfy
 * the alignment settings
 **/
void DefaultFontRenderer_NG::writeAligned(Adafruit_GFX_NG *gfx,
                                          const char *str) {
  if (_halign == LEFT && _valign == TOP) {
    // nothing to do, just use what was set by setTextWindow
    gfx->write(str);
    return;
  }

  const char *start;
  start = str;
  int nLines = 0;
  int fontHeight = getFontHeight();
  int totalHeight = fontHeight;
  int firstLineWidth = 0, width = 0;

  if (fitLine(&start, &firstLineWidth)) {
    nLines++;
    // continue for the rest of the lines just so we can count them all
    while ((totalHeight < textH) && fitLine(&start, &width)) {
      totalHeight += fontHeight;
      nLines++;
    }
  }
  if (nLines > 0) {
    // Figure out where to set cursorX and cursorY
    switch (_halign) {
      case LEFT:
        // already set
        break;

      case CENTER:
        cursor_x = textX + (textW - firstLineWidth) / 2;
        break;

      case RIGHT:
        cursor_x = textX + (textW - firstLineWidth);
        break;
    }

    switch (_valign) {
      case TOP:
        // already set
        break;

      case MIDDLE: {
        if (totalHeight < textH) {  // otherwise already set to top
          // use the space
          cursor_y = textY + (textH - totalHeight) / 2;
        }
      } break;

      case BOTTOM: {
        if (totalHeight < textH) {
          cursor_y = textY + (textH - totalHeight);
        }
      } break;
    }

    //textWindowed = false;

    // now print
    if (_halign == LEFT) {
      gfx->write(str);
    } else {
      // Need to set cursor_x for each line
      start = str;
      const char *next;
      next = str;

      while (fitLine(&next, &width)) {
        if(_halign == CENTER) {
          cursor_x = textX + (textW - width) /2;
        } else {
          cursor_x = textX + (textW - width);
        }
        while(start < next) {
          write(gfx, *start);
          start++;
        }
        cursor_y += fontHeight;
      }
    }
  }
}

// fit a line to the width of the window
// split the line at word boundaries
// return true if there is more to do
// return false if reached the end of the string
// Return false if a character does not fit in the width of the
// text window.
// If a word is wider than the text window, split it
bool DefaultFontRenderer_NG::fitLine(const char **start, int *lineWidth) {
  if (isEnd(**start)) {
    return false;
  }
  *lineWidth = 0;
  int spaceWidth, lastSpaceWidth = 0, wordWidth = 0, charWidth = 0;
  const char *end;
  end = *start;

  while(getWordWidth(start, &end, &wordWidth, &spaceWidth)) {
    // something added width to the string
    if((*lineWidth + lastSpaceWidth + wordWidth) > textW) {
      // Need to wrap. Can the word fit on the next line?
      if(wordWidth < textW) {
        // OK to wrap on the word boundary
        // there's room for the word on the next line
        // leave start pointing at this word
        return true;
      } else {
        // see if we can split the word
        charWidth = getCharWidth(**start);
        if(charWidth > textW) {
          // textW must be really narrow - can't even fit a single char
          return false;
        }
        if((*lineWidth + lastSpaceWidth + charWidth) > textW) {
          // no room on this line for one more letter
          return true;
        }
        *lineWidth += lastSpaceWidth;
        // OK to append this char
        *lineWidth += charWidth;
        while(*start < end) {
          (*start)++;
          charWidth = getCharWidth(**start);
          if((*lineWidth + charWidth) < textW) {
            *lineWidth += charWidth;
          } else {
            return true;
          }
        }
        return true;
      }
    } else {
      // Room for more words
      *lineWidth += wordWidth + lastSpaceWidth;
      lastSpaceWidth = spaceWidth;
      *start = end;
    }
  }
  return isEnd(**start);
}

bool DefaultFontRenderer_NG::getWordWidth(const char **start, const char **end, int *wordWidth, int *spaceWidth) {
  // skip leading spaces
  *wordWidth = 0;
  *spaceWidth = 0;

  if(isEnd(**start)) {
    return false; // We're done here
  }

  // Include leading spaces in th word width
  while (isSpace(**start) && !isEnd(**start)) {
    *wordWidth += getCharWidth(**start);
    (*start)++;
  }

  *end = *start;

  if (isEnd(**start)) {
    return true; // we've done some work. next call will return false
  }

  while (!isSpace(**end) && !isEnd(**end)) {
    *wordWidth += getCharWidth(**end);
    (*end)++;
  }

  if (isEnd(**end)) {
    return true; // we've done some work. next call will return false
  }

  while (isSpace(**end) && !isEnd(**end)) {
    *spaceWidth += getCharWidth(**end);
    (*end)++;
  }

  return true;
}

uint8_t DefaultFontRenderer_NG::getCharWidth(const char c) {
  GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - (uint8_t)pgm_read_byte(&gfxFont->first));
  uint8_t w = pgm_read_byte(&glyph->xAdvance);
  return w;
}

 bool DefaultFontRenderer_NG::isSpace(const char c) { return c == ' '; }
 bool DefaultFontRenderer_NG::isNewline(const char c) { return c == '\n'; }
 bool DefaultFontRenderer_NG::isEnd(const char c) { return c == '\0'; }

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
void DefaultFontRenderer_NG::drawChar(Adafruit_GFX_NG *gfx, int16_t x,
                                      int16_t y, unsigned char c,
                                      uint16_t color, uint16_t bg,
                                      uint8_t size_x, uint8_t size_y) {
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

  if (_stmdma != nullptr) {
    _stmdma->beginWindow(x + xo, y + yo, w, h, bg);
  } else {
    gfx->startWrite();
  }
  for (yy = 0; yy < h; yy++) {
    for (xx = 0; xx < w; xx++) {
      if (!(bit++ & 7)) {
        bits = pgm_read_byte(&bitmap[bo++]);
      }
      if (bits & 0x80) {
        if (size_x == 1 && size_y == 1) {
          if (_stmdma != nullptr) {
            _stmdma->drawWindowPixel(xx, yy, color);
          } else {
            gfx->writePixel(x + xo + xx, y + yo + yy, color);
          }
        } else {
          gfx->writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
                            size_x, size_y, color);
        }
      }
      bits <<= 1;
    }
  }
  if (_stmdma != nullptr) {
    _stmdma->flushWindow();
    _stmdma->waitComplete();
  }
  gfx->endWrite();
}
/**************************************************************************/
/*!
    @brief  Print one byte/character of data, used to support print()
    @param  c  The 8-bit ascii character to write
    @note If text windowing is enabled then any character which exceeds the
    bounds will not be printed at all - the whole character is clipped.
/**************************************************************************/
size_t DefaultFontRenderer_NG::write(Adafruit_GFX_NG *gfx, uint8_t c) {
  int16_t x0, y0, xw, yh;
  if (textWindowed) {
    x0 = textX;
    xw = textW;
    y0 = textY;
    yh = textH;
  } else {
    x0 = 0;
    xw = _width;
    y0 = 0;
    yh = _height;
  }

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
      if ((w > 0) && (h > 0)) {  // Is there an associated bitmap?
        int16_t xo = (int8_t)pgm_read_byte(&glyph->xOffset);  // sic
        uint16_t cw = textsize_x * (xo + w);
        if (wrap && ((cursor_x + cw) > (x0 + xw))) {
          cursor_x = x0;
          cursor_y +=
              (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
        }
        if (!textWindowed || (textWindowed && ((cursor_x + cw) <= (x0 + xw)) &&
                              (cursor_y <= (y0 + yh)))) {
          drawChar(gfx, cursor_x, cursor_y, c, textcolor, textbgcolor,
                   textsize_x, textsize_y);
        }
      }
      cursor_x +=
          (uint8_t)pgm_read_byte(&glyph->xAdvance) * (int16_t)textsize_x;
    }
  }
  return 1;
}

/**************************************************************************/
/*!
    @brief  Set a window to print text in, clip lines which are too long
    @param  x  x coordinate of the window's top left point
    @param  y  y coordinate of the window's top left point
    @param  w  The width of the window
    @param  h  The height of the window
    @note Also sets the text cursor position to the baseline
/**************************************************************************/
void DefaultFontRenderer_NG::setTextWindow(int16_t x, int16_t y, int16_t w,
                                           int16_t h) {
  textX = cursor_x = x;
  textY = y;
  cursor_y = y - 1 - ymax +
             (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
  textW = w;
  textH = h;
  textWindowed = true;
}
/**************************************************************************/
/*!
    @brief  Disables text windowing
/**************************************************************************/
void DefaultFontRenderer_NG::removeTextWindow() {
  textWindowed = false;
  _prepared = false;
}

void DefaultFontRenderer_NG::getTextBounds(const char *string, int16_t x,
                                           int16_t y, int16_t *x1, int16_t *y1,
                                           uint16_t *w, uint16_t *h) {
  FontRenderer::getTextBounds(string, x, y, x1, y1, w, h);
  if (textWindowed) {
    if (*x1 < textX) *x1 = textX;
    if (*w > textW) *w = textW;
    if (*y1 < textY) *y1 = textY;
    if (*h > textH) *h = textH;
  }
}
void DefaultFontRenderer_NG::getTextBounds(const __FlashStringHelper *s,
                                           int16_t x, int16_t y, int16_t *x1,
                                           int16_t *y1, uint16_t *w,
                                           uint16_t *h) {
  FontRenderer::getTextBounds(s, x, y, x1, y1, w, h);

  if (textWindowed) {
    if (*x1 < textX) *x1 = textX;
    if (*w > textW) *w = textW;
    if (*y1 < textY) *y1 = textY;
    if (*h > textH) *h = textH;
  }
}

/**************************************************************************/
/*!
    @brief Set the font to display when print()ing, either custom or default
    @param  f  The GFXfont object, if NULL use built in 6x8 font
*/
/**************************************************************************/
void DefaultFontRenderer_NG::setFont(const GFXfont *f) {
  if (f != nullptr && f != gfxFont) {
    gfxFont = (GFXfont *)f;
    // Find the lowest descender
    // Use this in setTextWindow to set where the bottom left point is
    uint8_t first = pgm_read_byte(&gfxFont->first);
    uint8_t last = pgm_read_byte(&gfxFont->last);
    ymax = 0;
    ymin = 0;
    for (uint8_t c = first; c <= last; c++) {
      GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
      int8_t yo = pgm_read_byte(&glyph->yOffset);
      if (yo > ymax) ymax = yo;
      if (yo < ymin) ymin = yo;
    }
  }
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
void DefaultFontRenderer_NG::charBounds(unsigned char c, int16_t *x, int16_t *y,
                                        int16_t *minx, int16_t *miny,
                                        int16_t *maxx, int16_t *maxy) {
  if (c == '\n') {  // Newline?
    *x = 0;         // Reset x to zero, advance y by one line
    *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
  } else if (c != '\r') {  // Not a carriage return; is normal char
    uint8_t first = pgm_read_byte(&gfxFont->first),
            last = pgm_read_byte(&gfxFont->last);
    if ((c >= first) && (c <= last)) {  // Char present in this font?
      GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c - first);
      uint8_t gw = pgm_read_byte(&glyph->width),
              gh = pgm_read_byte(&glyph->height),
              xa = pgm_read_byte(&glyph->xAdvance);
      int8_t xo = pgm_read_byte(&glyph->xOffset),
             yo = pgm_read_byte(&glyph->yOffset);
      if (wrap && ((*x + (((int16_t)xo + gw) * textsize_x)) > _width)) {
        *x = 0;  // Reset x to zero, advance y by one line
        *y += textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
      }
      int16_t tsx = (int16_t)textsize_x, tsy = (int16_t)textsize_y,
              x1 = *x + xo * tsx, y1 = *y + yo * tsy, x2 = x1 + xa * tsx - 1,
              y2 = y1 + gh * tsy - 1;
      if (x1 < *minx) *minx = x1;
      if (y1 < *miny) *miny = y1;
      if (x2 > *maxx) *maxx = x2;
      if (y2 > *maxy) *maxy = y2;
      *x += xa * tsx;
    }
  }
}

/**************************************************************************/
/*!
    @brief  Helper to determine the height in pixels of the current font
    @returns The height of the font
/**************************************************************************/
uint16_t DefaultFontRenderer_NG::getFontHeight() {
  return (int16_t)textsize_y * (uint8_t)pgm_read_byte(&gfxFont->yAdvance);
}

void DefaultFontRenderer_NG::setBlitter(STMDMA *stmdma) { _stmdma = stmdma; }