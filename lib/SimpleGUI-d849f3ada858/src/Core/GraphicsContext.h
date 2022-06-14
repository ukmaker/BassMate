#ifndef SIMPLEGUI_GRAPHICSCONTEXT_H
#define SIMPLEGUI_GRAPHICSCONTEXT_H

#include "Adafruit_GFX_NG.h"
#include "DefaultFontRenderer_NG.h"
#include "gfxfont.h"
#include "Events/EventSystem.h"

/**
* The GraphicsContext holds the core class needed to handle drawing and events
**/
class GraphicsContext
{

public:

    GraphicsContext(Adafruit_GFX_NG* display, EventDispatcher* dispatcher, const GFXfont* defaultFont, DefaultFontRenderer_NG *fontRenderer);

    Adafruit_GFX_NG* display();
    EventDispatcher* eventDispatcher();
    const GFXfont* defaultFont();
    DefaultFontRenderer_NG *fontRenderer();

private:
    Adafruit_GFX_NG* _display;
    EventDispatcher* _dispatcher;
    const GFXfont* _font;
    DefaultFontRenderer_NG *_fontRenderer;
};

#endif