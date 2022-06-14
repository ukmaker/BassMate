#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPINGS_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPINGS_H

#include "Core/LinkedList.h"
#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/ContextWindowEventWrapper.h"
#include "Events/NavKeyWrappers/KeyboardEventWrapper.h"
#include "Events/NavKeyWrappers/SelectEventWrapper.h"
#include "Events/NavKeyWrappers/TabWidgetEventWrapper.h"
#include "Events/NavKeyWrappers/NavKeyEventWrapperBase.h"
#include "Widgets/AbstractSelectWidget.h"

namespace simplegui
{

  class NavKeyEventWrapperBase; // forward declaration

  class Wrapped
  {
  public:
    Wrapped(NavKeyEventWrapperBase *n, Widget *w);
    NavKeyEventWrapperBase *_n;
    Widget *_w;
  };

  class Wrappings
  {
  public:

      Wrappings() {}
    ~Wrappings() {}


     void focus(NavKeyEventWrapperBase *wrapper);
     NavKeyEventWrapperBase *focus(Widget *w);
     NavKeyEventWrapperBase *wrapperFor(Widget *w);

     void handleEvent(Event e);

    template <class S>
     SelectEventWrapper<S> *wrap(AbstractSelectWidget<S> *select);

     KeyboardEventWrapper *wrap(KeyboardWidget *keyboard);

     ContextWindowEventWrapper *wrap(ContextWindow *window);

     TabWidgetEventWrapper *wrap(TabWidget *tabWidget);

  private:

    
     NavKeyEventWrapperBase *_focus;
    
     LinkedList<Wrapped> _wrappers;

    
     void _append(NavKeyEventWrapperBase *wrapper, Widget *widget);
  };

} // namespace simplegui

#endif