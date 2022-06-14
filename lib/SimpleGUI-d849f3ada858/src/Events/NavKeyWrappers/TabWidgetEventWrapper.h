#ifndef SIMPLEGUI_NAVKEY_TAB_WIDGET_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_TAB_WIDGET_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/TabWidget.h"

namespace simplegui {

class TabWidgetEventWrapper : public NavKeyEventWrapper<TabWidget> {
 public:
  TabWidgetEventWrapper(TabWidget *tabWidget)
      : NavKeyEventWrapper(tabWidget) {}
  ~TabWidgetEventWrapper() {}

  NavKeyEventWrapperBase *handleEvent(Event e) {
    NavKeyEventWrapperBase *to = this;

    switch (e.type) {
      case NAV_LEFT:
        to = _moveLeft(_wrapped->left());
        break;

      case NAV_RIGHT:
        to = _moveRight(_wrapped->right());
        break;

      case NAV_SELECT:
      case NAV_DOWN:
        // give focus to the selected widget
        _wrapped->focusChild();
        to = _down;
        break;

      case NAV_DEC:
      case NAV_INC:
        // just ignore these
        break;

      case NAV_UP:
      default:
        _onEvent.call(e);
        to = _up;
        break;
    }
    return to;
  };

};

}  // namespace simplegui
#endif