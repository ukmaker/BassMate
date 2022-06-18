#ifndef SIMPLEGUI_NAVKEY_CONTEXT_WINDOW_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_CONTEXT_WINDOW_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/ContextWindow.h"

namespace simplegui
{

  class ContextWindowEventWrapper : public NavKeyEventWrapper
  {
  public:
    ContextWindowEventWrapper(ContextWindow *window)
        : NavKeyEventWrapper(window) {}
    ~ContextWindowEventWrapper() {}

    ContextWindow *wrappedContextWindow()
    {
      return static_cast<ContextWindow *>(this->_wrapped);
    }

    virtual NavKeyEventWrapper *handleEvent(Event e)
    {

      NavKeyEventWrapper *to = this;

      switch (e.type)
      {
      case NAV_DEC:
      case NAV_LEFT:
        wrappedContextWindow()->left();
        break;

      case NAV_INC:
      case NAV_RIGHT:
        wrappedContextWindow()->right();
        break;

      case NAV_SELECT:
      case NAV_DOWN:
        // give focus to the selected widget
        wrappedContextWindow()->focusChild();
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

} // namespace simplegui_navkey
#endif