#ifndef SIMPLEGUI_EVENT_TYPE_H
#define SIMPLEGUI_EVENT_TYPE_H

// Defines core event types which can be used/re-used by other packages

enum EventType {

  NULL_EVENT = 0,
  // For touch screens
  TOUCH_START = 1 << 0,
  TOUCH_END = 1 << 1,
  TOUCH_MOVE = 1 << 2,
  TOUCH_TAP = 1 << 3,
  TOUCH_DOUBLE_TAP = 1 << 4,
  // For mice
  MOUSE_DOWN = 1 << 5,
  MOUSE_UP = 1 << 6,
  MOUSE_MOVE = 1 << 7,
  MOUSE_DRAG = 1 << 8,
  MOUSE_CLICK = 1 << 9,
  MOUSE_DOUBLE_CLICK = 1 << 10,

  //Widgets
  WIDGET_CHANGED = 1 << 11,

  // NavKey
  NAV_UP = 1 << 12,
  NAV_DOWN = 1 << 13,
  NAV_LEFT = 1 << 14,
  NAV_RIGHT = 1 << 15,
  NAV_INC = 1 << 16,
  NAV_DEC = 1 << 17,
  NAV_SELECT = 1 << 18,
  NAV_RELEASE = 1 << 19,

  // Generic encoder
  ENCODER_INC = 1 << 20,
  ENCODER_DEC = 1 << 21,
  ENCODER_SELECT = 1 << 22

};
#endif