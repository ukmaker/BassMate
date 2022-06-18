#ifndef SIMPLEGUI_NAVKEY_EVENT_WRAPPINGS_H
#define SIMPLEGUI_NAVKEY_EVENT_WRAPPINGS_H

#include "Core/LinkedList.h"
#include "Events/EventModel.h"
#include "Events/NavKeyWrappers/ContextWindowEventWrapper.h"
#include "Events/NavKeyWrappers/KeyboardEventWrapper.h"
#include "Events/NavKeyWrappers/SelectEventWrapper.h"
#include "Events/NavKeyWrappers/TabWidgetEventWrapper.h"
#include "Widgets/AbstractSelectWidget.h"

namespace simplegui
{

  class NavKeyEventWrapper; // forward declaration

  class Wrappings : public EventListener
  {
  public:
    Wrappings() {}
    ~Wrappings() {}

    void focusWrapper(NavKeyEventWrapper *wrapper);
    NavKeyEventWrapper *focus(Widget *w);
    NavKeyEventWrapper *wrapperFor(Widget *w);

    virtual void handleEvent(Event e);
    virtual void setEventHandler(EventHandler *handler) {}
    virtual void unsetEventHandler(EventHandler *handler) {}

    SelectEventWrapper *wrapSelect(SelectWidget *select);

    KeyboardEventWrapper *wrap(KeyboardWidget *keyboard);

    ContextWindowEventWrapper *wrap(ContextWindow *window);

    TabWidgetEventWrapper *wrap(TabWidget *tabWidget);

    void append(NavKeyEventWrapper *wrapper);

  private:
    NavKeyEventWrapper *_focus;

    LinkedList<NavKeyEventWrapper> _wrappers;
  };

} // namespace simplegui

#endif