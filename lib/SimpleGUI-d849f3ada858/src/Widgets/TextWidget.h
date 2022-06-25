#ifndef SIMPLEGUI_TEXT_WIDGET_H
#define SIMPLEGUI_TEXT_WIDGET_H

#include "Widgets/Widget.h"
#include "Align.h"

class TextWidget : public Widget
{

public:

    static const uint16_t ZONE_CONTENT = 1;

    TextWidget(GraphicsContext *context);
    TextWidget(GraphicsContext *context, GFXfont *font);

    /**************************
     * Custom methods of this class
     **************************/
    virtual void setText(const char *text);
    virtual void setHAlign(HAlign alignment);
    virtual void setVAlign(VAlign alignment);

protected:
    /**************************
     * Overrides of Widget
     **************************/
    virtual void _drawContent(bool force = false);

    /**************************
     * Custom data of this class
     **************************/
    char _text[64];
    HAlign _halign;
    VAlign _valign;
};

#endif