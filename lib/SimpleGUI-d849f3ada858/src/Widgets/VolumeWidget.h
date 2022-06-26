#ifndef SIMPLEGUI_VOLUME_WIDGET_H
#define SIMPLEGUI_VOLUME_WIDGET_H

#include "Renderers/VolumeBarRenderer.h"
#include "Widgets/Widget.h"
namespace simplegui {
class VolumeWidget : public Widget {
public:
    VolumeWidget(GraphicsContext* context)
        : Widget(context)
    {
    }

    static const uint8_t LINEAR = 0;
    static const uint8_t LOG = 1;
    static const uint8_t ANTILOG = 2;

    static const uint16_t ZONE_VOL = 1;
    static const uint16_t ZONE_LABEL = 2;
    static const uint16_t ZONE_ALL = 4;

    uint8_t getValue()
    {
        if (_muted)
            return 0;

        switch (_mode) {
        case LOG:
            if (_percent < 70) {
                return _percent;
            }
            return (_percent * _percent) / 100;
            break;

        case ANTILOG:
            if (_percent < 96) {
                return (uint8_t)((uint32_t)(_percent * _percent * _percent) / (96 * 96));
            }
            return _percent;
            break;

        case LINEAR:
        default:
            return _percent;
        }
    }

    void setPercent(uint8_t percent)
    {
        if (percent > 100)
            percent = 100;

        if (_percent != percent) {
            _percent = percent;
            noteDirtyContent(ZONE_VOL);
            _onChange.call(this);
        }
    }

    uint8_t getPercent() { return _percent; }

    void increment() { setPercent(_percent + _step); }

    void decrement() { setPercent(_percent > _step ? _percent - _step : 0); }

    void setStep(uint8_t step)
    {
        if (step > 127)
            return;
        if (_step != step) {
            _step = step;
        }
    }

    uint8_t getStep() { return _step; }

    void setMode(uint8_t mode)
    {
        if (mode < 3)
            _mode = mode;
    }

    uint8_t getMode() { return _mode; }

    void toggleMute()
    {
        _muted = !_muted;
        noteDirtyContent(ZONE_VOL);
        _onChange.call(this);
    }

    bool isMuted()
    {
        return _muted;
    }

protected:
    virtual void _clearContent(bool force = false)
    {
        if (force || (_dirtyContent & ZONE_ALL)) {
            Widget::_clearContent(true);
        } else {
            if (_dirtyContent & ZONE_VOL) {
                // can't be bothered to figure this out for now
                // just clear the whole area
                Widget::_clearContent(true);
            }
        }
    }
    /*
     * override
     * Draw label on the left, then a volume bar
     */
    virtual void _drawContent(bool force = false)
    {
        Adafruit_GFX* d = display();

        // split the width 30% speaker 10% gap 60% volume bar
        int speakerWidth = ((_inner.width() * 3) / 10) - 1;
        int barWidth = (_inner.width() * 6) / 10;
        int gap = _inner.width() - speakerWidth - barWidth;
        int squareSide = (speakerWidth * 5) / 10;

        int x = _inner.left();
        int y = _inner.top() + (_inner.height() - squareSide) / 2;

        // draw the wee speaker always
        d->fillRect(x, y, squareSide, squareSide, _fg);
        d->fillTriangle(_inner.left(), _inner.top() + _inner.height() / 2,
            _inner.left() + speakerWidth, _inner.top() + _inner.height(),
            _inner.left() + speakerWidth, _inner.top(), _fg);

        if (_muted) {
            // draw an X
            x = _inner.left() + speakerWidth + 4;
            y = _inner.top() + (_inner.height() - speakerWidth) / 2;
            d->drawLine(x, y, x + speakerWidth, y + speakerWidth - 2, _fg);
            d->drawLine(x, y + speakerWidth - 2, x + speakerWidth, y, _fg);
        } else {
            // draw the volume bar
            int left = _inner.left() + speakerWidth + gap;
            VolumeBarRenderer::render(_context, left, _inner.top(), barWidth,
                _inner.height(), _percent, _fg);
        }
    }

protected:
    uint8_t _percent = 0;
    uint8_t _step = 1;
    uint8_t _mode = LINEAR;
    bool _muted = false;
};
}
#endif