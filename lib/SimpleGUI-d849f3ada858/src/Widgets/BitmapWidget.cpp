#include "Widgets/BitmapWidget.h"
namespace simplegui {
BitmapWidget::BitmapWidget(GraphicsContext *context) : 
Widget(context), _monochrome(false), _bitmapWidth(0), _bitmapHeight(0)
{
}

BitmapWidget::BitmapWidget(GraphicsContext *context, bool monochrome) : 
Widget(context), _monochrome(monochrome), _bitmapWidth(0), _bitmapHeight(0)
{
}

void BitmapWidget::setMonochrome(bool enabled)
{
    if(_monochrome != enabled) {
        _monochrome = enabled;
        noteDirty();
    }
}

bool BitmapWidget::isMonochrome()
{
    return _monochrome;
}


void BitmapWidget::setBitmap(const unsigned char * bitmap, int width, int height)
{
    _bitmap = bitmap;
    _bitmapWidth = width;
    _bitmapHeight = height;
    // Adjust overall size so that the inner window fits the bitmap
    // Really we should just clip, but that's too hard for now
    setSize(_box.getWidth() + _bitmapWidth, _box.getHeight() + _bitmapHeight);
    noteDamage();
}

void BitmapWidget::setBorderWidth(int width)
{
    Widget::setBorderWidth(width);
    setSize(_box.getWidth() + _bitmapWidth, _box.getHeight() + _bitmapHeight);
    noteDamage();
}

void BitmapWidget::_drawContent(bool force)
{
    
    if(_monochrome) {
        display()->drawBitmap(_inner.x(), _inner.y(), _bitmap, _inner.width(), _inner.height(), _fg);
    } else {
        display()->drawRGBBitmap(_inner.x(), _inner.y(), (uint16_t*)_bitmap, _inner.width(), _inner.height());
    }
}
}