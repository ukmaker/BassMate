#ifndef SIMPLEGUI_GRAPHICSCONTEXT_H
#define SIMPLEGUI_GRAPHICSCONTEXT_H

#include "Adafruit_GFX.h"
#include "Renderers/AdafruitFontRenderer.h"
#include "gfxfont.h"
#include "Events/EventSystem.h"
namespace simplegui {
/**
* The GraphicsContext holds the core class needed to handle drawing and events
**/
class GraphicsContext
{

public:

    GraphicsContext(Adafruit_GFX* display, const GFXfont* defaultFont, AdafruitFontRenderer *fontRenderer);

    Adafruit_GFX* display();
    const GFXfont* defaultFont();
    AdafruitFontRenderer *fontRenderer();

private:
    Adafruit_GFX* _display;
    const GFXfont* _font;
    AdafruitFontRenderer *_fontRenderer;
};
}
#endif