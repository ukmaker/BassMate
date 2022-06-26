#ifndef SIMPLEGUI_BUTTONS_WIDGET_H
#define SIMPLEGUI_BUTTONS_WIDGET_H

#include "Core/ColorsRGB16.h"
#include "Widget.h"

namespace simplegui {

class ButtonsWidget : public Widget {
 public:
  static const uint16_t ZONE_BUTTONS = 1;
  static const uint16_t ZONE_TEXT = 2;

  ButtonsWidget(GraphicsContext *context, const char *labels[],
                const uint8_t numLabels)
      : Widget(context), _labels(labels), _numButtons(numLabels) {
    _highlightForegroundColor = WHITE;
    _highlightBackgroundColor = BLACK;
    _dirtyContent = 0xffff;
    _buttonWidth = new uint16_t[numLabels];
    for (uint8_t i = 0; i < numLabels; i++) {
      setLabel(i, labels[i]);
    }
    setSelectedButton(0);
  }

  void setLabel(uint8_t idx, const char *label) {
    if (idx < _numButtons && _labels[idx] != label) {
      _labels[idx] = label;
      noteDirtyContent(ZONE_BUTTONS);
    }
  }

  template <class T>
  void onButtonClickPress(T *tptr, void (T::*mptr)(uint8_t button)) {
    _buttonClickCallback.attach(tptr, mptr);
  }

  void setSelectedButton(uint8_t b) {
    if (_selectedButton != b & b < _numButtons) {
      _selectedButton = b;
      noteDirtyContent(ZONE_BUTTONS);
    }
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
    if (_selectedButton > 0) setSelectedButton(_selectedButton - 1);
  }

  void right() {
    if (_selectedButton < _numButtons) setSelectedButton(_numButtons + 1);
  }

  void click() { _buttonClickCallback.call(_selectedButton); }

 protected:
  uint16_t _highlightForegroundColor, _highlightBackgroundColor;
  FunctionPointerArg1<void, uint8_t> _buttonClickCallback;

  const char **_labels;
  const uint8_t _numButtons;
  uint16_t *_buttonWidth;
  uint8_t _selectedButton;

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
    // including button padding and border
    uint16_t h = fontRenderer()->getFontHeight() + 10;
    display()->fillRect(0, _buttonsY(), _inner.width(), h, RED);
  }

  uint16_t _getButtonWidth(uint8_t idx) {
    if (idx < _numButtons) {
      int16_t x = 0, x1 = 0;
      int16_t y = 0, y1 = 0;
      uint16_t w = 0 , h = 0;
      const char *l = _labels[idx];
      fontRenderer()->getTextBounds(l, x, y, &x1, &y1, &w, &h);
      return w + 10;
    } else {
      return 0;
    }
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
    uint16_t usedWidth = 0;

    if (force || _dirty || (_dirtyContent & ZONE_BUTTONS)) {
      x = l;
      y = t + 1.5 * dy;
      for (uint8_t i = 0; i < _numButtons; i++) {
        _buttonWidth[i] = _getButtonWidth(i);
        usedWidth += _buttonWidth[i];
      }
      // space the buttons equal on all sides
      uint16_t remainingWidth = _inner.width() - usedWidth;
      uint16_t spacing = remainingWidth / _numButtons;

      for (uint8_t i = 0; i < _numButtons; i++) {
        usedWidth += _buttonWidth[i];

        _button(x + spacing, y, _buttonWidth[i], dy, 3, 2, _labels[i],
                _selectedButton == i);
      }
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