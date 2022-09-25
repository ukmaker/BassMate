#ifndef UKMAKER_STM_TIMER_ENCODER_H
#define UKMAKER_STM_TIMER_ENCODER_H

#include "Base/FunctionPointer.h"
#include "timer.h"
#include <Arduino.h>

namespace ukmaker {

class STM32F411_TimerEncoder {
public:
    STM32F411_TimerEncoder(uint32_t buttonPin) { 
        _button = buttonPin;
    }

    ~STM32F411_TimerEncoder() { }

    virtual TIM_TypeDef *getInstance()=0;

    virtual void setupIO(bool enableInternalPullups = true) {
        if(enableInternalPullups) {
            pinMode(_button, INPUT_PULLUP);
        } else {
            pinMode(_button, INPUT);
        }
    }

    void begin()
    {
        TIM_Encoder_InitTypeDef sConfig = { 0 };
        TIM_MasterConfigTypeDef sMasterConfig = { 0 };

        htim.Instance = getInstance();
        htim.Init.Prescaler = 0;
        htim.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim.Init.Period = 65535;
        htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        htim.Init.RepetitionCounter = 0;
        htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
        sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
        sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
        sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC1Filter = 10;
        sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
        sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
        sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
        sConfig.IC2Filter = 10;
        if (HAL_TIM_Encoder_Init(&htim, &sConfig) != HAL_OK) {
            Error_Handler();
        }
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        if (HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig) != HAL_OK) {
            Error_Handler();
        }

        HAL_TIM_Encoder_Start(&htim, TIM_CHANNEL_ALL);
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

        bool p = digitalRead(_button);

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
        return getInstance()->CNT;
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
    TIM_HandleTypeDef htim;
    FunctionPointerArg1<void, int> _onChange;
    FunctionPointerArg1<void, void> _onIncrement;
    FunctionPointerArg1<void, void> _onDecrement;
    FunctionPointer _onClick;
    int _last = 0;
    bool _pressed = false;
    bool _clicked = false;
    uint32_t _pressedAt = 0;
    uint32_t _button;
};
}

#endif