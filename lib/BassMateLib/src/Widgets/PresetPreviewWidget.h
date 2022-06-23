#ifndef BM_PRESET_PREVIEW_WIDGET_H
#define BM_PRESET_PREVIEW_WIDGET_H
#include "Storage.h"
#include "Widgets/Widget.h"

namespace bassmate {
class PresetPreviewWidget : public Widget {
public:
    PresetPreviewWidget(GraphicsContext* context)
        : Widget(context)
    {
    }
    ~PresetPreviewWidget() { }

    void setPreset(Storage::Preset& preset)
    {
        _preset = preset;
        noteDirty();
    }

protected:
    Storage::Preset _preset;

    virtual void _drawContent(bool force)
    {
        uint16_t w = _inner.width / 12;
        uint16_t h = _inner.height / 12;
        if (w > h) {
            w = h;
        } else {
            h = w;
        }
        uint16_t r = w / 4;

        uint16_t dx = _inner.width / 9;
        uint16_t dy = _inner.height / 5;
        if (dx > dy)
            dx = dy;

        uint16_t y0 = _inner.top() + dy;
        for (uint8_t y = 0; y < 4; y++) {
            uint16_t x0 = _inner.left() + dx;
            for (uint8_t x = 0; x < 8; x++) {
                if (_preset.lines[y].notes[x].note != 0) {
                    display()->fillRoundRect(x0, y0, w, h, r, _fg);
                } else {
                    display()->fillRoundRect(x0, y0, w, h, r, _bg);
                }
                x0 += dx;
            }
            y0 += dy;
        }
    }
};

} // namespace bassmate
#endif