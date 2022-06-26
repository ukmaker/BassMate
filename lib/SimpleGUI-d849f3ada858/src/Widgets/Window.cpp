#include "Widgets/Window.h"

Window::Window(GraphicsContext *context) : Widget(context) {
  setSize(context->display()->width(), context->display()->height());
  setLocation(0, 0);
  setLayout(FIXED);
}

void Window::setLayout(Layout l) {
  if (_layout != l) {
    _layout = l;
    noteDirty();
  }
}

void Window::attach(Widget *widget) {
  _widgets.append(widget);
  widget->setParent(this);
  widget->show();
  adjust();
}

void Window::detach(Widget *widget) {
  widget->hide();
  _widgets.remove(widget);
  widget->setParent(nullptr);
}

void Window::_drawChildren(bool force) {

  // draw the kids only if they want to
  Widget *w;
  _widgets.reset();
  while ((w = _widgets.next()) != nullptr) {
    w->draw(force);
  }
}

void Window::_adjust() {
  Widget::_adjust();

  int wx = _inner.x();
  int wy = _inner.y();
  int width = 0;
  int height = 0;

  Widget *w;

  _widgets.reset();

  while ((w = _widgets.next()) != NULL) {
    if (_layout == FIXED) {
      w->setOffset(wx, wy);
    } else {
      // Position the widgets
      w->setLocation(wx, wy);

      if (_layout == HORIZONTAL) {
        wx += w->width();
      } else {
        wy += w->height();
      }
    }
  }
}
