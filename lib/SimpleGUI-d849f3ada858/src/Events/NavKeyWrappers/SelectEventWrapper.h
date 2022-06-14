#ifndef SIMPLEGUI_NAVKEY_SELECT_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_SELECT_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/AbstractSelectWidget.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
template <class S>
class SelectEventWrapper : public NavKeyEventWrapper<AbstractSelectWidget<S>> {
 public:
  SelectEventWrapper(AbstractSelectWidget<S> *select)
      : NavKeyEventWrapper<AbstractSelectWidget<S>>(select) {}
  ~SelectEventWrapper() {}

  NavKeyEventWrapperBase *handleEvent(Event e) {

    NavKeyEventWrapperBase *to = this;

    switch (e.type) {

      case NAV_DEC:
        this->wrapped()->scrollDown();
        break;

      case NAV_INC:
        this->wrapped()->scrollUp();
        break;

      case NAV_UP:
        // If not at the top, jump to the top
        // If already at the top, emit the event so it can be used for
        // navigation
        if (this->wrapped()->getSelectedIndex() == 0) {
          this->raiseEvent(e);
          to = this->_moveUp(true);
        } else {
          this->wrapped()->scrollToTop();
        }
        break;

      case NAV_DOWN:
        if (this->wrapped()->getSelectedIndex() == (this->wrapped()->getNumChoices() - 1)) {
          this->raiseEvent(e);
          to = this->_moveDown(true);
        } else {
          this->wrapped()->scrollDown();
        }
        break;

      case NAV_SELECT:
        this->wrapped()->selectCurrent();
        break;

      case NAV_LEFT:
        to = this->_moveLeft(true);
        break;

      case NAV_RIGHT:
        to = this->_moveRIght(true);
        break;

      default:
        this->raiseEvent(e);
        break;
    }

    return to;
  };
};

}  // namespace simplegui_navkey
#endif