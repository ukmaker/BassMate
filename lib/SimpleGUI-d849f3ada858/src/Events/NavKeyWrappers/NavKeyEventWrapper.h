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

    // Widgets can re-emit navigation events
    template <class T>
    void onEvent(T *tptr, void (T::*mptr)(Event e));

    void raiseEvent(Event e);

    /**
     * Handle the event and return the wrapper which is
     * the new focus for events
     */
    virtual NavKeyEventWrapper *handleEvent(Event e) = 0;
    virtual void upTo(NavKeyEventWrapper *wrapper);
    virtual void downTo(NavKeyEventWrapper *wrapper);
    virtual void leftTo(NavKeyEventWrapper *wrapper);
    virtual void rightTo(NavKeyEventWrapper *wrapper);

  protected:
    NavKeyEventWrapper *_up;
    NavKeyEventWrapper *_down;
    NavKeyEventWrapper *_left;
    NavKeyEventWrapper *_right;

    NavKeyEventWrapper *_moveUp(bool moved);
    NavKeyEventWrapper *_moveDown(bool moved);
    NavKeyEventWrapper *_moveLeft(bool moved);
    NavKeyEventWrapper *_moveRight(bool moved);

    FunctionPointerArg1<void, Event> _onEvent;
    Widget *_wrapped;
  };

} // namespace simplegui
#endif