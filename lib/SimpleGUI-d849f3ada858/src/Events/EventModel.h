#ifndef SIMPLEGUI_EVENT_MODEL_H
#define SIMPLEGUI_EVENT_MODEL_H

#include "Core/GraphicsContext.h"
#include "Widgets/Widget.h"

// Class providing a way to plug in different event models
// to cover e.g. mouse, touchscreen or NavKey
// Set an appropriate implementation on GraphicsContext
namespace simplegui {

  class EventModel {
    public:

    virtual bool isEventTarget(Widget &w, Event &e) = 0;

  };
}
#endif