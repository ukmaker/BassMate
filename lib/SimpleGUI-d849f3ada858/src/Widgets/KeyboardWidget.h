#ifndef SIMPLEGUI_KEYBOARD_WIDGET_H
#define SIMPLEGUI_KEYBOARD_WIDGET_H

#include "Core/ColorsRGB16.h"
#include "Widget.h"

namespace simplegui {

class KeyboardWidget : public Widget {
public:
    enum State { KEYS,
        TEXT };

    static const uint16_t ZONE_KEYS = 1;
    static const uint16_t ZONE_TEXT = 2;

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
        _dirtyContent = 0xffff;
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

    void top()
    {
        _x = 0;
        _y = 0;
        _state = KEYS;
        noteDirty();
    }

    /*
     * Move the cursor up one line.
     * returns true if movement happened
     */
    bool up()
    {

        bool moved = true;

        if (_state == KEYS) {
            if (_y > 0) {
                _y--;
                noteDirtyContent(ZONE_KEYS);
            } else {
                moved = false;
            }
        } else {
            noteDirtyContent(ZONE_TEXT | ZONE_KEYS);
            _state = KEYS;
        }

        _noteChanged();
        return moved;
    }

    bool down()
    {
        bool moved = true;

        if (_state == KEYS) {
            noteDirtyContent(ZONE_KEYS);
            if (_y < 3) {
                _y++;
            } else {
                _state = TEXT;
                noteDirtyContent(ZONE_KEYS | ZONE_TEXT);
            }
        } else {
            moved = false;
        }

        _noteChanged();
        return moved;
    }

    bool left()
    {
        if (_state == KEYS) {
            _left();
        } else {
            // treat as backspace
            _delete();
            noteDirtyContent(ZONE_TEXT);
        }

        _noteChanged();
        return true;
    }

    bool right()
    {
        if (_state == KEYS) {
            _right();
        }
        _noteChanged();
        return true;
    }

    void select()
    {
        if (_state == KEYS) {
            char c = _map[_y][_x];
            _append(c);
            _onChange.call(this);
        }
    }

protected:
    uint8_t _x, _y;
    char _c;
    uint16_t _highlightForegroundColor, _highlightBackgroundColor;

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
            _clearKeys();
            _clearText();
        } else {
            if (_dirtyContent & ZONE_KEYS) {
                _clearKeys();
            }
            if (_dirtyContent & ZONE_TEXT) {
                _clearText();
            }
        }
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
        if (hasFocus() && _state == TEXT) {
            display()->fillRect(l, t, _inner.width() - d, d + 2,
                _highlightBackgroundColor);
        } else {
            display()->fillRect(l, t, _inner.width() - d, d + 2, _bg);
        }
    }

    virtual void _clearKey(uint8_t x, uint8_t y, uint16_t color)
    {
        int dx = fontRenderer()->getFontHeight();
        display()->fillRect(_keyX(x, y), _keyY(x, y), dx, dx, color);
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
                    fontRenderer()->write(_map[y][x]);
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
            if (hasFocus() && _state == TEXT) {
                fontRenderer()->setTextColor(_highlightForegroundColor,
                    _highlightBackgroundColor);
            } else {
                fontRenderer()->setTextColor(_fg, _bg);
            }

            fontRenderer()->getTextBounds(_buf, l, t, &x1, &y1, &w, &h);
            fontRenderer()->setTextWindow(l, t, _inner.width() - 2 * dx, dy);
            fontRenderer()->write(_buf);
            fontRenderer()->removeTextWindow();
        }

        _lastX = _x;
        _lastY = _y;
        _lastLen = _len;
        _lastState = _state;
        _changed = false;
    }
};
} // namespace simplegui

#endif