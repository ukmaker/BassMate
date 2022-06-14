#ifndef SIMPLEGUI_OKCANCEL_WIDGET_H
#define SIMPLEGUI_OKCANCEL_WIDGET_H

#include "Core/ColorsRGB16.h"
#include "Widget.h"

namespace simplegui {

class Button : public Widget {
 public:
  typedef void (*ButtonCallback)();

  static const uint16_t ZONE_BUTTONS = 1;

  Button(GraphicsContext *context) : Widget(context) {
    _highlightForegroundColor = WHITE;
    _highlightBackgroundColor = BLACK;
    _dirtyContent = 0xffff;
  }

  template <class T>
  void onClick(T *tptr, void (T::*mptr)()) {
    _clickCallback.attach(tptr, mptr);
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
    _clickCallback.call();
  }

 protected:
  uint16_t _highlightForegroundColor, _highlightBackgroundColor;
  FunctionPointer _clickCallback;

  virtual void _drawContent(bool force) {
    fontRenderer()->setFont(_font);
    fontRenderer()->setTextColor(_fg, _bg);

    int dx = fontRenderer()->getFontHeight();
    int dy = dx;

    uint16_t l = _inner.left() + dx;
    uint16_t t = _inner.top() + 4 * dy;
    int16_t x, y, x1, y1 = 0;
    uint16_t w, h = 0;

    if (force || _dirty || _dirtyContent) {
      uint16_t width = 0;
      x = l;
      y = t + 1.5 * dy;
      fontRenderer()->getTextBounds(_label, x, y, &x1, &y1, &width, &h);

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
    display()->write(text);
    fontRenderer()->removeTextWindow();
  }
};
}  // namespace simplegui

#endif