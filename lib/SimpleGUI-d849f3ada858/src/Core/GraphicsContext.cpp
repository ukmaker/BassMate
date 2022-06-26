#include "Core/GraphicsContext.h"
namespace simplegui {
GraphicsContext::GraphicsContext(Adafruit_GFX* display, const GFXfont* defaultFont, AdafruitFontRenderer *fontRenderer) :
    _display(display), _font(defaultFont), _fontRenderer(fontRenderer)
{
}



Adafruit_GFX* GraphicsContext::display()
{
    return _display;
}

const GFXfont* GraphicsContext::defaultFont()
{
    return _font;
}

AdafruitFontRenderer *GraphicsContext::fontRenderer() {
    return _fontRenderer;
}
}