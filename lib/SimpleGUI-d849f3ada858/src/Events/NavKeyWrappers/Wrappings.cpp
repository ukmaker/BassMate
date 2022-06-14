#include "Events/NavKeyWrappers/Wrappings.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapperBase.h"

namespace simplegui {

    Wrapped::Wrapped(NavKeyEventWrapperBase *n, Widget *w) {
      _n = n;
      _w = w;
    }


   void Wrappings::focus(NavKeyEventWrapperBase *wrapper) { _focus = wrapper; }

   NavKeyEventWrapperBase *Wrappings::focus(Widget *w) {
    Wrapped *wrapped;
    _wrappers.reset();
    while (wrapped = _wrappers.next()) {
      if (wrapped->_w == w) {
        focus(wrapped->_w);
        return wrapped->_n;
      }
    }
    return nullptr;
  }

   NavKeyEventWrapperBase *Wrappings::wrapperFor(Widget *w) {
    Wrapped *wrapped;
    _wrappers.reset();
    while (wrapped = _wrappers.next()) {
      if (wrapped->_w == w) {
        return wrapped->_n;
      }
    }
    return nullptr;
  }

   void Wrappings::handleEvent(Event e) {
    if (_focus != nullptr) {
      _focus->handleEvent(e);
    }
  }

  template <class S>
   SelectEventWrapper<S> *Wrappings::wrap(AbstractSelectWidget<S> *select) {
    SelectEventWrapper<S> *w = new SelectEventWrapper<S>(select);
    _append(w, select);
    return w;
  }

   KeyboardEventWrapper *Wrappings::wrap(KeyboardWidget *keyboard) {
    KeyboardEventWrapper *w = new KeyboardEventWrapper(keyboard);
    _append(w, keyboard);
    return w;
  }

   ContextWindowEventWrapper *Wrappings::wrap(ContextWindow *window) {
    ContextWindowEventWrapper *w = new ContextWindowEventWrapper(window);
    _append(w, window);
    return w;
  }

   TabWidgetEventWrapper *Wrappings::wrap(TabWidget *tabWidget) {
    TabWidgetEventWrapper *w = new TabWidgetEventWrapper(tabWidget);
    _append(w, tabWidget);
    return w;
  }

   void Wrappings::_append(NavKeyEventWrapperBase *wrapper, Widget *widget) {
      Wrapped *w = new Wrapped(wrapper, widget);
      _wrappers.append(w);
  }


}  // namespace simplegui
