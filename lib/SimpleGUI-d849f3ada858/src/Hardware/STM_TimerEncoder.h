#ifndef SIMPLEGUI_STM_TIMER_ENCODER_H
#define SIMPLEGUI_STM_TIMER_ENCODER_H

#include "Core/FunctionPointer.h"
#include "timer.h"
#include <Arduino.h>

namespace simplegui {

/**
 * Attach encoder to PA8 and PA9, button to PA10
 **/
class STMTimerEncoder {
public:
    STMTimerEncoder() { }
    ~STMTimerEncoder() { }

    void begin()
    {

        __HAL_RCC_TIM1_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct = { 0 };

        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM1 GPIO Configuration
        PA8     ------> TIM1_CH1
        PA9     ------> TIM1_CH2
        */
        GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        TIM_Encoder_InitTypeDef sConfig = { 0 };
        TIM_MasterConfigTypeDef sMasterConfig = { 0 };

        /* USER CODE BEGIN TIM1_Init 1 */

        /* USER CODE END TIM1_Init 1 */
        htim1.Instance = TIM1;
        htim1.Init.Prescaler = 0;
        htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim1.Init.Period = 65535;
        htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = 0;
        htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
        sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
        sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC1Filter = 10;
        sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
        sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC2Filter = 10;
        if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK) {
            Error_Handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
            Error_Handler();
        }

        HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    }

    void tick()
    {
        if (_last != getCount()) {
            if (_last < getCount()) {
                _onDecrement.call();
            } else {
                _onIncrement.call();
            }
            _last = getCount();
            _onChange.call(_last);
        }

        bool p = digitalRead(PA10);

        if (p == _pressed && p == 0) {

            if ((millis() - _pressedAt) > 20) {
                // no change for the debounce period
                if (!_clicked) {
                    _onClick.call();
                    _clicked = true;
                }
            }

        } else if (p == 0) {
            _pressed = 0;
            _pressedAt = millis();
            _clicked = false;
        } else {
            _pressed = 1;
        }
    }

    int getCount()
    {
        return TIM1->CNT;
    }

    template <class T>
    void onChange(T* tptr, void (T::*mptr)(int))
    {
        _onChange.attach(tptr, mptr);
    }

    template <class T>
    void onClick(T* tptr, void (T::*mptr)())
    {
        _onClick.attach(tptr, mptr);
    }

    template <class T>
    void onIncrement(T* tptr, void (T::*mptr)())
    {
        _onIncrement.attach(tptr, mptr);
    }

    template <class T>
    void onDecrement(T* tptr, void (T::*mptr)())
    {
        _onDecrement.attach(tptr, mptr);
    }

protected:
    TIM_HandleTypeDef htim1;
    FunctionPointerArg1<void, int> _onChange;
    FunctionPointerArg1<void, void> _onIncrement;
    FunctionPointerArg1<void, void> _onDecrement;
    FunctionPointer _onClick;
    int _last = 0;
    bool _pressed = false;
    bool _clicked = false;
    uint32_t _pressedAt = 0;
};
}

#endif