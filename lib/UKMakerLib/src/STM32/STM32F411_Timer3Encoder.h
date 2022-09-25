#ifndef UKMAKER_STM_TIMER3_ENCODER_H
#define UKMAKER_STM_TIMER3_ENCODER_H

#include "STM32F411_TimerEncoder.h"

namespace ukmaker {

/**
 * Attach encoder to PA8 and PA9, button to e.g. PA10
 **/
class STM32F411_Timer3Encoder : public STM32F411_TimerEncoder {
public:
    STM32F411_Timer3Encoder(uint32_t buttonPin)
        : STM32F411_TimerEncoder(buttonPin)
    {
    }
    ~STM32F411_Timer3Encoder() { }

    TIM_TypeDef* getInstance()
    {
        return TIM3;
    }

    virtual void setupIO(bool enableInternalPullups)
    {
        STM32F411_TimerEncoder::setupIO(enableInternalPullups);
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };
        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PA6     ------> TIM3_CH1
        PA7     ------> TIM3_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        if(enableInternalPullups) {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
        } else {
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
};
}

#endif