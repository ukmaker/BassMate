#ifndef SIMPLEGUI_OKCANCEL_WIDGET_H
#define SIMPLEGUI_OKCANCEL_WIDGET_H

#include "Core/ColorsRGB16.h"
#include "Widget.h"

namespace simplegui {

class OKCancelWidget : public Widget {
 public:
  typedef void (*OKCancelCallback)();

  enum State { OK, CANCEL, USER };

  static const uint16_t ZONE_BUTTONS = 1;

  OKCancelWidget(GraphicsContext *context)
      : Widget(context), _state(CANCEL) {
    _highlightForegroundColor = WHITE;
    _highlightBackgroundColor = BLACK;
    _user = false;
    _dirtyContent = 0xffff;
  }

  template <class T>
  void onKeyPress(T *tptr, void (T::*mptr)(char c)) {
    _keyPressCallback.attach(tptr, mptr);
  }

  template <class T>
  void onOK(T *tptr, void (T::*mptr)()) {
    _okCallback.attach(tptr, mptr);
  }

  template <class T>
  void onCancel(T *tptr, void (T::*mptr)()) {
    _cancelCallback.attach(tptr, mptr);
  }

  template <class T>
  void onUserKey(T *tptr, void (T::*mptr)()) {
    _userCallback.attach(tptr, mptr);
  }

  void setUserLabel(const char *label) {
    _userLabel = label;
    _user = true;
    noteDirtyContent(ZONE_BUTTONS);
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

  void left() {
    switch (_state) {
      case USER: {
        _state = OK;
        noteDirtyContent(ZONE_BUTTONS);
        break;
      }
      case OK: {
        _state = CANCEL;
        noteDirtyContent(ZONE_BUTTONS);
        break;
      }
      default:
        break;
    }
  }

  void right() {
    switch (_state) {
      case CANCEL: {
        _state = OK;
        noteDirtyContent(ZONE_BUTTONS);
        break;
      }

      case OK:
        if (_user) {
          _state = USER;
          noteDirtyContent(ZONE_BUTTONS);
          break;
        }

      default:
        break;
    }
  }

  void select() {
    switch (_state) {
      case OK: {
        noteDirtyContent(ZONE_BUTTONS);
        _okCallback.call();
        break;
      }

      case CANCEL: {
        noteDirtyContent(ZONE_BUTTONS);
        _cancelCallback.call();
        break;
      }

      case USER: {
        noteDirtyContent(ZONE_BUTTONS);
        _userCallback.call();
        break;
      }

      default:
        break;
    }
  }

 protected:
  uint16_t _highlightForegroundColor, _highlightBackgroundColor;
  FunctionPointerArg1<void, char> _keyPressCallback;
  FunctionPointer _okCallback, _cancelCallback, _userCallback;

  const char *_ok = "OK";
  const char *_cancel = "Cancel";
  bool _user = false;
  const char *_userLabel;
  State _state;

  virtual void _clearContent(bool force) {
    fontRenderer()->setFont(_font);
    if (force) {
      Widget::_clearContent(force);
    } else {
      if (_dirtyContent & ZONE_BUTTONS) {
        _clearButtons();
      }
    }
  }

  virtual void _clearButtons() {
    uint16_t h = fontRenderer()->getFontHeight() +
                 10;  // including button padding and border
    display()->fillRect(0, _buttonsY(), _inner.width, h, RED);
  }

  uint16_t _buttonsY() {
    return _inner.top() + 8 * fontRenderer()->getFontHeight();
  }

  virtual void _drawContent(bool force) {
    fontRenderer()->setFont(_font);
    fontRenderer()->setTextColor(_fg, _bg);

    int dx = fontRenderer()->getFontHeight();
    int dy = dx;

    uint16_t l = _inner.left() + dx;
    uint16_t t = _inner.top() + 4 * dy;
    int16_t x, y, x1, y1 = 0;
    uint16_t w, h = 0;

    if (force || _dirty || (_dirtyContent & ZONE_BUTTONS)) {
      // OK and Cancel buttons
      // And user button if enabled

      uint16_t cancelWidth, okWidth, userWidth, usedWidth = 0, n = 3;
      x = l;
      y = t + 1.5 * dy;
      fontRenderer()->getTextBounds(_ok, x, y, &x1, &y1, &okWidth, &h);
      usedWidth += okWidth;

      fontRenderer()->getTextBounds(_cancel, x, y, &x1, &y1, &cancelWidth, &h);
      usedWidth += cancelWidth;

      if (_user) {
        fontRenderer()->getTextBounds(_userLabel, x, y, &x1, &y1, &userWidth,
                                      &h);
        usedWidth += userWidth;
        n = 4;
      }
      // space the buttons equal on all sides
      uint16_t remainingWidth = _inner.width - usedWidth;
      uint16_t spacing = remainingWidth / n;

      _button(x + spacing, y, cancelWidth, dy, 3, 2, _cancel, _state == CANCEL);
      _button(x + cancelWidth + 2 * spacing, y, okWidth, dy, 3, 2, _ok,
              _state == OK);
      if (_user)
        _button(x + cancelWidth + okWidth + 3 * spacing, y, okWidth, dy, 3, 2,
                _userLabel, _state == USER);
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