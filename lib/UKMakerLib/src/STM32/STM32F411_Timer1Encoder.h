#ifndef UKMAKER_STM_TIMER1_ENCODER_H
#define UKMAKER_STM_TIMER1_ENCODER_H

#include "STM32F411_TimerEncoder.h"

namespace ukmaker {

/**
 * Attach encoder to PA8 and PA9, button to e.g. PA10
 **/
class STM32F411_Timer1Encoder : public STM32F411_TimerEncoder {
public:
    STM32F411_Timer1Encoder(uint32_t buttonPin)
        : STM32F411_TimerEncoder(buttonPin)
    {
    }
    ~STM32F411_Timer1Encoder() { }

    TIM_TypeDef* getInstance()
    {
        return TIM1;
    }

    virtual void setupIO(bool enableInternalPullups)
    {
        STM32F411_TimerEncoder::setupIO(enableInternalPullups);
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };
        __HAL_RCC_TIM1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        PA9     ------> TIM1_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        if (enableInternalPullups) {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
        } else {
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
};
}

#endif