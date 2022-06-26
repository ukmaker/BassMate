#ifndef SIMPLEGUI_BOX_H
#define SIMPLEGUI_BOX_H

#include <stdint.h>
namespace simplegui {
class Edge
{
public:
    enum Property
    {
        MARGIN,
        BORDER,
        COLOR,
        PADDING
    };

    void set(Property property, uint16_t value)
    {
        switch (property)
        {
        case MARGIN:
            margin = value;
            break;
        case BORDER:
            border = value;
            break;
        case COLOR:
            color = value;
            break;
        case PADDING:
            padding = value;
            break;
        default:
            break;
        }
    }

    uint16_t get(Property property)
    {
        switch (property)
        {
        case MARGIN:
            return margin;
            break;
        case BORDER:
            return border;
            break;
        case COLOR:
            return color;
            break;
        case PADDING:
            return padding;
            break;
        default:
            return 0;
            break;
        }
    }

    /**
     * @brief Return the sum of margin, border and padding
     * 
     */
    uint16_t thickness() {
        return margin+border+padding;
    }

    uint16_t margin, border, color, padding;
};

class Box
{

public:
    enum Side
    {
        TOP,
        RIGHT,
        BOTTOM,
        LEFT,
        ALL
    };

    void set(Side side, Edge::Property property, uint16_t value)
    {
        switch (side)
        {
        case TOP:
            top.set(property, value);
            break;
        case RIGHT:
            right.set(property, value);
            break;
        case BOTTOM:
            bottom.set(property, value);
            break;
        case LEFT:
            left.set(property, value);
            break;
        case ALL:
            top.set(property, value);
            right.set(property, value);
            bottom.set(property, value);
            left.set(property, value);
            break;
        default:
            break;
        }
    }

    /*
    * Return the overall width = sum of left+right margin, border and padding
    */
   uint16_t getWidth() {
       return left.thickness() + right.thickness();
   }
    /*
    * Return the overall height = sum of top+bottom margin, border and padding
    */
   uint16_t getHeight() {
       return top.thickness() + bottom.thickness();
   }

    Edge top;
    Edge right;
    Edge bottom;
    Edge left;

};
}
#endif