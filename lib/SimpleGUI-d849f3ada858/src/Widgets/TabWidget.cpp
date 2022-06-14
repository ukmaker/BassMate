#include "Widgets/TabWidget.h"

#include "Core/BorderTool.h"

/**
 * A basic widget implementation which just draws some text.
 * If the text does not fit in the bounding-box it will be clipped
 **/
TabWidget::TabWidget(GraphicsContext *context) : Window(context) {}

void TabWidget::attach(Widget *widget) {
  Window::attach(widget);
  if (_widgets.size() == 1) {
    _selected = widget;
    _idx = 0;
    widget->show();
  } else {
    widget->hide();
  }
}

bool TabWidget::left() {
    if(_idx > 0) {
        selectTabByIndex(_idx -1);
        return true;
    }
    return false;
}

bool TabWidget::right() {
    if((_idx +1) <  _widgets.size()) {
        selectTabByIndex(_idx+1);
        return true;
    }
    return false;
}

void TabWidget::selectTabByIndex(uint8_t idx) {
 if(_idx < _widgets.size()) {
     selectTab(_widgets.getByIndex(idx));
 }
}

void TabWidget::selectTab(Widget *widget) {
  if (widget != nullptr && _selected != widget) {
    if (_selected != nullptr) {
      _selected->hide();
    }
    _selected = widget;
    _selected->show();
    noteDirtyContent(ZONE_TABS);
    _idx = _widgets.indexOf(_selected);
  } else if (widget == nullptr) {
    if (_selected != nullptr) {
      _selected->hide();
    }
    _selected = nullptr;
    _idx = 0;
    noteDirtyContent(ZONE_TABS);
  }
}

void TabWidget::setTabSelectedColor(uint16_t c) {
  if (_selectedColor != c) {
    _selectedColor = c;
    noteDirty();
  }
}

void TabWidget::setTabUnselectedColor(uint16_t c) {
  if (_unselectedColor != c) {
    _unselectedColor = c;
    noteDirty();
  }
}

void TabWidget::setTabSelectedFontColor(uint16_t c) {
  if (_selectedFontColor != c) {
    _selectedFontColor = c;
    noteDirty();
  }
}

void TabWidget::setTabUnselectedFontColor(uint16_t c) {
  if (_unselectedFontColor != c) {
    _unselectedFontColor = c;
    noteDirty();
  }
}

void TabWidget::setTabBorderColor(uint16_t c) {
  if (_tabBorderColor != c) {
    _tabBorderColor = c;
    noteDirty();
  }
}

uint16_t TabWidget::getTabSelectedColor() { return _selectedColor; }

uint16_t TabWidget::getTabUnselectedColor() { return _unselectedColor; }

uint16_t TabWidget::getTabSelectedFontColor() { return _selectedFontColor; }

uint16_t TabWidget::getTabUnselectedFontColor() { return _unselectedFontColor; }

uint16_t TabWidget::getTabBorderColor() { return _tabBorderColor; }

void TabWidget::setTabHeight(uint16_t h) {
  if (_tabHeight != h) {
    _tabHeight = h;
    adjust();
    noteDirtyContent(ZONE_TABS | ZONE_CONTENT);
  }
}
void TabWidget::setTabWidth(uint16_t w) {
  if (_tabWidth != w) {
    _tabWidth = w;
    noteDirtyContent(ZONE_TABS);
  }
}
uint16_t TabWidget::getTabWidth() { return _tabWidth; }
uint16_t TabWidget::getTabHeight() { return _tabHeight; }

void TabWidget::setTabBorderWidth(uint8_t w) {
  if (_tabBorderWidth != w) {
    _tabBorderWidth = w;
    noteDirtyContent(ZONE_TABS | ZONE_CONTENT);
  }
}
uint8_t TabWidget::getTabBorderWidth() { return _tabBorderWidth; }

void TabWidget::setTabPadding(uint8_t p) {
  if (_tabPadding != p) {
    _tabPadding = p;
    noteDirtyContent(ZONE_TABS | ZONE_CONTENT);
  }
}
uint8_t TabWidget::getTabPadding() { return _tabPadding; }

void TabWidget::focusChild() {
  if (_selected != nullptr) {
    unfocus();
    _selected->focus();
  }
}

void TabWidget::unfocusChild() {
  if (_selected != nullptr) {
    focus();
    _selected->unfocus();
  }
}

void TabWidget::_drawContent(bool force) {
  // set up for text
  const GFXfont *f = fontRenderer()->getFont();
  if (_font != nullptr) fontRenderer()->setFont(_font);

  uint16_t bx = _inner.x;
  uint16_t tx = _inner.x + _tabBorderWidth;
  uint16_t ty = _inner.y + _tabBorderWidth;
  uint16_t fx = tx + _tabPadding;
  uint16_t fy = ty + _tabPadding;
  uint16_t cy = _inner.y + _tabHeight;
  uint16_t to = _tabBorderWidth + _tabPadding;
  uint16_t tw = _tabWidth - _tabBorderWidth * 2;
  uint16_t th = _tabHeight - _tabBorderWidth * 2;
  uint16_t fw = _tabWidth - to * 2;
  uint16_t fh = _tabHeight - to * 2;

  Widget *w;
  _widgets.reset();
  while (w = _widgets.next()) {
    // tab border
    BorderTool::border(display(), bx, _inner.y, _tabWidth, _tabHeight,
                       _tabBorderWidth, _tabBorderColor);
    if (w == _selected) {
      // fill tab
      display()->fillRect(tx, ty, tw, th, _selectedColor);
      fontRenderer()->setTextColor(_selectedFontColor, _selectedColor);
    } else {
      // fill tab
      display()->fillRect(tx, ty, tw, th, _bg);
      fontRenderer()->setTextColor(_unselectedFontColor, _unselectedColor);
    }
    fontRenderer()->setTextWindow(fx, fy, fw, fh);
    display()->print(w->getLabel());
    bx += _tabWidth;
    tx += _tabWidth;
    fx += _tabWidth;
  }

  fontRenderer()->removeTextWindow();
  fontRenderer()->setFont(f);
}

void TabWidget::_drawChildren(bool force) {
  Widget *w;
  _widgets.reset();
  while (w = _widgets.next()) {
    if (w == _selected) {
      w->draw(force);
    }
  }
}

// override
// reserve height for the tabs, tab padding
// force fixed layout
void TabWidget::_adjust() {
  /*
   * interpret tab border and padding to apply to the tabs and the the inner
   *
   *  ----------------------------------------------------------
   *  |
   *  |    -----------------------  ^
   *  |    |   ^                 |  |
   *  |    |   |text             | tab height includes tabBorderWidth
   *  |    |  tabPadding  -->    |  |
   *  |    -----------------------  |
   *  |    ||                   |
   *  |    |^---- start of widget
   *  |    ^----- tabBorder
   *  |  ^------- padding
   *  ^---------- border
   *
   *
   *
   *
   *
   */
  Widget::_adjust();

  int wx = _inner.x + _tabBorderWidth;
  int wy = _inner.y + _tabHeight;

  Widget *w;

  _widgets.reset();

  while ((w = _widgets.next()) != NULL) {
    w->setOffset(wx, wy);
    w->setSize(_inner.width - _box.getWidth(),
               _inner.height - _box.getHeight() - _tabHeight);
  }
}
