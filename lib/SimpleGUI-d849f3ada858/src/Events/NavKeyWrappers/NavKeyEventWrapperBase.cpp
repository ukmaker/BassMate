#include "Events/NavKeyWrappers/NavKeyEventWrapperBase.h"


namespace simplegui
{

  NavKeyEventWrapperBase::NavKeyEventWrapperBase()
  {
    _up = nullptr;
    _down = nullptr;
    _left = nullptr;
    _right = nullptr;
  }

  NavKeyEventWrapperBase::~NavKeyEventWrapperBase() {}

  /**
   * Handle the event and return the wrapper which is
   * the new focus for events
   */
  void NavKeyEventWrapperBase::upTo(NavKeyEventWrapperBase *wrapper) { _up = wrapper; }
  void NavKeyEventWrapperBase::downTo(NavKeyEventWrapperBase *wrapper) { _down = wrapper; }
  void NavKeyEventWrapperBase::leftTo(NavKeyEventWrapperBase *wrapper) { _left = wrapper; }
  void NavKeyEventWrapperBase::rightTo(NavKeyEventWrapperBase *wrapper) { _right = wrapper; }

  /***********************************
   * Protected methods
   ***********************************/

  NavKeyEventWrapperBase *NavKeyEventWrapperBase::_moveUp(bool moved)
  {
    if (moved && (_up != nullptr))
    {
      return _up;
    }
    return this;
  }

  NavKeyEventWrapperBase *NavKeyEventWrapperBase::_moveDown(bool moved)
  {
    if (moved && (_down != nullptr))
    {
      return _down;
    }
    return this;
  }

  NavKeyEventWrapperBase *NavKeyEventWrapperBase::_moveLeft(bool moved)
  {
    if (moved && (_left != nullptr))
    {
      return _left;
    }
    return this;
  }

  NavKeyEventWrapperBase *NavKeyEventWrapperBase::_moveRight(bool moved)
  {
    if (moved && (_right != nullptr))
    {
      return _right;
    }
    return this;
  }


} // namespace simplegui
