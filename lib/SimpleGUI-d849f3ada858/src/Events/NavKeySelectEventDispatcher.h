#ifndef SIMPLEGUI_NAVKEY_SELECT_EVENT_DISPATCHER_H
#define SIMPLEGUI_NAVKEY_SELECT_EVENT_DISPATCHER_H

#include "Widgets/AbstractSelectWidget.h"

namespace simplegui {

template <class V>
class NavKeySelectEventDispatcher {
 public:
  NavKeySelectEventDispatcher(AbstractSelectWidget<V> *w) { _select(w) }

  void dispatch(Event &e) {
      
  }

 protected:
  AbstractSelectWidget<V> *_select;
};
}  // namespace simplegui
#endif