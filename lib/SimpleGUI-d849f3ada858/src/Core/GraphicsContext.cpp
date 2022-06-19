#include "Core/GraphicsContext.h"

GraphicsContext::GraphicsContext(Adafruit_GFX_NG* display, const GFXfont* defaultFont, DefaultFontRenderer_NG *fontRenderer) :
    _display(display), _font(defaultFont), _fontRenderer(fontRenderer)
{
}



Adafruit_GFX_NG* GraphicsContext::display()
{
    return _display;
}

const GFXfont* GraphicsContext::defaultFont()
{
    return _font;
}

DefaultFontRenderer_NG *GraphicsContext::fontRenderer() {
    return _fontRenderer;
}