#include "Adafruit_ILI9341_STMDMA.h"

namespace simplegui {

Adafruit_ILI9341_STMDMA::Adafruit_ILI9341_STMDMA(SPIClass* spiClass, int8_t dc,
    int8_t cs, int8_t rst)
    : Adafruit_ILI9341(spiClass, dc, cs, rst)
{
}

void Adafruit_ILI9341_STMDMA::setSTMDMA(STMDMA* stmdma) { _stmdma = stmdma; }

void Adafruit_ILI9341_STMDMA::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
    uint16_t color)
{
    startWrite();
    setAddrWindow(x, y, w, h);
    _stmdma->fillRect(x, y, w, h, color);
    endWrite();
}

void Adafruit_ILI9341_STMDMA::writeFillRectPreclipped(int16_t x, int16_t y, int16_t w, int16_t h,
    uint16_t color)
{
    setAddrWindow(x, y, w, h);
    _stmdma->fillRect(x, y, w, h, color);
}
}