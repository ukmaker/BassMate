#ifndef SIMPLEGUI_SELECT_WIDGET_H
#define SIMPLEGUI_SELECT_WIDGET_H

#include "Widgets/Widget.h"

/**
 * Base class for all select widgets
 * Defines the methdos for manipulating what is selected
 * Methods which deal with actual values are declared in AbstractSelectWidget
 **/
class SelectWidget : public Widget {
 public:

 static const uint16_t ZONE_CONTENT = 1;

  SelectWidget(GraphicsContext *context) : Widget(context) {}

  // abstract, so no constructor
  // default destructor to be a good c++ citizen
  ~SelectWidget() {}

  // concrete subclasses must implement these methods
  virtual const char *getSelectedText() = 0;
  virtual const char *getText(uint16_t idx) = 0;
  virtual uint16_t getNumChoices() = 0;

  /*
   * Toggle the selected value on or off
   */
  void setSelectedIndex(uint16_t idx) {
    if (_selectedIndex == idx && _isSelected) {
      _isSelected = false;
    } else {
      _selectedIndex = idx;
      _isSelected = true;
    }
    noteDirtyContent(ZONE_CONTENT);
    _changed();
  }

  bool isSelected() { return _isSelected; }

  void unselect() {
    _isSelected = false;
    noteDirtyContent(ZONE_CONTENT);
    _changed();
  }

  uint16_t selectCurrent() {
    setSelectedIndex(getViewIndex());
    return _selectedIndex;
  }

  uint16_t getSelectedIndex() { return _selectedIndex; }

  uint16_t getViewIndex() { return _viewIndex; }

  void setViewIndex(uint8_t idx) {
    if (_viewIndex != idx) {
      _viewIndex = idx;
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

  void setTextWrap(bool wrap) {
    if (_wrap != wrap) {
      _wrap = wrap;
      noteDirtyContent(ZONE_CONTENT);
    }
  }

  void setHighlightColor(uint16_t c) {
    if (_highlightColor != c) {
      _highlightColor = c;
      noteDirtyContent(ZONE_CONTENT);
    }
  }

  uint16_t getHighlightColor() { return _highlightColor; }

  void scrollDown() {
    _viewIndex++;
    if (_viewIndex >= getNumChoices()) {
      _viewIndex = getNumChoices() - 1;
    } else {
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

  void scrollUp() {
    if (_viewIndex > 0) {
      _viewIndex--;
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

  void scrollToTop() {
    if (_viewIndex != 0) {
      _viewIndex = 0;
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

  void scrollToBottom() {
    if (_viewIndex != getNumChoices() - 1) {
      _viewIndex = getNumChoices() - 1;
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

 protected:
  /**
   * Overrides
   **/
  virtual void _drawContent(bool force = false) {
    // draw the visible text options

    // how many lines can we show at a time?
    const GFXfont *f = fontRenderer()->getFont();
    bool dwrap = fontRenderer()->getTextWrap();
    if (_font != nullptr) fontRenderer()->setFont(_font);
    fontRenderer()->setTextWrap(_wrap);
    uint16_t fontHeight = fontRenderer()->getFontHeight();
    uint16_t triangleWidth = fontHeight / 2;

    uint16_t lines = _inner.height() / fontHeight;
    if (getNumChoices() < lines) {
      lines = getNumChoices();
    }

    int sel = -1;
    fontRenderer()->setTextColor(_fg, _bg);
    fontRenderer()->setTextWindow(_inner.x() + triangleWidth + 2, _inner.y(),
                                  _inner.width() - triangleWidth - 20,
                                  lines * fontHeight);

    /**
     * centre the choices on the current view
     * with an odd number of lines, put it in the middle
     * with an even number, put it at the bottom of the top half
     * Draw an arrow to show the viewed line
     * Draw the selected value(if visible) inverted
     * e.g. 4 lines, 6 choices, view at 2
     * 0 first
     * 1
     * 2 <
     * 3 last
     * **/
    int first = _viewIndex - (lines / 2);
    int last = _viewIndex + (lines / 2) - 1;
    if (first < 0) {
      last -= first;
      first = 0;
    }

    if(last < 0) {
      last = 0;
    }

    for (int i = first; i <= last; i++) {
      if ((i >= 0) && (i < getNumChoices())) {
        const char *str = getText(i);
        int16_t x = fontRenderer()->getCursorX();
        int16_t y = fontRenderer()->getCursorY();
        int16_t x1;
        int16_t y1;
        uint16_t w;
        uint16_t h;
        fontRenderer()->getTextBounds(str, x, y, &x1, &y1, &w, &h);

        if (i == _selectedIndex) {
          // when using a custom (i.e. new-style) font, print() ignores
          // background color, so we have to clear it ourselves
          display()->fillRect(x1 - 1, y - fontHeight + 4,
                              _inner.width() - triangleWidth - 20, fontHeight,
                              _highlightColor);
          //_drawBorder(x1 - 1, y + 2 + _highlightBorderWidth, _inner.width -
          // triangleWidth - 20, fontHeight + 2);
          fontRenderer()->setTextColor(_bg, _highlightColor);
          display()->print(str);
          fontRenderer()->setTextColor(_fg, _bg);
        } else {
          display()->print(str);
        }
        if (i == _viewIndex) {
          display()->fillTriangle(_inner.x(), y1, _inner.x(), y1 + h,
                                  _inner.x() + triangleWidth, y1 + h / 2, _highlightColor);
        }
      }
      display()->print("\n");
    }

    fontRenderer()->removeTextWindow();
    fontRenderer()->setFont(f);
    fontRenderer()->setTextWrap(dwrap);
  }

  void _changed() {
    _onChange.call(this);
  }

 private:
  uint16_t _selectedIndex = 0;
  bool _isSelected = false;
  const GFXfont *_font = nullptr;
  uint16_t _viewIndex = 0;
  bool _wrap = false;
  uint16_t _highlightColor = 0xffff;
};
#endif