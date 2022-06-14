#include "Widgets/SpinnerWidget.h"

#include "resources/BlueButtonLeft_50x64_bmp.h"
#include "resources/BlueButtonRight_50x64_bmp.h"

SpinnerWidget::SpinnerWidget(GraphicsContext *context)
    : Window(context),
      _upArrow(context), _text(context), _downArrow(context),
      _min(0), _max(0), _increment(0.5), _value(0),
      _format(""), _buf("")
{

    setLayout(FIXED);
    setBorderWidth(1);
    setBorderColor(_fg);
    setPadding(1);
    _upArrow.setBitmap(BlueButtonLeft_50x64_bmp, 50, 64);
    _downArrow.setBitmap(BlueButtonRight_50x64_bmp, 50, 64);

    // Calculate these here to take account of border and margin settings
    // since BitmapWidgets resize themselves rather than clip (!)
    setSize(
        context->display()->width(),
        _upArrow.height() + _box.getHeight());

    _upArrow.setLocation(
        _box.left.thickness(),
        _box.top.thickness());

    _downArrow.setLocation(
        width() - _downArrow.width() - _box.left.thickness(),
        _box.top.thickness());

    int textWidth = width() - _upArrow.width() - _box.getWidth() - _downArrow.width() - _box.getHeight();

    int textHeight = height() - _box.getHeight() - 2;

    _text.setSize(textWidth, textHeight);
    _text.setLocation(_downArrow.width() + _box.left.thickness(), _box.top.thickness());
    _text.setForeground(_fg);
    _text.setBackground(_bg);
    _text.setVAlign(VAlign::MIDDLE);

    EventHandler *up = new EventHandler(TOUCH_TAP, this, &SpinnerWidget::_onUpClick);
    EventHandler *down = new EventHandler(TOUCH_TAP, this, &SpinnerWidget::_onDownClick);

    attach(&_upArrow);
    attach(&_text);
    attach(&_downArrow);

    _upArrow.setEventHandler(up);
    _downArrow.setEventHandler(down);
}

TextWidget *SpinnerWidget::getTextWidget()
{
    return &_text;
}

void SpinnerWidget::setMin(float min)
{
    if (_min != min)
    {
        _min = min;
        _update();
    }
}

void SpinnerWidget::setMax(float max)
{
    if (_max != max)
    {
        _max = max;
        _update();
    }
}

void SpinnerWidget::setIncrement(float increment)
{
    if (_increment != increment)
    {
        _increment = increment;
        _update();
    }
}

void SpinnerWidget::setValue(float value)
{
    if (_value != value)
    {
        _value = value;
        _update();
    }
}

void SpinnerWidget::setFormat(const char *format)
{
    _format = format;
    _update();
}

float SpinnerWidget::getMin()
{
    return _min;
}

float SpinnerWidget::getMax()
{
    return _max;
}

float SpinnerWidget::getIncrement()
{
    return _increment;
}

float SpinnerWidget::getValue()
{
    return _value;
}

const char *SpinnerWidget::getFormat()
{
    return _format;
}

template <typename T>
void SpinnerWidget::onChange(T *tptr, void (T::*mptr)(Event))
{
    _onChange.attach(tptr, mptr);
}

void SpinnerWidget::_update()
{

    sprintf(_buf, _format, _value);
    _text.setText(_buf);

    noteDirty();
}

void SpinnerWidget::_onUpClick(Event e)
{
    _value += _increment;
    if (_value > _max)
    {
        _value = _max;
    }
    _update();
}

void SpinnerWidget::_onDownClick(Event e)
{
    _value -= _increment;
    if (_value < _min)
    {
        _value = _min;
    }
    _update();
}