#ifndef SIMPLEGUI_VOLUMEBAR_RENDERER_H
#define SIMPLEGUI_VOLUMEBAR_RENDERER_H

#include "Core/GraphicsContext.h"

class VolumeBarRenderer {
 public:
  static void render(GraphicsContext *context, uint16_t x, uint16_t y,
                     uint16_t barWidth, uint16_t barHeight, uint8_t percent,
                     uint16_t color) {
    // draw the volume bar
    int xextent = ((barWidth - 1) * percent) / 100;
    int yextent = ((barHeight - 1) * percent) / 100;

    context->display()->fillTriangle(
        x, y + barHeight - 1, 
        x + xextent, y + barHeight - 1, 
        x + xextent, y + barHeight - 1 - yextent, color);
  }
};
#endif