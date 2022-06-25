#ifndef SIMPLEGUI_TIMER_H
#define SIMPLEGUI_TIMER_H

#include "FunctionPointer.h"
#include <Arduino.h>

namespace simplegui {
/**
 * Class to provide easy to use timing callbacks for the GUI
 * Does NOT use an actual hardware timer and interrupts
 * since the GUI needs to be single-threaded
 *
 */
class Timer {
public:
    Timer() { }
    ~Timer() { }

    template <class T>
    void setTimeout(uint32_t millisecs, T* tptr, void (T::*mptr)())
    {
        _callback.attach(tptr, mptr);
        _millis = millisecs;
        _start = millis();
        _ticking = true;
    }

    void setTimeout(uint32_t millisecs, void (*fptr)())
    {
        _callback.attach(fptr);
        _millis = millisecs;
        _start = millis();
        _ticking = true;
    }

    void tick()
    {
        if (_ticking && (millis() - _start) > _millis) {
            _callback.call();
            _ticking = false;
        }
    }

protected:
    bool _ticking = false;
    uint32_t _millis;
    uint32_t _start;
    FunctionPointer _callback;
};
}
#endif