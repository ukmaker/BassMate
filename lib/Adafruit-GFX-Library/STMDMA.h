#ifndef UKMAKER_STM_DMA_H
#define UKMAKER_STM_DMA_H
/**
 * Helper class to make shoving data through the SPI using DMA
 * a bit easier.
 * Should allow high-speed flood-fills and fast buffered write
 * for font rendering
 */
#include <stdint.h>

#include "SPI.h"
#include "stm32f411xe.h"

class Adafruit_ILI9341_NG;

extern volatile bool spiDmaTransferComplete;

class STMDMA {
 public:
  STMDMA(Adafruit_ILI9341_NG *display, uint16_t bufferSize, SPI_TypeDef *spi, DMA_Stream_TypeDef *dma);

  ~STMDMA();

  void begin();
  void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

  void beginWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t backgroundColor);
  void drawWindowPixel(int16_t x, int16_t y, uint16_t color);
  void flushWindow();
  void waitComplete();

 public:
  Adafruit_ILI9341_NG *_display;
  SPI_HandleTypeDef _spi;
  DMA_HandleTypeDef _dma;
  uint16_t _bufferSize;
  uint16_t *_buffer;
  uint16_t _fw, _fl;

 protected:
  uint16_t byteSwap(uint16_t w);
};
#endif