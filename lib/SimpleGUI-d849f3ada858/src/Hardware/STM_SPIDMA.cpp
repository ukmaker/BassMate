#include "STM_SPIDMA.h"

namespace simplegui {

/***
 * Note that this implementation uses DMA1 Stream 4 for SPI2
 * If you need to use different DMA or SPI, you'll need to
 * modify the code appropriately and implement the corresponding
 * interrupt handlers
 *
 *
 * Your main.cpp should look something like this:
 * 
 
    volatile bool spiDmaTransferComplete = true;
    STM_SPIDMA stmdma(&tft, 32768, SPI2, DMA1_Stream4);

    extern "C" {
    void DMA1_Stream4_IRQHandler()
    {
        HAL_DMA_IRQHandler(&stmdma._dma);
        // spiDmaTransferComplete = true;
    }
    }

    extern "C" {
    void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
    {
        spiDmaTransferComplete = true;
    }
    }
*
*
* And in your setup routine you'll need:
*
    tft.setDMA(&stmdma);
    view.fontRenderer->setDMA(&stmdma);
    HAL_DMA_RegisterCallback(&stmdma._dma, HAL_DMA_XFER_CPLT_CB_ID, DMACallback);
    spiDmaTransferComplete = true;
    stmdma.begin();
 ***/

STM_SPIDMA::STM_SPIDMA(Adafruit_SPITFT* display, uint16_t bufferSize,
    SPI_TypeDef* spi, DMA_Stream_TypeDef* dma)
    : _display(display)
{
    // Make sure our uint16_t state doesn't get overflowed
    if (bufferSize > 32767)
        bufferSize = 32767;
    _bufferSize = bufferSize;
    _buffer = new uint16_t[bufferSize];
    _spi.Instance = spi;
    _dma.Instance = dma;
    _fw = _fl = 0;
}

STM_SPIDMA::~STM_SPIDMA() { }

void STM_SPIDMA::begin()
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Stream4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

    // Wire up the SPI
    _spi.Init.Mode = SPI_MODE_MASTER;
    _spi.Init.Direction = SPI_DIRECTION_2LINES;
    _spi.Init.DataSize = SPI_DATASIZE_16BIT;
    _spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    _spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    _spi.Init.NSS = SPI_NSS_HARD_OUTPUT;
    _spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    _spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    _spi.Init.TIMode = SPI_TIMODE_DISABLE;
    _spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    _spi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&_spi) != HAL_OK) {
        Error_Handler();
    }
    /*
      // Wire up the DMA
      /* SPI2 DMA Init */
    /* SPI2_TX Init */
    _dma.Instance = DMA1_Stream4;
    _dma.Init.Channel = DMA_CHANNEL_0;
    _dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    _dma.Init.PeriphInc = DMA_PINC_DISABLE;
    _dma.Init.MemInc = DMA_MINC_ENABLE;
    _dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    _dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    _dma.Init.Mode = DMA_NORMAL;
    _dma.Init.Priority = DMA_PRIORITY_LOW;
    _dma.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    //_dma.XferCpltCallback = complete;
    if (HAL_DMA_Init(&_dma) != HAL_OK) {
        Error_Handler();
    }

    __HAL_LINKDMA(&_spi, hdmatx, _dma);
}

void STM_SPIDMA::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t color)
{
    uint32_t total = w * h;
    uint16_t blocks = total / _bufferSize;
    color = byteSwap(color);
    if (total < _bufferSize) {
        for (uint16_t i = 0; i < total; i++)
            _buffer[i] = color;
    } else {
        for (uint16_t i = 0; i < _bufferSize; i++)
            _buffer[i] = color;
    }
    while (blocks > 0) {
        while (!spiDmaTransferComplete)
            ;
        spiDmaTransferComplete = false;
        HAL_SPI_Transmit_DMA(&_spi, (uint8_t*)_buffer, _bufferSize * 2);
        blocks--;
        total -= _bufferSize;
    }

    if (total > 0) {
        while (!spiDmaTransferComplete)
            ;
        spiDmaTransferComplete = false;
        HAL_SPI_Transmit_DMA(&_spi, (uint8_t*)_buffer, total * 2);
    }
    while (!spiDmaTransferComplete)
        ;
}

void STM_SPIDMA::beginWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
    uint16_t backgroundColor)
{
    _fw = w;
    _fl = w * h;
    backgroundColor = byteSwap(backgroundColor);
    for (uint16_t i = 0; i < _fl; i++)
        _buffer[i] = backgroundColor;
    _display->startWrite();
    _display->setAddrWindow(x, y, w, h);
}

void STM_SPIDMA::drawWindowPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x >= 0 && y >= 0)
        _buffer[x + (y * _fw)] = byteSwap(color);
}

void STM_SPIDMA::flushWindow()
{
    while (!spiDmaTransferComplete)
        ;
    if (_fl > 0) {
        spiDmaTransferComplete = false;
        HAL_SPI_Transmit_DMA(&_spi, (uint8_t*)_buffer, _fl * 2);
    }
}

void STM_SPIDMA::waitComplete()
{
    while (!spiDmaTransferComplete)
        ;
}

uint16_t STM_SPIDMA::byteSwap(uint16_t w)
{
    uint8_t hb = w >> 8;
    w = hb | (w << 8);
    return w;
}
}