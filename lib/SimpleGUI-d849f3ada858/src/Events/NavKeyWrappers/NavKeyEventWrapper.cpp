#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
  template <class W>
  NavKeyEventWrapper<W>::NavKeyEventWrapper(W *wrapped) : NavKeyEventWrapperBase() {
    _wrapped = wrapped;
  }

   template <class W>
   W *NavKeyEventWrapper<W>::wrapped() { return _wrapped; }

  // Widgets can re-emit navigation events
  template <class W>
  template < class T>
  void NavKeyEventWrapper<W>::onEvent(T *tptr, void (T::*mptr)(Event e)) {
    _onEvent.attach(tptr, mptr);
  }

    template <class W>
    void NavKeyEventWrapper<W>::raiseEvent(Event e) {
    _onEvent.call(e);
}
}