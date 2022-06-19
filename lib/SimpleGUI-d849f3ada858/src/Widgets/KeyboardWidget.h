#ifndef SIMPLEGUI_KEYBOARD_WIDGET_H
#define SIMPLEGUI_KEYBOARD_WIDGET_H

#include "Widget.h"

namespace simplegui {

class KeyboardWidget : public Widget {
public:
    typedef void (*KeyPressCallback)(char c);
    typedef void (*OKCancelCallback)();

    enum State { KEYS,
        TEXT,
        OK,
        CANCEL,
        USER };

    static const uint16_t ZONE_KEYS = 1;
    static const uint16_t ZONE_TEXT = 2;
    static const uint16_t ZONE_BUTTONS = 4;

    KeyboardWidget(GraphicsContext* context, char* buf, uint8_t buflen)
        : Widget(context)
        , _buf(buf)
        , _buflen(buflen - 1)
        , _state(KEYS)
    {
        _x = _lastX = 0;
        _y = _lastY = 0;
        _c = ' ';
        _highlightForegroundColor = WHITE;
        _highlightBackgroundColor = BLACK;
        _len = _lastLen = 0;
        _lastState = KEYS;
        _changed = true;
        _user = false;
        _dirtyContent = 0xffff;
    }

    template <class T>
    void onKeyPress(T* tptr, void (T::*mptr)(char c))
    {
        _keyPressCallback.attach(tptr, mptr);
    }

    template <class T>
    void onOK(T* tptr, void (T::*mptr)())
    {
        _okCallback.attach(tptr, mptr);
    }

    template <class T>
    void onCancel(T* tptr, void (T::*mptr)())
    {
        _cancelCallback.attach(tptr, mptr);
    }

    template <class T>
    void onUserKey(T* tptr, void (T::*mptr)())
    {
        _userCallback.attach(tptr, mptr);
    }

    void setText(char* text, uint8_t len)
    {
        if (len >= (_buflen - 1)) {
            memcpy(_buf, text, _buflen - 1);
            _buf[_buflen - 1] = '_';
            _buf[_buflen] = '\0';
            _len = _buflen - 1;
        } else {
            memcpy(_buf, text, len);
            _buf[len] = '_';
            _buf[len + 1] = '\0';
            _len = len;
        }
        noteDirtyContent(ZONE_TEXT);
    }

    char* getText() { return _buf; }
    uint8_t getTextLen()
    {
        return _len;
    }

    void setUserLabel(const char* label)
    {
        _userLabel = label;
        _user = true;
        noteDirtyContent(ZONE_BUTTONS);
    }

    void setHighlightForegroundColor(uint16_t color)
    {
        if (_highlightForegroundColor != color) {
            _highlightForegroundColor = color;
            noteDirtyContent(0xffff);
        }
    }

    uint16_t getHighlightForegroundColor() { return _highlightForegroundColor; }

    void setHighlightBackgroundColor(uint16_t color)
    {
        if (_highlightBackgroundColor != color) {
            _highlightBackgroundColor = color;
            noteDirtyContent(0xffff);
        }
    }

    uint16_t getHighlightBackgroundColor() { return _highlightBackgroundColor; }

    char getValue() { return _c; }

    /*
     * Move the cursor up one line.
     * returns true if movement happened
     */
    bool up()
    {

        bool moved = true;

        switch (_state) {
        case KEYS: {
            if (_y > 0) {
                _y--;
                noteDirtyContent(ZONE_KEYS);
            } else {
                moved = false;
            }
            break;
        }

        case TEXT: {
            noteDirtyContent(ZONE_TEXT | ZONE_KEYS);
            _state = KEYS;
            break;
        }

        default: {
            _state = TEXT;
            noteDirtyContent(ZONE_BUTTONS | ZONE_TEXT);
        }
        }
        _noteChanged();
        return moved;
    }

    bool down()
    {
        bool moved = true;

        switch (_state) {
        case KEYS: {
            if (_y < 3) {
                _y++;
                noteDirtyContent(ZONE_KEYS);
            } else {
                _state = TEXT;
                noteDirtyContent(ZONE_KEYS | ZONE_TEXT);
            }
            break;
        }

        case TEXT: {
            noteDirtyContent(ZONE_TEXT | ZONE_BUTTONS);
            _state = CANCEL;
            break;
        }

        default:
            moved = false;
            break;
        }
        _noteChanged();
        return moved;
    }

    bool left()
    {
        bool moved = true;

        switch (_state) {
        case KEYS: {
            moved = _left();
            break;
        }
        case TEXT: {
            // treat as backspace
            _delete();
            noteDirtyContent(ZONE_TEXT);
            break;
        }
        case USER: {
            _state = OK;
            noteDirtyContent(ZONE_BUTTONS);
            break;
        }
        case OK: {
            _state = CANCEL;
            noteDirtyContent(ZONE_BUTTONS);
            break;
        }
        default:
            moved = false;
            break;
        }
        _noteChanged();

        return moved;
    }

