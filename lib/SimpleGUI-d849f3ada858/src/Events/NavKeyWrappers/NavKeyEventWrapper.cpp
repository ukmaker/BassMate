#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"

namespace simplegui
{

  /*
   * An interfac definition for classes which understand how
   * NavKey (and keyboard arrows) shouldl interact with a widget
   */
  NavKeyEventWrapper::NavKeyEventWrapper(Widget *wrapped)
  {
    _wrapped = wrapped;
    _up = nullptr;
    _down = nullptr;
    _left = nullptr;
    _right = nullptr;
  }

  Widget *NavKeyEventWrapper::wrapped() { return this->_wrapped; }

  /**
   * Handle the event and return the wrapper which is
   * the new focus for events
   */
  void NavKeyEventWrapper::upTo(NavKeyEventWrapper *wrapper) { _up = wrapper; }
  void NavKeyEventWrapper::downTo(NavKeyEventWrapper *wrapper) { _down = wrapper; }
  void NavKeyEventWrapper::leftTo(NavKeyEventWrapper *wrapper) { _left = wrapper; }
  void NavKeyEventWrapper::rightTo(NavKeyEventWrapper *wrapper) { _right = wrapper; }

  /***********************************
   * Protected methods
   ***********************************/

  Widget *NavKeyEventWrapper::_moveUp(bool moved)
  {
    if (moved && (_up != nullptr))
    {
      return _up->wrapped();
    }
    return this->wrapped();
  }

  Widget *NavKeyEventWrapper::_moveDown(bool moved)
  {
    if (moved && (_down != nullptr))
    {
      return _down->wrapped();
    }
    return this->wrapped();
  }

  Widget *NavKeyEventWrapper::_moveLeft(bool moved)
  {
    if (moved && (_left != nullptr))
    {
      return _left->wrapped();
    }
    return this->wrapped();
  }

  Widget *NavKeyEventWrapper::_moveRight(bool moved)
  {
    if (moved && (_right != nullptr))
    {
      return _right->wrapped();
    }
    return this->wrapped();
  }
}