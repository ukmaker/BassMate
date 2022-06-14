#ifndef SIMPLEGUI_MOUSE_EVENT_MODEL_H
#define SIMPLEGUI_MOUSE_EVENT_MODEL_H

#include "Events/EventModel.h"

// Class providing a way to plug in different event models
// to cover e.g. mouse, touchscreen or NavKey
// Set an appropriate implementation on GraphicsContext
namespace simplegui {

class MouseEventModel : public EventModel {
 public:
  MouseEventModel() {}
  ~MouseEventModel() {}

  virtual bool isEventTarget(Widget &w, Event &e) {
    return !w.isHidden() && e.screenX >= w.x() &&
           e.screenX <= (w.x() + w.width()) && e.screenY >= w.y() &&
           e.screenY <= (w.y() + w.height());
  }
};
}  // namespace simplegui
#endif