    bool back()
    {

        bool moved = false;

        switch (_state) {
        case KEYS: {
            moved = _left();
            break;
        }

        default:
            break;
        }
        _noteChanged();
        return moved;
    }

    bool right()
    {
        bool moved = true;

        switch (_state) {
        case KEYS: {
            moved = _right();
            break;
        }
        case CANCEL: {
            _state = OK;
            noteDirtyContent(ZONE_BUTTONS);
            break;
        }

        case OK:
            if (_user) {
                _state = USER;
                noteDirtyContent(ZONE_BUTTONS);
                break;
            } else {
                moved = false;
            }

        default:
            moved = false;
            break;
        }
        _noteChanged();
        return moved;
    }

    bool forward()
    {
        bool moved = true;

        switch (_state) {
        case KEYS: {
            moved = _right();
            break;
        }
        default:
            moved = false;
            break;
        }
        _noteChanged();
        return moved;
    }

    void select()
    {
        switch (_state) {
        case KEYS: {
            char c = _map[_y][_x];
            _append(c);
            _keyPressCallback.call(c);
            break;
        }

        case OK: {
            noteDirtyContent(ZONE_BUTTONS);
            _okCallback.call();
            break;
        }

        case CANCEL: {
            noteDirtyContent(ZONE_BUTTONS);
            _cancelCallback.call();
            break;
        }

        case USER: {
            noteDirtyContent(ZONE_BUTTONS);
            _userCallback.call();
            break;
        }

        default:
            break;
        }
    }

protected:
    uint8_t _x, _y;
    char _c;
    uint16_t _highlightForegroundColor, _highlightBackgroundColor;
    FunctionPointerArg1<void, char> _keyPressCallback;
    FunctionPointer _okCallback, _cancelCallback, _userCallback;

    const char* _ok = "OK";
    const char* _cancel = "Cancel";
    bool _user = false;
    const char* _userLabel;

    char* _buf;
    uint8_t _buflen;
    State _state;
    uint8_t _len;
    bool _changed;
    uint8_t _lastX, _lastY, _lastLen;
    State _lastState;

    const char _map[4][12] = {
        { '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=' },
        { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']' },
        { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '|' },
        { 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', '~', ':' }
    };

    void _noteChanged()
    {
        if (_state != _lastState)
            _changed = true;
        if (_x != _lastX)
            _changed = true;
        if (_y != _lastY)
            _changed = true;
        if (_len != _lastLen)
            _changed = true;
        if (_changed && _parent != nullptr) {
            _parent->noteChildDirty();
        }
    }

    bool _left()
    {
        bool moved = false;

        if (_x > 0) {
            _x--;
            noteDirtyContent(ZONE_KEYS);
            moved = true;
        } else if (_y > 0) {
            _y--;
            _x = 11;
            noteDirtyContent(ZONE_KEYS);
            moved = true;
        }

        return moved;
    }

    bool _right()
    {
        bool moved = false;
        if (_x < 11) {
            _x++;
            noteDirtyContent(ZONE_KEYS);
            moved = true;
        } else if (_y < 3) {
            _y++;
            _x = 0;
            noteDirtyContent(ZONE_KEYS);
            moved = true;
        }

        return moved;
    }

    void _append(char c)
    {
        if (_len < (_buflen - 2)) {
            _buf[_len] = c;
            _len++;
            _buf[_len] = '_';
            _buf[_len + 1] = '\0';
            _noteChanged();
            noteDirtyContent(ZONE_TEXT);
        }
    }

    void _delete()
    {
        if (_len > 0) {
            _len--;
            _buf[_len] = '_';
            _buf[_len + 1] = '\0';
        }
        _noteChanged();
    }

    virtual void _clearContent(bool force)
    {
        fontRenderer()->setFont(_font);
        if (force) {
            Widget::_clearContent(force);
        } else {
            if (_dirtyContent & ZONE_BUTTONS) {
                _clearButtons();
            }
            if (_dirtyContent & ZONE_KEYS) {
                _clearKeys();
            }
            if (_dirtyContent & ZONE_TEXT) {
                _clearText();
            }
        }
    }

    virtual void _clearButtons()
    {
        uint16_t h = fontRenderer()->getFontHeight() + 10; // including button padding and border
        display()->fillRect(0, _buttonsY(), _inner.width, h, RED);
    }

    virtual void _clearKeys()
    {
        uint16_t d = fontRenderer()->getFontHeight();
        _clearKey(_lastX, _lastY, _bg);
        _clearKey(_x, _y, _state == KEYS ? _highlightBackgroundColor : _bg);
    }

    virtual void _clearText()
    {
        uint16_t d = fontRenderer()->getFontHeight();
        uint16_t l = _inner.left() + d;
        uint16_t t = _inner.top() + 4 * d;
        if (_state == TEXT) {
            display()->fillRect(l, t, _inner.width - d, d + 2,
                _highlightBackgroundColor);
        } else {
            display()->fillRect(l, t, _inner.width - d, d + 2, _bg);
        }
    }

    virtual void _clearKey(uint8_t x, uint8_t y, uint16_t color)
    {
        int dx = fontRenderer()->getFontHeight();
        display()->fillRect(_keyX(x, y), _keyY(x, y), dx, dx, color);
    }

    uint16_t _buttonsY()
    {
        return _inner.top() + 8 * fontRenderer()->getFontHeight();
    }

    uint16_t _keyX(uint8_t x, uint8_t y)
    {
        int dx = fontRenderer()->getFontHeight();
        return _inner.left() + (x * dx) + (y * dx / 2);
    }

    uint16_t _keyY(uint8_t x, uint8_t y)
    {
        int dy = fontRenderer()->getFontHeight();
        return _inner.top() + (y * dy);
    }

    bool _isDirtyKey(uint8_t x, uint8_t y)
    {
        if (x == _lastX && y == _lastY)
            return true;
        if (x == _x && y == _y)
            return true;
        return false;
    }

    virtual void _drawContent(bool force)
    {
        fontRenderer()->setFont(_font);
        fontRenderer()->setTextColor(_fg, _bg);

        int dx = fontRenderer()->getFontHeight();
        int dy = dx;
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 12; x++) {
                if (force || _dirty || _isDirtyKey(x, y)) {
                    if (hasFocus() && _state == KEYS && (x == _x && _y == y)) {
                        fontRenderer()->setTextColor(_highlightForegroundColor,
                            _highlightBackgroundColor);

                    } else {
                        fontRenderer()->setTextColor(_fg, _bg);
                    }
                    fontRenderer()->setTextWindow(_keyX(x, y), _keyY(x, y), dx, dy);
                    display()->write(_map[y][x]);
                }
            }
        }

