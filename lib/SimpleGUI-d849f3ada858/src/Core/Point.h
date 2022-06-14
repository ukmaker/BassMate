#ifndef SIMPLEGUI_POINT_H
#define SIMPLEGUI_POINT_H

class Point {
    
    public:
    
    Point(int x, int y) : _x(x), _y(y) {}
    
    int x() { return _x; }
    int y() { return _y; }
    void x(int x) { _x = x; }
    void y(int y) { _y = y; }
    
    bool equals(Point *p) {
        return (p->x() == x()) && (p->y() == y());
    }
    
    private:
    
    int _x, _y;
    
};


#endif