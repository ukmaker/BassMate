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

  // Widgets can re-emit navigation events
  template <class T>
  void NavKeyEventWrapper::onEvent(T *tptr, void (T::*mptr)(Event e))
  {
    _onEvent.attach(tptr, mptr);
  }

  void NavKeyEventWrapper::raiseEvent(Event e)
  {
    _onEvent.call(e);
  }

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

  NavKeyEventWrapper *NavKeyEventWrapper::_moveUp(bool moved)
  {
    if (moved && (_up != nullptr))
    {
      return _up;
    }
    return this;
  }

  NavKeyEventWrapper *NavKeyEventWrapper::_moveDown(bool moved)
  {
    if (moved && (_down != nullptr))
    {
      return _down;
    }
    return this;
  }

  NavKeyEventWrapper *NavKeyEventWrapper::_moveLeft(bool moved)
  {
    if (moved && (_left != nullptr))
    {
      return _left;
    }
    return this;
  }

  NavKeyEventWrapper *NavKeyEventWrapper::_moveRight(bool moved)
  {
    if (moved && (_right != nullptr))
    {
      return _right;
    }
    return this;
  }
}