#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H

#include "Widgets/Widget.h"
#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapperBase.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
class NavKeyEventWrapper : public NavKeyEventWrapperBase {
 public:
  NavKeyEventWrapper(Widget *wrapped);
  ~NavKeyEventWrapper() {}

  virtual Widget *wrapped();

  // Widgets can re-emit navigation events
  template <class T>
  void onEvent(T *tptr, void (T::*mptr)(Event e)) ;

  void raiseEvent(Event e) ;

 protected:
  FunctionPointerArg1<void, Event> _onEvent;
  Widget *_wrapped;
};

}  // namespace simplegui
#endif