#ifndef SIMPLEGUI_WIDGET_H
#define SIMPLEGUI_WIDGET_H

class Widget;

#include "Core/GraphicsContext.h"
#include "Events/EventSystem.h"
#include "Core/Rectangle.h"
#include "Core/Box.h"
#include "Core/LinkedList.h"
#include "DefaultFontRenderer_NG.h"
/**
 * A basic widget draws itself in a rectangular area
 **/

class Widget
{
public:
    constexpr static char *EMPTY = "_";

    Widget(GraphicsContext *context);

    virtual void setParent(Widget *parent);
    virtual Widget *getParent();

    /********************************************************
     * Methods relating to containment
     * In the base class these methods do nothing.
     * They are implemented in container classes
     ********************************************************/
    virtual void attach(Widget *widget) {}
    virtual void detach(Widget *widget) {}

    /********************************************************
     * Common Widget methods
     ********************************************************/
    virtual void setLocation(int x, int y);
    virtual void setSize(int width, int height);
    virtual void setWidth(int width);
    virtual void setHeight(int height);
    virtual int x();
    virtual int y();
    virtual int height();
    virtual int width();

    virtual void setOffset(int x, int y);
    virtual int offsetX();
    virtual int offsetY();

    virtual void setForeground(uint16_t color);
    virtual void setBackground(uint16_t color);

    /*
     * Sets the padding for all the side
     */
    virtual void setPadding(int pixels);
    /*
     * Sets the border for all the side
     */
    virtual void setBorderWidth(int width);
    /*
     * Sets the border color for all the side
     */
    virtual void setBorderColor(uint16_t colour);
    /*
     * Sets the margin for all the side
     */
    virtual void setMargin(int width);

    /*
     * Sets the padding for the given side(s)
     */
    virtual void setPadding(Box::Side side, int pixels);
    /*
     * Sets the border for the given side(s)
     */
    virtual void setBorderWidth(Box::Side side, int width);
    /*
     * Sets the border color for the given side(s)
     */
    virtual void setBorderColor(Box::Side side, uint16_t colour);
    /*
     * Sets the margin for for the given side(s)
     */
    virtual void setMargin(Box::Side side, int width);

    virtual void setBevel(bool bevel);

    virtual void setFont(const GFXfont *font);
    virtual const GFXfont *getFont();
    virtual void setLabel(const char *label);
    virtual const char *getLabel();
    /**
     * Cause the widget to redraw itself if is is dirty or damaged
     **/
    virtual void draw(bool force = false);
    /*
     * Cause the widget to paint its area of the screen with the background color
     **/
    virtual void clear();

    void show();
    void hide();
    bool isHidden();

    void focus();
    void blur();
    bool hasFocus();

    bool intersects(Widget *widget);

    /**
     * Mark the widget as needing to be redrawn
     **/
    void noteDirty();
    bool isDirty();
    /*
     * Note which zones are dirty - _dirtyContent is ORed with any previous call
     * and notify the parent (if there is one) that it has a dirty child
     */
    void noteDirtyContent(uint16_t zones);
    bool isDirtyContent();
    void noteChildDirty();
    bool hasDirtyChildren();

    /**
     * Mark the widget as having changed dimensions or location
     **/
    void noteDamage();

    void adjust();

    /**
     * Convenience methods
     **/
    Adafruit_GFX_NG *display();
    DefaultFontRenderer_NG *fontRenderer();

    /**************************************************
     * Templated methods need to be defined in the
     * header file, not the .cpp file
     * Otherwise they are not visible to derived classes
     * Isn't C++ just wonderful...
     **************************************************/
    template <class T>
    void onFocus(T *tptr, void (T::*mptr)(Widget *))
    {
        _onFocus.attach(tptr, mptr);
    }

    void onFocus(void (*mptr)(Widget *))
    {
        _onFocus.attach(mptr);
    }

    template <class T>
    void onChange(T *tptr, void (T::*mptr)(Widget *))
    {
        _onChange.attach(tptr, mptr);
    }

    void onChange(void (*mptr)(Widget *))
    {
        _onChange.attach(mptr);
    }

    template <class T>
    void onBlur(T *tptr, void (T::*mptr)(Widget *))
    {
        _onBlur.attach(tptr, mptr);
    }

    void onBlur(void (*mptr)(Widget *))
    {
        _onBlur.attach(mptr);
    }

protected:
    virtual void _draw();
    virtual void _drawContent(bool force);
    virtual void _drawChildren(bool force);
    virtual void _clear();
    virtual void _clearContent(bool force);
    virtual void _adjust();

    Widget *_parent = nullptr;
    GraphicsContext *_context;
    uint16_t _fg, _bg;
    const GFXfont *_font;
    const char *_label = EMPTY;

    Point _offset;
    Rectangle _outer;
    Rectangle _inner;

    bool _hidden;
    bool _focus;

    Box _box;
    bool _bevel;

    bool _dirty;            // if set, this widget has changed and needs to be redrawn
    uint16_t _dirtyContent; // interpreted by the widget to indicate which zone are dirty and need to be redrawn
    bool _dirtyChildren = false;
    bool _revealed = false;

    FunctionPointerArg1<void, Widget *> _onChange;
    FunctionPointerArg1<void, Widget *> _onFocus;
    FunctionPointerArg1<void, Widget *> _onBlur;
};

#endif