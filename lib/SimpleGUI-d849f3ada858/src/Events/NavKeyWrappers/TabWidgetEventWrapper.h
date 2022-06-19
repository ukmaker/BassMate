#ifndef SIMPLEGUI_NAVKEY_TAB_WIDGET_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_TAB_WIDGET_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/TabWidget.h"

namespace simplegui
{

  class TabWidgetEventWrapper : public NavKeyEventWrapper
  {
  public:
    TabWidgetEventWrapper(TabWidget *tabWidget)
        : NavKeyEventWrapper(tabWidget) {}
    ~TabWidgetEventWrapper() {}

    TabWidget *wrappedTabWidget()
    {
      return static_cast<TabWidget *>(this->_wrapped);
    }

    Widget *handleEvent(Event &e)
    {

      Widget *to = this->wrapped();

      switch (e.type)
      {
      case NAV_LEFT:
        e.cancelled = true;
        _moveLeft(wrappedTabWidget()->left());
        break;

      case NAV_RIGHT:
        e.cancelled = true;
        to = _moveRight(wrappedTabWidget()->right());
        break;

      case NAV_SELECT:
      case NAV_DOWN:
        // give focus to the selected widget
        e.cancelled = true;
        to = wrappedTabWidget()->focusChild();
        break;

      case NAV_DEC:
      case NAV_INC:
        e.cancelled = true;
        // just ignore these
        break;

      case NAV_UP:
      default:
        e.cancelled = true;
        to = _up->wrapped();
        break;
      }
      return to;
    };
  };

} // namespace simplegui
#endif