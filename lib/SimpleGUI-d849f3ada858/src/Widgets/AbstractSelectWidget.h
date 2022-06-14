#ifndef SIMPLEGUI_ASELECT_WIDGET_H
#define SIMPLEGUI_ASELECT_WIDGET_H

#include "Core/BorderTool.h"
#include "Core/Color.h"
#include "Widgets/Widget.h"

/**
 * a select widget allows the user to choose one of a set of choices
 * Choices are presented as scrolling text, with the selected choice centred and
 *bolded Choices are mapped to a backing value
 **/
template <typename V>
class AbstractSelectWidget : public Widget {
 public:

 static const uint16_t ZONE_CONTENT = 1;

  AbstractSelectWidget(GraphicsContext *context) : Widget(context) {}

  // abstract, so no constructor
  // default destructor to be a good c++ citizen
  ~AbstractSelectWidget() {}

  // concrete subclasses must implement these methods
  virtual const char *getSelectedText() = 0;
  virtual const char *getText(uint16_t idx) = 0;
  virtual const V getSelectedValue() = 0;
  virtual const V getValue(uint16_t idx) = 0;
  virtual uint16_t getNumChoices() = 0;
  virtual bool equals(V a, V b) = 0;

  void setSelectedValue(V value) {
    for (uint16_t idx = 0; idx < getNumChoices(); idx++) {
      if (equals(value, getValue(idx))) {
        setSelectedIndex(idx);
        setViewIndex(idx);
        return;
      }
    }
    unselect();
  }

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

  /*
   * Register an event handler to be called whenever a change occurs:
   * - scrollUp or scrollDown
   * - setSelected
   */
  template <class T>
  void onChange(T *tptr, void (T::*mptr)(Event e)) {
    _onChange.attach(tptr, mptr);
  }

  void scrollUp() {
    _viewIndex++;
    if (_viewIndex >= getNumChoices()) {
      _viewIndex = getNumChoices() - 1;
    } else {
      noteDirtyContent(ZONE_CONTENT);
      _changed();
    }
  }

  void scrollDown() {
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

    uint16_t lines = _inner.height / fontHeight;
    if (getNumChoices() < lines) {
      lines = getNumChoices();
    }

    int sel = -1;
    fontRenderer()->setTextColor(_fg, _bg);
    fontRenderer()->setTextWindow(_inner.x + triangleWidth + 2, _inner.y,
                                  _inner.width - triangleWidth - 20,
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
                              _inner.width - triangleWidth - 20, fontHeight,
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
          display()->fillTriangle(_inner.x, y1, _inner.x, y1 + h,
                                  _inner.x + triangleWidth, y1 + h / 2, _fg);
        }
      }
      display()->print("\n");
    }

    fontRenderer()->removeTextWindow();
    fontRenderer()->setFont(f);
    fontRenderer()->setTextWrap(dwrap);
  }

  void _changed() {
    Event e;
    e.screenX = 0;
    e.screenY = 0;
    e.type = EventType::WIDGET_CHANGED;
    _onChange.call(e);
  }

 private:
  uint16_t _selectedIndex = 0;
  bool _isSelected = false;
  const GFXfont *_font = nullptr;
  uint16_t _viewIndex = 0;
  bool _wrap = false;
  uint16_t _highlightColor = 0xffff;

  FunctionPointerArg1<void, Event> _onChange;
};
#endif