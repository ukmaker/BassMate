#ifndef SIMPLEGUI_NAVKEY_KEYBOARD_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_KEYBOARD_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/KeyboardWidget.h"

namespace simplegui
{

  class KeyboardEventWrapper : public NavKeyEventWrapper
  {
  public:
    KeyboardEventWrapper(KeyboardWidget *keyboard)
        : NavKeyEventWrapper(keyboard) {}
    ~KeyboardEventWrapper() {}

    KeyboardWidget *wrappedKeyboardWidget()
    {
      return static_cast<KeyboardWidget *>(this->_wrapped);
    }

    Widget *handleEvent(Event &e)
    {
      Widget *to = this->wrapped();
      switch (e.type)
      {
      case NAV_DEC:
        e.cancelled = true;
        wrappedKeyboardWidget()->forward();
        break;

      case NAV_INC:
        e.cancelled = true;
        wrappedKeyboardWidget()->back();
        break;

      case NAV_UP:
        to = _moveUp(wrappedKeyboardWidget()->up());
        break;

      case NAV_DOWN:
        to = _moveDown(wrappedKeyboardWidget()->down());
        break;

      case NAV_SELECT:
        e.cancelled = true;
        wrappedKeyboardWidget()->select();
        break;

      case NAV_LEFT:
        to = _moveLeft(wrappedKeyboardWidget()->left());
        break;

      case NAV_RIGHT:
        to = _moveRight(wrappedKeyboardWidget()->right());
        break;

      default:
        break;
      }

      return to;
    };
  };
} // namespace simplegui
#endif