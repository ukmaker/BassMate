#ifndef SIMPLEGUI_NAVKEY_CONTEXT_WINDOW_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_CONTEXT_WINDOW_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/ContextWindow.h"

namespace simplegui {

class ContextWindowEventWrapper : public NavKeyEventWrapper<ContextWindow> {
 public:
  ContextWindowEventWrapper(ContextWindow *window)
      : NavKeyEventWrapper(window) {}
  ~ContextWindowEventWrapper() {}

  virtual NavKeyEventWrapperBase *handleEvent(Event e) {

    NavKeyEventWrapperBase *to = this;

    switch (e.type) {
      case NAV_DEC:
      case NAV_LEFT:
        _wrapped->left();
        break;

      case NAV_INC:
      case NAV_RIGHT:
        _wrapped->right();
        break;

      case NAV_SELECT:
      case NAV_DOWN:
        // give focus to the selected widget
        _wrapped->focusChild();
        break;

      case NAV_UP:
       to = _moveUp(true);
       break;

      default:
        _onEvent.call(e);
        break;
    }

    return to;
  };

};

}  // namespace simplegui_navkey
#endif