#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapperBase.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
template <class W>
class NavKeyEventWrapper : public NavKeyEventWrapperBase {
 public:
  NavKeyEventWrapper(W *wrapped);
  ~NavKeyEventWrapper() {}

  virtual W *wrapped();

  // Widgets can re-emit navigation events
  template <class T>
  void onEvent(T *tptr, void (T::*mptr)(Event e)) ;

  void raiseEvent(Event e) ;

 protected:
  FunctionPointerArg1<void, Event> _onEvent;
  W *_wrapped;
};

}  // namespace simplegui
#endif