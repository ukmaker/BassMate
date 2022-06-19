#ifndef SIMPLEGUI_NAVKEY_SELECT_EVENT_WRAPPER_H
#define SIMPLEGUI_NAVKEY_SELECT_EVENT_WRAPPER_H

#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapper.h"
#include "Widgets/AbstractSelectWidget.h"

namespace simplegui {

/*
 * An interfac definition for classes which understand how
 * NavKey (and keyboard arrows) shouldl interact with a widget
 */
class SelectEventWrapper : public NavKeyEventWrapper {
 public:
  SelectEventWrapper(SelectWidget *select);
  ~SelectEventWrapper();

/**
 * @brief Override
 * 
 * @return AbstractSelectWidget<S>* 
 */
  virtual SelectWidget *wrappedSelect();

  virtual Widget *handleEvent(Event &e);
};

}  // namespace simplegui_navkey
#endif