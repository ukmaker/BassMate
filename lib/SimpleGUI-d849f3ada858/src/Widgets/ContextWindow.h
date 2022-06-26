#ifndef SIMPLEGUI_CONTEXT_WINDOW_H
#define SIMPLEGUI_CONTEXT_WINDOW_H
/**
 * A Window version which shows a title bar with
 * left and right arrows.
 * Only one child may be active at a time
 * Arrows point left and right to indicate
 * if there are more children to be navigated to
 */

#include "Core/ColorsRGB16.h"
#include "Window.h"

class ContextWindow : public Window
{
public:
  static const uint16_t ZONE_TITLE = 1;
  static const uint16_t ZONE_CONTENT = 2;

  ContextWindow(GraphicsContext *context) : Window(context)
  {
    _idx = 0;
    _titleBarBG = WHITE;
    _titleBarFG = BLACK;
    _selected = nullptr;
  }

  ~ContextWindow() {}

  void setTitleBarHeight(uint16_t height) { _titleBarHeight = height; }

  void setTitleBarForeground(uint16_t color)
  {
    if (_titleBarFG != color)
    {
      _titleBarFG = color;
      noteDirtyContent(ZONE_TITLE);
    }
  }

  void setTitleBarBackground(uint16_t color)
  {
    if (_titleBarBG != color)
    {
      _titleBarBG = color;
      noteDirtyContent(ZONE_TITLE);
    }
  }

  void select(uint8_t idx)
  {
    if (idx != _idx || _selected == nullptr)
    {
      _moveFocus(idx);
      noteDirtyContent(ZONE_TITLE | ZONE_CONTENT);
    }
  }

  void selectWidget(Widget *widget)
  {
    if (_widgets.contains(widget))
    {
      uint16_t idx = _widgets.indexOf(widget);
      select(idx);
    }
  }

  void left()
  {
    if (_idx > 0)
    {
      select(_idx - 1);
    }
  }

  void right()
  {
    if ((_idx + 1) < _widgets.size())
    {
      select(_idx + 1);
    }
  }

  Widget *focusChild()
  {
    if(_selected == nullptr) {
      _moveFocus(_idx);
    }

    if (_selected != nullptr)
    {
      blur();
      _selected->focus();
    }
    return _selected;
  }

  void unfocusChild()
  {
    if(_selected == nullptr) {
      _moveFocus(_idx);
    }
    
    if (_selected != nullptr)
    {
      focus();
      _selected->blur();
    }
  }

protected:
  Widget *_selected;

  virtual void _moveFocus(uint8_t to)
  {
    if (_selected != nullptr)
    {
      _selected->hide();
    }

    _selected = _widgets.getByIndex((uint16_t)to);
    if (_selected != nullptr)
      _selected->show();
    _idx = to;
  }

  virtual void _clearContent(bool force)
  {
    uint16_t fg, bg;

    if (hasFocus())
    {
      fg = _titleBarFG;
      bg = _titleBarBG;
    }
    else
    {
      fg = _fg;
      bg = _bg;
    }
    if (force || _dirtyContent & ZONE_TITLE)
    {
      display()->fillRect(_inner.top(), _inner.left(), _inner.width(),
                          _titleBarHeight, bg);
    }
  }

  virtual void _drawContent(bool force)
  {
    // Title first
    bool l, r;
    if (_idx > 0)
    {
      l = true;
    }
    else
    {
      l = false;
    }

    if ((_idx + 1) < _widgets.size())
    {
      r = true;
    }
    else
    {
      r = false;
    }

    uint16_t fg, bg;

    if (hasFocus())
    {
      fg = _titleBarFG;
      bg = _titleBarBG;
    }
    else
    {
      fg = _fg;
      bg = _bg;
    }

    // little scrolling triangles
    uint16_t third = (_titleBarHeight / 3);
    uint16_t trim = _inner.top() + _titleBarHeight / 2 - 1;
    uint16_t trib = _inner.top() + 2 * third;
    uint16_t trit = _inner.top() + third;
    uint16_t triw = (7 * (_titleBarHeight - 2)) / 10;
    if (l)
    {
      display()->fillTriangle(_inner.left() + 1, trim, _inner.left() + triw,
                              trib, _inner.left() + triw, trit, fg);
    }
    if (r)
    {
      display()->fillTriangle(_inner.right() - 1, trim, _inner.right() - triw,
                              trib, _inner.right() - triw, trit, fg);
    }
    fontRenderer()->setFont(getFont());
    fontRenderer()->setTextWindow(_inner.left() + triw + 4, _inner.top() + 1,
                                  _inner.width() - 2 * triw - 8,
                                  _titleBarHeight - 2);
    fontRenderer()->setTextColor(fg, bg);
    Widget *widget = _widgets.getByIndex((uint16_t)_idx);
    if (widget != nullptr)
    {
      display()->print(widget->getLabel());
    }
  }

  virtual void _drawChildren(bool force = false)
  { // Override to only show one
    Widget *widget = _widgets.getByIndex((uint16_t)_idx);
    if (widget != nullptr)
    {
      widget->draw(force);
    }
  }

  uint16_t _titleBarHeight;
  uint16_t _titleBarBG;
  uint16_t _titleBarFG;
  uint8_t _idx;
};
#endif