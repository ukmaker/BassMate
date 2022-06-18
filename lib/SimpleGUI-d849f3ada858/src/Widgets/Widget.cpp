#include "Widgets/Widget.h"

#include "Core/BorderTool.h"

Widget::Widget(GraphicsContext *context)
    : _context(context),
      _fg(0xffff),
      _bg(0x0000),
      _offset(0, 0),
      _inner(0, 0, 0, 0),
      _outer(0, 0, 0, 0),
      _hidden(false),
      _dirty(true),
      _dirtyContent(0xffff),
      _bevel(false),
      _font(nullptr),
      _focus(false) {
  setMargin(0);
  setPadding(0);
  setBorderWidth(0);
  setBorderColor(0x00);
}

void Widget::setParent(Widget *parent) { _parent = parent; }

Widget *Widget::getParent() { return _parent; }

// Location is referred to the offset
void Widget::setLocation(int x, int y) {
  if ((_outer.x != (x + _offset.x())) || (_outer.y != y + _offset.y())) {
    _outer.x = x + _offset.x();
    _outer.y = y + _offset.y();
    adjust();
  }
}

void Widget::setSize(int width, int height) {
  if (_outer.width != width || _outer.height != height) {
    _outer.width = width;
    _outer.height = height;
    adjust();
  }
}

void Widget::setWidth(int width) {
  if (_outer.width != width) {
    _outer.width = width;
    adjust();
  }
}

void Widget::setHeight(int height) {
  if (_outer.height != height) {
    _outer.height = height;
    adjust();
  }
}

int Widget::x() { return _outer.x; }

int Widget::y() { return _outer.y; }

int Widget::height() { return _outer.height; }

int Widget::width() { return _outer.width; }

void Widget::setOffset(int x, int y) {
  _outer.x = _outer.x - _offset.x() + x;
  _outer.y = _outer.y - _offset.y() + y;
  _offset.x(x);
  _offset.y(y);
  adjust();
}

int Widget::offsetX() { return _offset.x(); }

int Widget::offsetY() { return _offset.y(); }

void Widget::setForeground(uint16_t color) {
  if (_fg != color) {
    _fg = color;
    noteDirty();
  }
}

void Widget::setBackground(uint16_t color) {
  if (_bg != color) {
    _bg = color;
    noteDirty();
  }
}

/**
 * Set the amount of padding between the border and a widget edge
 **/
void Widget::setPadding(int pixels) {
  _box.set(Box::Side::ALL, Edge::PADDING, pixels);
  adjust();
}
void Widget::setMargin(int pixels) {
  _box.set(Box::Side::ALL, Edge::MARGIN, pixels);
  adjust();
}

void Widget::setBorderWidth(int width) {
  _box.set(Box::Side::ALL, Edge::BORDER, width);
  adjust();
}

void Widget::setBorderColor(uint16_t colour) {
  _box.set(Box::Side::ALL, Edge::COLOR, colour);
  adjust();
}

void Widget::setPadding(Box::Side side, int pixels) {
  _box.set(side, Edge::PADDING, pixels);
  adjust();
}
void Widget::setMargin(Box::Side side, int pixels) {
  _box.set(side, Edge::MARGIN, pixels);
  adjust();
}

void Widget::setBorderWidth(Box::Side side, int width) {
  _box.set(side, Edge::BORDER, width);
  adjust();
}

void Widget::setBorderColor(Box::Side side, uint16_t colour) {
  _box.set(side, Edge::BORDER, colour);
  adjust();
}

void Widget::setBevel(bool bevel) {
  if (_bevel != bevel) {
    _bevel = bevel;
    noteDirty();
  }
}

void Widget::setFont(const GFXfont *font) {
  if (_font != font) {
    _font = font;
    noteDirty();
  }
}

const GFXfont *Widget::getFont() { return _font; }

void Widget::setLabel(const char *label) {
  if (_label != label) {
    _label = label;
    noteDirty();
  }
}

const char *Widget::getLabel() { return _label; }

void Widget::draw(bool force) {
  if (_revealed) force = true;
  if (!_hidden) {
    if (force) _dirtyContent = 0xffff;

    if (force || _dirty) {
      _clear();
      _draw();
      _clearContent(true);
      _drawContent(true);
      _drawChildren(true);
    } else if (_dirtyContent) {
      _clearContent(false);
      _drawContent(false);
      _drawChildren(false);
    } else if (_dirtyChildren) {
      _drawChildren(false);
    }

    _dirty = false;
    _dirtyContent = 0;
    _dirtyChildren = false;
    _revealed = false;
  }
}

void Widget::clear() {
  if (!_hidden) _clear();
}

void Widget::show() {
  if (_hidden) {
    _hidden = false;
    _revealed = true;
    noteDirty();
  }
}

void Widget::hide() {
  if (!_hidden) {
    _hidden = true;
    noteDamage();
  }
}

void Widget::focus() {
  if (!_focus) {
    _focus = true;
    noteDirty();
  }
}

void Widget::unfocus() {
  if (_focus) {
    _focus = false;
    noteDirty();
  }
}

bool Widget::hasFocus() { return _focus; }

bool Widget::isHidden() { return _hidden; }

bool Widget::intersects(Widget *w) { return _outer.intersects(w->_outer); }

void Widget::noteDirty() {
  _dirty = true;
  _dirtyContent = true;
  _dirtyChildren = true;
  if (_parent != nullptr) {
    _parent->noteChildDirty();
  }
}
void Widget::noteDirtyContent(uint16_t zones) {
  _dirtyContent |= zones;
  if (_parent != nullptr) {
    _parent->noteChildDirty();
  }
}

void Widget::noteChildDirty() {
  _dirtyChildren = true;
  if (_parent != nullptr) {
    _parent->noteChildDirty();
  }
}

bool Widget::hasDirtyChildren() { return _dirtyChildren; }
bool Widget::isDirty() { return _dirty; }
bool Widget::isDirtyContent() { return _dirtyContent; }

void Widget::noteDamage() {
  _dirty = true;
  if (_parent != nullptr) {
    _parent->noteDirty();
  }
}

void Widget::adjust() { _adjust(); }

Adafruit_GFX_NG *Widget::display() { return _context->display(); }

DefaultFontRenderer_NG *Widget::fontRenderer() {
  return _context->fontRenderer();
}

/**********************************************************
 * Protected methods
 **********************************************************/
void Widget::_draw() {
  Adafruit_GFX_NG *d = display();
  // Draw the border
  if (_bevel) {
    BorderTool::bevel(d, _outer, _box);
  } else {
    BorderTool::border(d, _outer, _box);
  }
}

void Widget::_drawContent(bool force) {}
void Widget::_drawChildren(bool force){};

void Widget::_clear() {
  display()->fillRect(_outer.x, _outer.y, _outer.width, _outer.height, _bg);
}

void Widget::_clearContent(bool force) {
  if (force || _dirtyContent) {
    // if the parent has just redrawn the background with the same colour, don't
    // repeat that here
    if (_parent != nullptr) {
      if (_parent->_dirty && _parent->_bg == _bg) {
        return;
      }
    }
    display()->fillRect(_inner.x, _inner.y, _inner.width, _inner.height, _bg);
  }
}

void Widget::_adjust() {
  // Recalculate size of inner Rect
  // _outer is the overall size of the widget
  // _inner is the area available for content after margin, border and padding
  _inner.resize(_outer, _box);
  noteDamage();
}
