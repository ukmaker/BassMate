#ifndef SIMPLEGUI_RECTANGLE_H
#define SIMPLEGUI_RECTANGLE_H

#include <algorithm>
#include "Point.h"
#include "Box.h"

class Rectangle
{

public:

    Rectangle(int x, int y, int w, int h) :
        _x(x), _y(y), _width(w), _height(h) {}

    void resize(const Rectangle &outer, Box box) {
        _x = outer._x+box.left.thickness();
        _y = outer._y+box.top.thickness();
        _width = outer._width - box.getWidth();
        _height = outer._height - box.getHeight();
    }

    void resize(int w, int h) {
        _width = w;
        _height = h;
    }

    bool contains(int pointX, int pointY) {
        return pointX >= _x
               && pointX <= (_x+_width)
               && pointY >= _y
               && pointY <= (_y+_height);
    }

    bool intersects(const Rectangle &r) {
        int x1 = std::max(_x, r._x);
        int x2 = std::min(_x+_width, r._x + r._width);
        int y1 = std::max(_y, r._y);
        int y2 = std::min(_y+_height, r._y + r._height);
        if((x2 < x1) || (y2 < y1)) {
            return false;
        }
        return true;
    }
    
    bool isLocatedAt(Point *p) {
        return p->x() == _x && p->y() == _y;
    }

    inline int left() {
        return _x;
    }

    inline int right() {
        return _x + _width - 1;
    }

    inline int top() {
        return _y;
    }

    inline int bottom() {
        return _y + _height -1;
    }

    inline int x() {
        return _x;
    }

    inline int y() {
        return _y;
    }

    inline int width() {
        return _width;
    }

    inline int height() {
        return _height;
    }

    void move(int x, int y) {
        _x = x;
        _y = y;
    }

    protected:
    int _x, _y, _width, _height;
};

#endif