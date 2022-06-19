#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H

#include "Widgets/Widget.h"
#include "Events/EventModel.h"

namespace simplegui
{

  /*
   * An interfac definition for classes which understand how
   * NavKey (and keyboard arrows) shouldl interact with a widget
   */
  class NavKeyEventWrapper
  {
  public:
    NavKeyEventWrapper(Widget *wrapped);
    ~NavKeyEventWrapper() {}

    virtual Widget *wrapped();

    /**
     * Handle the event and return the widget which is
     * the new focus for events
     * 
     * If the event has been successfully handle then its cancelled property
     * should be set to false. Otherwise it should be forwarded to the new focus
     */
    virtual Widget *handleEvent(Event &e) = 0;
    virtual void upTo(NavKeyEventWrapper *wrapper);
    virtual void downTo(NavKeyEventWrapper *wrapper);
    virtual void leftTo(NavKeyEventWrapper *wrapper);
    virtual void rightTo(NavKeyEventWrapper *wrapper);

  protected:
    NavKeyEventWrapper *_up;
    NavKeyEventWrapper *_down;
    NavKeyEventWrapper *_left;
    NavKeyEventWrapper *_right;

    Widget *_moveUp(bool moved);
    Widget *_moveDown(bool moved);
    Widget *_moveLeft(bool moved);
    Widget *_moveRight(bool moved);

    Widget *_wrapped;
  };

} // namespace simplegui
#endif