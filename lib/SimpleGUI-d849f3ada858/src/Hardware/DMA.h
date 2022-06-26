#ifndef SIMPLEGUI_DMA_H
#define SIMPLEGUI_DMA_H
/**
 * Interface defining how to encapsulate hardware DMA support
 */
#include <stdint.h>

namespace simplegui {

class DMA {

 public:

  virtual void begin()=0;
  virtual void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)=0;
  virtual void beginWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t backgroundColor)=0;
  virtual void drawWindowPixel(int16_t x, int16_t y, uint16_t color)=0;
  virtual void flushWindow()=0;
  virtual void waitComplete()=0;
};
}
#endif