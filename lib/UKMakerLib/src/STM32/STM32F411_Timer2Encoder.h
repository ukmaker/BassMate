#ifndef UKMAKER_STM_TIMER2_ENCODER_H
#define UKMAKER_STM_TIMER2_ENCODER_H

#include "STM32F411_TimerEncoder.h"

namespace ukmaker {

/**
 * Attach encoder to PA8 and PA9, button to e.g. PA10
 **/
class STM32F411_Timer2Encoder : public STM32F411_TimerEncoder {
public:
    STM32F411_Timer2Encoder(uint32_t buttonPin)
        : STM32F411_TimerEncoder(buttonPin)
    {
    }
    ~STM32F411_Timer2Encoder() { }

    TIM_TypeDef* getInstance()
    {
        return TIM2;
    }

    virtual void setupIO(bool enableInternalPullups)
    {
        STM32F411_TimerEncoder::setupIO(enableInternalPullups);
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM2 GPIO Configuration
        PA0-WKUP     ------> TIM2_CH1
        PA1     ------> TIM2_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        if(enableInternalPullups) {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
        } else {
            GPIO_InitStruct.Pull = GPIO_NOPULL;
        }
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
};
}

#endif