        fontRenderer()->removeTextWindow();

        uint16_t l = _inner.left() + dx;
        uint16_t t = _inner.top() + 4 * dy;
        int16_t x, y, x1, y1 = 0;
        uint16_t w, h = 0;

        // now draw the text in the buffer
        if (force || _dirty || (_dirtyContent & ZONE_TEXT)) {
            if (_state == TEXT) {
                fontRenderer()->setTextColor(_highlightForegroundColor,
                    _highlightBackgroundColor);
            } else {
                fontRenderer()->setTextColor(_fg, _bg);
            }

            fontRenderer()->getTextBounds(_buf, l, t, &x1, &y1, &w, &h);
            fontRenderer()->setTextWindow(l, t, _inner.width - 2 * dx, dy);
            display()->write(_buf);
            fontRenderer()->removeTextWindow();
        }

        if (force || _dirty || (_dirtyContent & ZONE_BUTTONS)) {
            // OK and Cancel buttons
            // And user button if enabled

            uint16_t cancelWidth, okWidth, userWidth, usedWidth = 0, n = 3;
            x = l;
            y = t + 1.5 * dy;
            fontRenderer()->getTextBounds(_ok, x, y, &x1, &y1, &okWidth, &h);
            usedWidth += okWidth;

            fontRenderer()->getTextBounds(_cancel, x, y, &x1, &y1, &cancelWidth, &h);
            usedWidth += cancelWidth;

            if (_user) {
                fontRenderer()->getTextBounds(_userLabel, x, y, &x1, &y1, &userWidth,
                    &h);
                usedWidth += userWidth;
                n = 4;
            }
            // space the buttons equal on all sides
            uint16_t remainingWidth = _inner.width - usedWidth;
            uint16_t spacing = remainingWidth / n;

            _button(x + spacing, y, cancelWidth, dy, 3, 2, _cancel, _state == CANCEL);
            _button(x + cancelWidth + 2 * spacing, y, okWidth, dy, 3, 2, _ok,
                _state == OK);
            if (_user)
                _button(x + cancelWidth + okWidth + 3 * spacing, y, okWidth, dy, 3, 2,
                    _userLabel, _state == USER);
        }
        _lastX = _x;
        _lastY = _y;
        _lastLen = _len;
        _lastState = _state;
        _changed = false;
    }

    /*
     * x, y : top left location of button
     * w, h : width and height of the text
     * p : padding to put bottom, left and right of the text
     * b: border width of the button outline
     */
    void _button(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t p,
        uint8_t b, const char* text, bool highlight)
    {
        uint16_t fg, bg;

        if (highlight && hasFocus()) {
            fg = _highlightForegroundColor;
            bg = _highlightBackgroundColor;
        } else {
            fg = _fg;
            bg = _bg;
        }

        display()->fillRoundRect(x, y, w + 2 * (b + p), h + 2 * b + p, 2 * b, fg);

        display()->fillRoundRect(x + b, y + b, w + 2 * p, h + p, 2 * b, bg);
        fontRenderer()->setTextColor(fg, bg);

        fontRenderer()->setTextWindow(x + b + p, y + p, w + 2 * p, h + p);
        display()->write(text);
        fontRenderer()->removeTextWindow();
    }
};
} // namespace simplegui

#endif