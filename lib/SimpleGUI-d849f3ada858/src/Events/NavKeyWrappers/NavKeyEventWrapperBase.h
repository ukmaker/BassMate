#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPER_BASE_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPER_BASE_H

#include "Events/EventModel.h"

namespace simplegui
{


  /*
   * An interfac definition for classes which understand how
   * NavKey (and keyboard arrows) shouldl interact with a widget
   */
  class NavKeyEventWrapperBase
  {
  public:
    NavKeyEventWrapperBase();
    ~NavKeyEventWrapperBase();

    /**
     * Handle the event and return the wrapper which is
     * the new focus for events
     */
    virtual NavKeyEventWrapperBase *handleEvent(Event e) = 0;
    virtual void upTo(NavKeyEventWrapperBase *wrapper);
    virtual void downTo(NavKeyEventWrapperBase *wrapper);
    virtual void leftTo(NavKeyEventWrapperBase *wrapper);
    virtual void rightTo(NavKeyEventWrapperBase *wrapper);

  protected:
    NavKeyEventWrapperBase *_up;
    NavKeyEventWrapperBase *_down;
    NavKeyEventWrapperBase *_left;
    NavKeyEventWrapperBase *_right;

    NavKeyEventWrapperBase *_moveUp(bool moved);

    NavKeyEventWrapperBase *_moveDown(bool moved);

    NavKeyEventWrapperBase *_moveLeft(bool moved);

    NavKeyEventWrapperBase *_moveRight(bool moved);

  };

} // namespace simplegui
#endif