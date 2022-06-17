#ifndef SIMPLEGUI_NAVKEY_SELECT_EVENT_DISPATCHER_H
#define SIMPLEGUI_NAVKEY_SELECT_EVENT_DISPATCHER_H

#include "Widgets/AbstractSelectWidget.h"
#include "i2cNavKey.h"

namespace simplegui
{

  class NavKeyEventDispatcher
  {
  public:

    NavKeyEventDispatcher(EventListener *listener) { _listener = listener; }

    void dispatch(Event &e)
    {
      if(_listener != nullptr) {
        _listener->handleEvent(e);
      }
    }

    void UP_Button_Pressed(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_UP;
      dispatch(e);
    }
    void DOWN_Button_Pressed(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_DOWN;
      dispatch(e);
    }
    void LEFT_Button_Pressed(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_LEFT;
      dispatch(e);
    }
    void RIGHT_Button_Pressed(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_RIGHT;
      dispatch(e);
    }

    void Encoder_Increment(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_INC;
      dispatch(e);
    }
    void Encoder_Decrement(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_DEC;
      dispatch(e);
    }
    void Encoder_Push(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_SELECT;
      dispatch(e);
    }
    void Encoder_Release(i2cNavKey *p)
    {
      Event e;
      e.type = EventType::NAV_RELEASE;
      dispatch(e);
    }

  protected:
    EventListener *_listener;
  };
} // namespace simplegui
#endif