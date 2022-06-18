#ifndef SIMPLEGUI_WINDOW_H
#define SIMPLEGUI_WINDOW_H

/**
 * Defines an interface for classes which can contain widgets
 **/

class Window;

#include "Core/GraphicsContext.h"
#include "Core/LinkedList.h"
#include "Widgets/Widget.h"

class Window : public Widget {
 public:
  enum Layout {
    VERTICAL_LEFT,
    VERTICAL_RIGHT,
    VERTICAL_CENTER,
    HORIZONTAL,
    FIXED
  };

  Window(GraphicsContext *context);

  /**
   * We are going to override these Widget methods
   **/
  virtual void attach(Widget *widget);
  virtual void detach(Widget *widget);

  void setLayout(Layout l);

 protected:
  Layout _layout;
  LinkedList<Widget> _widgets;

  virtual void _adjust();
  virtual void _drawChildren(bool force = false);
};

#endif