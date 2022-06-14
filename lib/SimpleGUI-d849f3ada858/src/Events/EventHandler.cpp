#include "Events/EventSystem.h"

EventHandler::EventHandler(EventType eventType, EventHandlerFunction fn)
    : type(eventType) {
  _fp.attach(fn);
}

template <typename T>
EventHandler::EventHandler(EventType eventType, T* tptr, void (T::*mptr)(Event))
    : type(eventType) {
  _fp.attach(tptr, mptr);
}

void EventHandler::handle(Event e) { _fp.call(e); }
