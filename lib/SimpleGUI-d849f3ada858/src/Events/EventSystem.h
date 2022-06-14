#ifndef SIMPLEGUI_EVENTSYSTEM_H
#define SIMPLEGUI_EVENTSYSTEM_H

#include <Arduino.h>

#include "Core/FunctionPointer.h"
#include "Core/LinkedList.h"
#include "Events/EventType.h"

/**
 * Bung all the event definitions in one file so we can sort out the forward
 * definitions
 */
class Event;
class EventListener;
class EventSource;
class EventHandler;
class EventDispatcher;

class Event {
 public:
  EventListener *target;
  EventType type;
  EventSource *source;
  // This should probably be some kind of union to cope with the different kinds
  // of event we expect to handle For now I'm going to explicitly assume events
  // relate to something on-screen
  int screenX;
  int screenY;
};

template <size_t capacity>
class EventQueue {
 public:
  EventQueue() : size(capacity + 1), write(0), read(0) {}

  bool push(Event e) {
    if (!full()) {
      events[write++] = e;

      if (write >= size) {
        write -= size;
      }
      return true;
    }
    return false;
  }

  Event pull() {
    if (!empty()) {
      Event e = events[read++];
      if (read >= size) read -= size;
      return e;
    }
    Event e;
    e.type = EventType::NULL_EVENT;
    return e;
  }

  bool full() { return getNumEvents() == size - 1; }

  bool empty() { return read == write; }

  size_t getNumEvents() {
    int16_t n = write - read;
    if (n < 0) n += size;
    return (size_t)n;
  }

  size_t getCapacity() { return size - 1; }

 private:
  const size_t size;
  size_t write;
  size_t read;

  Event events[capacity + 1];
};

/*
 * An interface
 */

class EventListener {
 public:
  virtual void handleEvent(Event e) = 0;
  virtual void setEventHandler(EventHandler *handler) = 0;
  virtual void unsetEventHandler(EventHandler *handler) = 0;
};

typedef void (*EventHandlerFunction)(Event e);
class EventHandler {
 public:
  EventHandler(EventType eventType, EventHandlerFunction fn);

  template <typename T>
  EventHandler(EventType eventType, T *tptr, void (T::*mptr)(Event));

  void handle(Event e);

  EventType type;
  FunctionPointerArg1<void, Event> _fp;
};

class EventDispatcher {
 public:
  EventDispatcher();

  void attachListener(EventListener *l);
  void detachListener(EventListener *l);
  void dispatchEvent(Event e);

  /**
   * Normally called from a separate thread to queue an event for later
   *processing by the main thread
   **/
  void queueEvent(const Event e);

  /**
   * Should be called on the main thread
   **/
  void pumpEvents();

 private:
  LinkedList<EventListener> _listeners;
  EventQueue<16> _mailbox;
};

#endif