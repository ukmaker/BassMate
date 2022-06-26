#ifndef SIMPLEGUI_ASELECT_WIDGET_H
#define SIMPLEGUI_ASELECT_WIDGET_H

#include "Core/BorderTool.h"
#include "Core/Color.h"
#include "Widgets/SelectWidget.h"
namespace simplegui {
/**
 * a select widget allows the user to choose one of a set of choices
 * Choices are presented as scrolling text, with the selected choice centred and
 *bolded Choices are mapped to a backing value
 **/
template <typename V>
class AbstractSelectWidget : public SelectWidget {
 public:

 static const uint16_t ZONE_CONTENT = 1;

  AbstractSelectWidget(GraphicsContext *context) : SelectWidget(context) {}

  // abstract, so no constructor
  // default destructor to be a good c++ citizen
  ~AbstractSelectWidget() {}

  // concrete subclasses must implement these methods
  virtual const V getSelectedValue() = 0;
  virtual const V getValue(uint16_t idx) = 0;
  virtual bool equals(V a, V b) = 0;

  void setSelectedValue(V value) {
    for (uint16_t idx = 0; idx < getNumChoices(); idx++) {
      if (equals(value, getValue(idx))) {
        setSelectedIndex(idx);
        setViewIndex(idx);
        return;
      }
    }
    unselect();
  }
};
}
#endif