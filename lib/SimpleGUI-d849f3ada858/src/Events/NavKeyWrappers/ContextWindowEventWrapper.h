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

    virtual Widget *handleEvent(Event &e)
    {

      Widget *to = wrappedContextWindow();

      switch (e.type)
      {
      case NAV_DEC:
      case NAV_LEFT:
        e.cancelled = true;
      wrappedContextWindow()->left();
        break;

      case NAV_INC:
      case NAV_RIGHT:
      e.cancelled = true;
        wrappedContextWindow()->right();
        break;

      case NAV_SELECT:
      case NAV_DOWN:
        // give focus to the selected widget
        to = wrappedContextWindow()->focusChild();
        break;

      case NAV_UP:
        to = _moveUp(true);
        break;

      default:
        break;
      }

      return to;
    };
  };

} // namespace simplegui_navkey
#endif