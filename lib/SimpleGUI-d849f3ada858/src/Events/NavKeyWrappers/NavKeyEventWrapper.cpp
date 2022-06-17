#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
  NavKeyEventWrapper::NavKeyEventWrapper(Widget *wrapped) : NavKeyEventWrapperBase() {
    _wrapped = wrapped;
  }

   Widget *NavKeyEventWrapper::wrapped() { return this->_wrapped; }

  // Widgets can re-emit navigation events
  template < class T>
  void NavKeyEventWrapper::onEvent(T *tptr, void (T::*mptr)(Event e)) {
    _onEvent.attach(tptr, mptr);
  }

    void NavKeyEventWrapper::raiseEvent(Event e) {
    _onEvent.call(e);
}
}