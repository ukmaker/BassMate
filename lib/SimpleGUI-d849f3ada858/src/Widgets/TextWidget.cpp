#include "Widgets/TextWidget.h"

/**
 * A basic widget implementation which just draws some text.
 * If the text does not fit in the bounding-box it will be clipped
 **/
TextWidget::TextWidget(GraphicsContext *context) : Widget(context),
                                                   _halign(LEFT), _valign(TOP)
{
    memset(_text, 0, 64);
}

TextWidget::TextWidget(GraphicsContext *context, GFXfont *font) : Widget(context),
                                                                  _halign(LEFT), _valign(TOP)
{
    setFont(font);
}

void TextWidget::setText(const char *text)
{
    if (strncmp(_text, text, strlen(text)) != 0)
    {
        strcpy(_text, text);
        noteDirtyContent(ZONE_CONTENT);
    }
}

void TextWidget::setHAlign(HAlign alignment)
{
    _halign = alignment;
    noteDirtyContent(ZONE_CONTENT);
}

void TextWidget::setVAlign(VAlign alignment)
{
    _valign = alignment;
    noteDirtyContent(ZONE_CONTENT);
}

void TextWidget::_drawContent(bool force)
{
    /**
     * Figure out how many lines of text we have
     **/
    int numLines = 1;
    const char *c = _text;
    while (*c != NULL)
    {
        if (*c == '\n')
        {
            numLines++;
        }
        c++;
    }

    /******************************************************************/
    /*   ---------------------------------            ^               */
    /*   |                               |            |               */
    /*   |                               |            |               */
    /*   |                               |       inner.height         */
    /*   |                               |            |               */
    /*   |                               |            |               */
    /*   |                               |            |               */
    /*   |                               |            |               */
    /*   ---------------------------------            |               */
    /******************************************************************/
    /**
     * We need a window as high as the font with it's origin:
     * VALIGN=TOP    : (0, 0)
     * VALIGN=MIDDLE : (0, inner.height/2 - numLines * font.height/2)
     * VALIGN=BOTTOM : (0, inner.height   - numLines * font.height)
     **/
    const GFXfont *f = fontRenderer()->getFont();
    if (_font != nullptr)
        fontRenderer()->setFont(_font);

    fontRenderer()->setTextColor(_fg, _bg);

    fontRenderer()->setTextWindow(_inner.x(), _inner.y(), _inner.width(), _inner.height());
    fontRenderer()->setHAlign(_halign);
    fontRenderer()->setVAlign(_valign);
    fontRenderer()->writeAligned(display(), _text);
    fontRenderer()->removeTextWindow();
    fontRenderer()->setFont(f);
}