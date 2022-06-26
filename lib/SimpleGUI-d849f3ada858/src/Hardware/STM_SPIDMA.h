#ifndef SIMPLEGUI_STM_DMA_H
#define SIMPLEGUI_STM_DMA_H
/**
 * Helper class to make shoving data through the SPI using DMA
 * a bit easier.
 * Should allow high-speed flood-fills and fast buffered write
 * for font rendering
 */
#include "DMA.h"
#include "Adafruit_SPITFT.h"
#include "SPI.h"
#include "stm32f411xe.h"

extern volatile bool spiDmaTransferComplete;

namespace simplegui {

class STM_SPIDMA : public DMA {
  
 public:
  STM_SPIDMA(Adafruit_SPITFT *display, uint16_t bufferSize, SPI_TypeDef *spi, DMA_Stream_TypeDef *dma);

  ~STM_SPIDMA();

  void begin();
  void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

  void beginWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t backgroundColor);
  void drawWindowPixel(int16_t x, int16_t y, uint16_t color);
  void flushWindow();
  void waitComplete();

 public:
  Adafruit_SPITFT *_display;
  SPI_HandleTypeDef _spi;
  DMA_HandleTypeDef _dma;
  uint16_t _bufferSize;
  uint16_t *_buffer;
  uint16_t _fw, _fl;

 protected:
  uint16_t byteSwap(uint16_t w);
};
}
#endif