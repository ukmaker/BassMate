#ifndef SIMPLEGUI_NAVKEY_KEYBOARD_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_KEYBOARD_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/KeyboardWidget.h"

namespace simplegui {

class KeyboardEventWrapper : public NavKeyEventWrapper<KeyboardWidget> {
 public:
  KeyboardEventWrapper(KeyboardWidget *keyboard)
      : NavKeyEventWrapper(keyboard) {}
  ~KeyboardEventWrapper() {}

  NavKeyEventWrapperBase *handleEvent(Event e) {
    NavKeyEventWrapperBase *to = this;
    switch (e.type) {
      case NAV_DEC:
        _wrapped->forward();
        break;

      case NAV_INC:
        _wrapped->back();
        break;

      case NAV_UP:
        to = _moveUp(_wrapped->up());
        break;

      case NAV_DOWN:
        to = _moveDown(_wrapped->down());
        break;

      case NAV_SELECT:
        _wrapped->select();
        break;

      case NAV_LEFT:
        to = _moveLeft(_wrapped->left());
        break;

      case NAV_RIGHT:
        to = _moveRight(_wrapped->right());
        break;

      default:
        _onEvent.call(e);
        break;
    }

    return to;
  };
};
}  // namespace simplegui
#endif