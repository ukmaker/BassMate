#ifndef SIMPLEGUI_WRAPPER_SELECT_WIDGET_H
#define SIMPLEGUI_WRAPPER_SELECT_WIDGET_H

#include "Widgets/AbstractSelectWidget.h"
template <class V>
class Wrapper {
  
  public:


  virtual const char *getText(V arg);
  virtual bool equals(V a, V b);

};


namespace simplegui {



template <typename V>
class WrapperSelectWidget : public AbstractSelectWidget<V> {
 public:
  WrapperSelectWidget(GraphicsContext *context, Wrapper<V> *wrapper, V *values,
                      uint8_t numValues)
      : AbstractSelectWidget<V>(context),
        _wrapper(wrapper),
        _values(nullptr),
        _numValues(0) {}

  // abstract, so no constructor
  // default destructor to be a good c++ citizen
  ~WrapperSelectWidget() {}

  void setValues(const V *values, uint8_t numValues) {
    _values = values;
    _numValues = numValues;
  }

  // Implement abstract methods
  const char *getSelectedText() {
    if (this->getSelectedIndex() < _numValues) {
      return _wrapper->getText(_values[this->getSelectedIndex()]);
    }
    return nullptr;
  }
  const char *getText(uint16_t idx) {
    if (idx < _numValues) {
      return _wrapper->getText(_values[idx]);
    }
    return nullptr;
  }

  const V getSelectedValue() {
    if (this->getSelectedIndex() < _numValues) {
      return _values[this->getSelectedIndex()];
    }
    return _values[0];
  }

  const V getValue(uint16_t idx) {
    if (idx < _numValues) {
      return _values[idx];
    }
    return _values[0];
  }

  const V getViewValue() {
    return _values[this->getViewIndex()];
  }

  uint16_t getNumChoices() { return _numValues; }

  bool equals(V a, V b) {
    return _wrapper->equals(a,b);
  }

 protected:
  const V *_values;
  uint8_t _numValues;
  Wrapper<V> *_wrapper;
};
}  // namespace simplegui
#endif