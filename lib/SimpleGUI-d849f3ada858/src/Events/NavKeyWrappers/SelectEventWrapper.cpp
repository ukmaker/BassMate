#include "Events/NavKeyWrappers/SelectEventWrapper.h"

namespace simplegui
{

  SelectEventWrapper::SelectEventWrapper(SelectWidget *select)
      : NavKeyEventWrapper(select) {}

  SelectEventWrapper::~SelectEventWrapper() {}

  SelectWidget *SelectEventWrapper::wrappedSelect()
  {
    return static_cast<SelectWidget*> (this->_wrapped);
  }

  NavKeyEventWrapperBase *SelectEventWrapper::handleEvent(Event e)
  {

    NavKeyEventWrapperBase *to = this;

    switch (e.type)
    {

    case NAV_DEC:
      this->wrappedSelect()->scrollDown();
      break;

    case NAV_INC:
      this->wrappedSelect()->scrollUp();
      break;

    case NAV_UP:
      // If not at the top, jump to the top
      // If already at the top, emit the event so it can be used for
      // navigation
      if (this->wrappedSelect()->getSelectedIndex() == 0)
      {
        this->raiseEvent(e);
        to = this->_moveUp(true);
      }
      else
      {
        this->wrappedSelect()->scrollToTop();
      }
      break;

    case NAV_DOWN:
      if (this->wrappedSelect()->getSelectedIndex() == (this->wrappedSelect()->getNumChoices() - 1))
      {
        this->raiseEvent(e);
        to = this->_moveDown(true);
      }
      else
      {
        this->wrappedSelect()->scrollDown();
      }
      break;

    case NAV_SELECT:
      this->wrappedSelect()->selectCurrent();
      break;

    case NAV_LEFT:
      to = this->_moveLeft(true);
      break;

    case NAV_RIGHT:
      to = this->_moveRight(true);
      break;

    default:
      this->raiseEvent(e);
      break;
    }

    return to;
  }

} // namespace simplegui_navkey
