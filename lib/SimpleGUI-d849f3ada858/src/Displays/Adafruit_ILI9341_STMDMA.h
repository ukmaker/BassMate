/*!
 * @file Adafruit_ILI9341_NG.h
 *
 * This is the documentation for Adafruit's ILI9341 driver for the
 * Arduino platform.
 *
 * This library works with the Adafruit 2.8" Touch Shield V2 (SPI)
 *    http://www.adafruit.com/products/1651
 * Adafruit 2.4" TFT LCD with Touchscreen Breakout w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/2478
 * 2.8" TFT LCD with Touchscreen Breakout Board w/MicroSD Socket - ILI9341
 *    https://www.adafruit.com/product/1770
 * 2.2" 18-bit color TFT LCD display with microSD card breakout - ILI9340
 *    https://www.adafruit.com/product/1770
 * TFT FeatherWing - 2.4" 320x240 Touchscreen For All Feathers
 *    https://www.adafruit.com/product/3315
 *
 * These displays use SPI to communicate, 4 or 5 pins are required
 * to interface (RST is optional).
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 *
 * This library depends on <a href="https://github.com/adafruit/Adafruit_GFX">
 * Adafruit_GFX</a> being present on your system. Please make sure you have
 * installed the latest version before using this library.
 *
 * Written by Limor "ladyada" Fried for Adafruit Industries.
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#ifndef SIMPLEGUI_ADAFRUIT_ILI9341_STMDMA_H
#define SIMPLEGUI_ADAFRUIT_ILI9341_STMDMA_H

#include "Adafruit_ILI9341.h"
#include "STMDMA.h"

namespace simplegui {

class Adafruit_ILI9341_STMDMA : public Adafruit_ILI9341 {
public:
    Adafruit_ILI9341_STMDMA(SPIClass* spiClass, int8_t dc, int8_t cs = -1,
        int8_t rst = -1);

    void setSTMDMA(STMDMA* stmdma);

    // override
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
        uint16_t color);

    virtual inline void writeFillRectPreclipped(int16_t x, int16_t y, int16_t w,
        int16_t h, uint16_t color);

protected:
    STMDMA* _stmdma;
};

}

#endif // SIMPLEGUI_ADAFRUIT_ILI9341_STMDMA_H
