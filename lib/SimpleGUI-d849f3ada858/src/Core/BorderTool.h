#ifndef SIMPLEGUI_BEVELTOOL_H
#define SIMPLEGUI_BEVELTOOL_H

#include "Adafruit_GFX.h"
#include "Core/Color.h"
#include "Core/Box.h"
#include "Core/Rectangle.h"
namespace simplegui {
class BorderTool
{

public:
    static void bevel(Adafruit_GFX *gfx, int x, int y, int w, int h, uint8_t bw, int color)
    {
        _draw(gfx, x, y, w, h, bw, color, true);
    }

    static void border(Adafruit_GFX *gfx, int x, int y, int w, int h, uint8_t bw, int color)
    {
        _draw(gfx, x, y, w, h, bw, color, false);
    }

    static void bevel(Adafruit_GFX *gfx, Rectangle r, Box box)
    {
        _draw(gfx, r, box, true);
    }

    static void border(Adafruit_GFX *gfx, Rectangle r, Box box)
    {
        _draw(gfx, r, box, false);
    }

protected:
    static void _draw(Adafruit_GFX *gfx, int x, int y, int w, int h, int bw, int color, bool bevel)
    {
        for (int b = 0; b < bw; b++)
        {
            // inset shading
            // B and R are rendered at 75% dark
            // T and L are rendered from 75% to 100% going outward
            uint8_t percent = ((b * 25) / bw);
            int c = bevel ? Color::fadeToBlack(color, percent) : color;
            gfx->drawFastHLine(x + b, y + b, w - 2 * b, c);                 // top - full width
            gfx->drawFastVLine(x + w - b - 1, y + b + 1, h - 2 * b - 2, c); // right - margin top and bottom
            gfx->drawFastHLine(x + b + 1, y + h - b - 1, w - 1 - 2 * b, c); // bottom - margin left
            gfx->drawFastVLine(x + b, y + b + 1, h - 2 * b - 1, c);             // left - margin top
        }
    }
    static void _draw(Adafruit_GFX *gfx, Rectangle r, Box box, bool bevel)
    {
        int tw = box.top.margin + box.top.border;
        int rw = box.right.margin + box.right.border;
        int bw = box.bottom.margin + box.bottom.border;
        int lw = box.left.margin + box.left.border;

        int mm = box.top.margin > box.right.margin? box.top.margin : box.right.margin;
        mm = mm > box.bottom.margin ? mm : box.bottom.margin;
        mm = mm > box.left.margin ? mm : box.left.margin;

        int mb = box.top.border > box.right.border? box.top.border : box.right.border;
        mb = mb > box.bottom.border ? mb : box.bottom.border;
        mb = mb > box.left.border ? mb : box.left.border;

        int mw = tw > rw ? tw : rw;
        mw = mw > bw ? mw : bw;
        mw = mw > lw ? mw : lw;

        int x = r.left();
        int y = r.top();
        int w = r.width();
        int h = r.height();
        
        for (int b = 0; b < mw; b++)
        {
            // inset shading
            // B and R are rendered at 75% dark
            // T and L are rendered from 75% to 100% going outward
            uint8_t percent = ((b * 25) / mw);
            int tc = bevel ? Color::fadeToBlack(box.top.color, percent) : box.top.color;
            int rc = bevel ? Color::fadeToBlack(box.right.color, percent) : box.right.color;
            int bc = bevel ? Color::fadeToBlack(box.bottom.color, percent) : box.bottom.color;
            int lc = bevel ? Color::fadeToBlack(box.left.color, percent) : box.left.color;
            if(b >= box.top.margin && b < tw) gfx->drawFastHLine(x + b, y + b, w - 2 * b, tc);                 // top - full width
            if(b >= box.right.margin && b < rw) gfx->drawFastVLine(x + w - b - 1, y + b + 1, h - 2 * b - 2, rc); // right - margin top and bottom
            if(b >= box.bottom.margin && b < bw) gfx->drawFastHLine(x + b + 1, y + h - b - 1, w - 1 - 2 * b, bc); // bottom - margin left
            if(b >= box.left.margin && b < lw) gfx->drawFastVLine(x + b, y + b + 1, h - 2 * b - 1, lc);             // left - margin top
        }
    }
};
}
#endif