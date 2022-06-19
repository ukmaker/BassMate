#ifndef SIMPLEGUI_TAB_WIDGET_H
#define SIMPLEGUI_TAB_WIDGET_H

#include "Widgets/Window.h"

class TabWidget : public Window {
 public:
  static const uint16_t ZONE_CONTENT = 1;
  static const uint16_t ZONE_TABS = 2;

  TabWidget(GraphicsContext *context);

  /**************************
   * Overrides of Window
   **************************/
  // the first attached widget is shown by default
  virtual void attach(Widget *widget);
  /*************************
   * Custom to this class
   *************************/
  virtual bool left();
  virtual bool right();
  virtual void selectTabByIndex(uint8_t idx);
  virtual void selectTab(Widget *widget);
  virtual void setTabSelectedColor(uint16_t c);
  virtual void setTabUnselectedColor(uint16_t c);
  virtual void setTabSelectedFontColor(uint16_t c);
  virtual void setTabUnselectedFontColor(uint16_t c);
  virtual void setTabBorderColor(uint16_t c);
  virtual uint16_t getTabSelectedColor();
  virtual uint16_t getTabUnselectedColor();
  virtual uint16_t getTabSelectedFontColor();
  virtual uint16_t getTabUnselectedFontColor();
  virtual uint16_t getTabBorderColor();
  virtual void setTabHeight(uint16_t h);
  virtual void setTabWidth(uint16_t w);
  virtual uint16_t getTabWidth();
  virtual uint16_t getTabHeight();
  virtual void setTabBorderWidth(uint8_t w);
  virtual uint8_t getTabBorderWidth();
  virtual void setTabPadding(uint8_t w);
  virtual uint8_t getTabPadding();
  virtual Widget *focusChild();
  virtual void unfocusChild();

 protected:
  Widget *_selected = nullptr;
  uint8_t _idx = 0;
  uint16_t _selectedColor = 0;
  uint16_t _selectedFontColor = 0;
  uint16_t _unselectedColor = 0;
  uint16_t _unselectedFontColor = 0;
  uint16_t _tabHeight = 0;
  uint16_t _tabWidth = 0;
  uint8_t _tabBorderWidth = 0;
  uint8_t _tabPadding = 0;
  uint16_t _tabBorderColor = 0;

  /**************************
   * Overrides of Widget
   **************************/
  virtual void _drawContent(bool force = false);
  virtual void _drawChildren(bool force = false);
  virtual void _adjust();
};

#endif