#ifndef SIMPLEGUI_DIALOG_WINDOW_H
#define SIMPLEGUI_DIALOG_WINDOW_H
#include "Window.h"

namespace simplegui {

/*
 * A DialogWindow expects that all its children
 * will be shown or hidden with the window.
 * 
 * When the window is focussed, the first (or previously focussed) child is
 * also automatically focussed.
 * 
 * Methods allow child focus to be stepped through sequentially
 * 
 * When the window is blurred, all children are
 * also blurred
 */
class DialogWindow : public Window {
public:

    DialogWindow(GraphicsContext *context) : Window(context) {
        _focus = nullptr;
        _traversable = true;
    }

    ~DialogWindow() {}

    void setTraversable(bool t) {
        _traversable = t;
    }

    bool isTraversable() {
        return _traversable;
    }

    virtual void attachFocus(Widget *w) {
        Window::attach(w);
        _focus = w;
        w->show();
    }

    virtual void show()
    {
        Window::show();
        Widget* w;
        _widgets.reset();
        while ((w = _widgets.next()) != NULL) {
            w->show();
        }
    }

    virtual void hide()
    {
        Window::hide();
        Widget* w;
        _widgets.reset();
        while ((w = _widgets.next()) != NULL) {
            w->hide();
        }
    }

    virtual void focus() {
        Window::focus();

        _getFocus();
        
        Widget* w;
        _widgets.reset();
        while ((w = _widgets.next()) != NULL) {
            if(w != _focus) {
                w->blur();
            }
        }       
        if(_focus != nullptr) {
            _focus->focus();
        }
    }

    virtual void blur() {
        Window::blur();

        Widget* w;
        _widgets.reset();
        while ((w = _widgets.next()) != NULL) {
            w->blur();
        }       
    }

    virtual Widget *focusLeft() {
        _getFocus();
        if(_traversable && _focus != nullptr) {
            int idx = _widgets.indexOf(_focus);
            if(idx < 0) {
                _focus = _widgets.getByIndex(idx - 1);
            }
        }
        return _focus;
    }

    virtual Widget *focusRight() {
        _getFocus();
        if(_traversable && _focus != nullptr) {
            int idx = _widgets.indexOf(_focus);
            if(idx + 1 < _widgets.size()) {
                _focus = _widgets.getByIndex(idx + 1);
            }
        }
        return _focus;
    }

    protected:

    bool _traversable;

    Widget *_getFocus() {
        if(_focus == nullptr) {
            _focus = _widgets.getByIndex(0);
        }
        return _focus;
    }

    Widget *_focus;
};
}
#endif