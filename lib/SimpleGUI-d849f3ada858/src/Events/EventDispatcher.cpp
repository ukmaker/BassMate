#include "Events/EventSystem.h"

EventDispatcher::EventDispatcher() {
}

void EventDispatcher::attachListener(EventListener* l) {
    _listeners.appendOnce(l);
}

void EventDispatcher::detachListener(EventListener* l) {
    _listeners.remove(l);
}

void EventDispatcher::dispatchEvent(Event e) {
    EventListener* l;
    _listeners.reset();
    while((l=_listeners.next()) != NULL) {
        l->handleEvent(e);
    }
}

void EventDispatcher::queueEvent(const Event e)
{
    _mailbox.push(e);
}

void EventDispatcher::pumpEvents()
{
    Event e = _mailbox.pull();
    if(e.type != EventType::NULL_EVENT) {
        dispatchEvent(e);
    }
}
