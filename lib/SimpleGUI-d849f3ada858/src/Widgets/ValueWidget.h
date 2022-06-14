#ifndef SIMPLEGUI_VALUE_WIDGET_H
#define SIMPLEGUI_VALUE_WIDGET_H

#include "Widgets/Widget.h"
#include "gfxfont.h"

class ValueWidget : public Widget {
 public:
  ValueWidget(GraphicsContext *context) : Widget(context) {}

  static const uint8_t LINEAR = 0;
  static const uint8_t LOG = 1;
  static const uint8_t ANTILOG = 2;

  static const uint16_t ZONE_VALUE = 1;

  void setFont(const GFXfont *font) {
    if (_font != font) {
      _font = font;
      noteDirtyContent(ZONE_VALUE);
    }
  }

  const GFXfont *getFont() { return _font; }

  void setValue(int v) {
    if (_value != v) {
      _value = v;
      noteDirtyContent(ZONE_VALUE);
    }
  }

  int getValue() { return _value; }

  void setMax(int max) { _max = max; }

  int getMax() { return _max; }

  void setMin(int min) { _min = min; }

  int getMin() { return _min; }

  void increment() {
    _value += _step;
    if (_value > _max) {
      _value = _max;
    }
    noteDirtyContent(ZONE_VALUE);
  }

  void decrement() {
    _value -= _step;
    if (_value < _min) {
      _value = _min;
    }
    noteDirtyContent(ZONE_VALUE);
  }
  void setStep(uint8_t step) {
    if (step > _max / 2) return;
    if (_step != step) {
      _step = step;
    }
  }

  uint8_t getStep() { return _step; }

  void setMode(uint8_t mode) {
    if (mode < 3) _mode = mode;
  }

  uint8_t getMode() { return _mode; }

 protected:
  /*
   * override
   * Draw label on the left, then a volume bar
   */
  virtual void _drawContent(bool force = false) {

    Adafruit_GFX_NG *d = display();

    const GFXfont *f = fontRenderer()->getFont();
    fontRenderer()->setFont(_font);
    fontRenderer()->setTextColor(_fg, _bg);
    fontRenderer()->setTextWindow(_inner.x, _inner.y, _inner.width, _inner.height);
    display()->print(_value);
    fontRenderer()->removeTextWindow();
    fontRenderer()->setFont(f);
  }

 protected:
  int _value = 0;
  int _max = 100;
  int _min = 0;
  uint8_t _step = 1;
  uint8_t _mode = LINEAR;
  const GFXfont *_font;
};

#endif