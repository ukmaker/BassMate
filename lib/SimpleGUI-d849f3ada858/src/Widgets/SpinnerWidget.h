#ifndef SIMPLEGUI_SPINNER_WIDGET_H
#define SIMPLEGUI_SPINNER_WIDGET_H

#include "Widgets/BitmapWidget.h"
#include "Widgets/TextWidget.h"
#include "Widgets/Window.h"

/**
* A spinner widget shows up and down arrows to set a value
* It automatically sizes itself to fit the screen dimensions when created
**/
class SpinnerWidget : public Window
{

public:

    SpinnerWidget(GraphicsContext *context);
    
    TextWidget *getTextWidget();

    void setMin(float min);
    void setMax(float max);
    void setIncrement(float increment);
    void setValue(float value);
    void setFormat(const char* format);

    float getMin();
    float getMax();
    float getIncrement();
    float getValue();
    const char* getFormat();

    template<typename T>
    void onChange(T* tptr, void (T::*mptr)(Event e));


protected:

    BitmapWidget _upArrow;
    TextWidget _text;
    BitmapWidget _downArrow;

    float _min, _max, _increment, _value;

    const char* _format;
    char _buf[64];

    FunctionPointerArg1<void,Event> _onChange;

    virtual void _onUpClick(Event e);
    virtual void _onDownClick(Event e);

    void _update();
};
#endif