#ifndef SIMPLEGUI_PPSWIDGET_H
#define SIMPLEGUI_PPSWIDGET_H
#include "Widgets/Widget.h"

class PlayPauseStopWidget : public Widget
{

public:
    static const uint8_t STOP = 0;
    static const uint8_t PAUSE = 1;
    static const uint8_t PLAY = 2;

    PlayPauseStopWidget(GraphicsContext *context) : Widget(context) {}
    ~PlayPauseStopWidget() {}

    void setState(uint8_t state)
    {
        if (state > 2)
            return;

        if (_state != state)
        {
            _state = state;
            noteDirtyContent(1);
        }
    }

protected:
    uint8_t _state = STOP;

    virtual void _drawContent(bool force = false)
    {

        switch (_state)
        {
        case STOP:
            display()->fillRect(_inner.left(), _inner.top(), _inner.width, _inner.height, _fg);
            break;

        case PAUSE:
            display()->fillRect(_inner.left(), _inner.top(), _inner.width / 3, _inner.height, _fg);
            display()->fillRect(_inner.left() + _inner.width * 2 / 3, _inner.top(), _inner.width / 3, _inner.height, _fg);
            break;

        case PLAY:
            display()->fillTriangle(
                _inner.left(), _inner.top(),
                _inner.left(), _inner.bottom(),
                _inner.right(), _inner.top() + _inner.height / 2, _fg);
            break;

        default:
            break;
        }
    }
};
#endif