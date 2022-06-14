#ifndef SIMPLEGUI_RECTANGLE_H
#define SIMPLEGUI_RECTANGLE_H

#include <algorithm>
#include "Point.h"
#include "Box.h"

class Rectangle
{

public:

    Rectangle(int _x, int _y, int _w, int _h) :
        x(_x), y(_y), width(_w), height(_h) {}

    void resize(const Rectangle &outer, Box box) {
        x = outer.x+box.left.thickness();
        y = outer.y+box.top.thickness();
        width = outer.width - box.getWidth();
        height = outer.height - box.getHeight();
    }

    bool contains(int pointX, int pointY) {
        return pointX >= x
               && pointX <= (x+width)
               && pointY >= y
               && pointY <= (y+height);
    }

    bool intersects(const Rectangle &r) {
        int x1 = std::max(x, r.x);
        int x2 = std::min(x+width, r.x + r.width);
        int y1 = std::max(y, r.y);
        int y2 = std::min(y+height, r.y + r.height);
        if((x2 < x1) || (y2 < y1)) {
            return false;
        }
        return true;
    }
    
    bool isLocatedAt(Point *p) {
        return p->x() == x && p->y() == y;
    }

    int left() {
        return x;
    }

    int right() {
        return x + width - 1;
    }

    int top() {
        return y;
    }

    int bottom() {
        return y + height -1;
    }
    
    int x, y, width, height;
};

#endif