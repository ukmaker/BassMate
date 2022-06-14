#ifndef SIMPLEGUI_BITMAP_WIDGET_H
#define SIMPLEGUI_BITMAP_WIDGET_H

#include "Widgets/Widget.h"

class BitmapWidget : public Widget {
    
    public:
    
        BitmapWidget(GraphicsContext *context);
        BitmapWidget(GraphicsContext *context, bool monochrome);
                
        // Ccncrete methods for this class
        void setBitmap(const unsigned char * bitmap, int width, int height);
        
        // Set to draw the bitmap in monochrome
        void setMonochrome(bool enabled);
        bool isMonochrome();
        
        // Override to accommodate auto-resizing (yes, yes. sorry)
        virtual void setBorderWidth(int width);
    
    protected:

        virtual void _drawContent(bool force = false);      
            
        const unsigned char* _bitmap;
        bool _monochrome;
        int _bitmapWidth, _bitmapHeight;
};

#endif