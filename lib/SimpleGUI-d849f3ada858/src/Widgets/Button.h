#ifndef SIMPLEGUI_BUTTON_H
#define SIMPLEGUI_BUTTON_H

#include "Core/ColorsRGB16.h"
#include "Widget.h"

namespace simplegui {

class Button : public Widget {
 public:

  static const uint16_t ZONE_BUTTONS = 1;

  Button(GraphicsContext *context) : Widget(context) {
    _highlightForegroundColor = WHITE;
    _highlightBackgroundColor = BLACK;
    _dirtyContent = 0xffff;
  }

  ~Button() {}

  /**
   * @brief This is just a synonym for onChange to register the
   * callback to be called when the button is clicked
   * 
   * @param tptr 
   * @param mptr 
   */
  template <class T>
  void onClick(T *tptr, void (T::*mptr)(Widget*)) {
    _onChange.attach(tptr, mptr);
  }

  void onClick(void (*mptr)(Widget*)) {
    _onChange.attach(mptr);
  }

  void setHighlightForegroundColor(uint16_t color) {
    if (_highlightForegroundColor != color) {
      _highlightForegroundColor = color;
      noteDirtyContent(0xffff);
    }
  }

  uint16_t getHighlightForegroundColor() { return _highlightForegroundColor; }

  void setHighlightBackgroundColor(uint16_t color) {
    if (_highlightBackgroundColor != color) {
      _highlightBackgroundColor = color;
      noteDirtyContent(0xffff);
    }
  }

  uint16_t getHighlightBackgroundColor() { return _highlightBackgroundColor; }

  void select() {
    noteDirtyContent(ZONE_BUTTONS);
    _onChange.call(this);
  }

 protected:
  uint16_t _highlightForegroundColor, _highlightBackgroundColor;

  virtual void _drawContent(bool force) {
    fontRenderer()->setFont(_font);
    fontRenderer()->setTextColor(_fg, _bg);

    int dx = fontRenderer()->getFontHeight();
    int dy = dx;

    uint16_t l = _inner.left() + dx;
    uint16_t t = _inner.top();
    int16_t x, y, x1, y1 = 0;
    uint16_t w, h = 0;

    if (force || _dirty || _dirtyContent) {
      uint16_t width = 0;
      x = l;
      y = t;
      fontRenderer()->getTextBounds(_label, x, y, &x1, &y1, &width, &h);

      // centre the button vertically and horizontally
      x = _inner.left() + (_inner.width() - width) / 2;
      y = _inner.top() + (_inner.height() - dy)/2;

      _button(x, y, width, dy, 3, 2, _label, hasFocus());
    }
  }

  /*
   * x, y : top left location of button
   * w, h : width and height of the text
   * p : padding to put bottom, left and right of the text
   * b: border width of the button outline
   */
  void _button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t p,
               uint8_t b, const char *text, bool highlight) {
    uint16_t fg, bg;

    if (highlight && hasFocus()) {
      fg = _highlightForegroundColor;
      bg = _highlightBackgroundColor;
    } else {
      fg = _fg;
      bg = _bg;
    }

    display()->fillRoundRect(x, y, w + 2 * (b + p), h + 2 * b + p, 2 * b, fg);

    display()->fillRoundRect(x + b, y + b, w + 2 * p, h + p, 2 * b, bg);
    fontRenderer()->setTextColor(fg, bg);

    fontRenderer()->setTextWindow(x + b + p, y + p, w + 2 * p, h + p);
    fontRenderer()->write(text);
    fontRenderer()->removeTextWindow();
  }
};
}  // namespace simplegui

#endif