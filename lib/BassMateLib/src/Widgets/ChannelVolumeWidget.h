#ifndef BM_CHANNELVOLUME_WIDGET_H
#define BM_CHANNELVOLUME_WIDGET_H

#include "Renderers/VolumeBarRenderer.h"
#include "Widgets/Widget.h"

namespace bassmate {

using namespace simplegui;

template <int N>
class ChannelVolumeWidget : public Widget {
  static const uint16_t ZONE_VOLS = 0;
  static const uint16_t ZONE_TEXT = 8;

 public:
  ChannelVolumeWidget(GraphicsContext *context)
      : Widget(context), _channels(N) {
    for (uint8_t i = 0; i < N; i++) {
      _percent[i] = 0;
      _labels[i] = "";
      _dirtyContent = 0xffff;
    }
  }

  void setPercent(uint8_t channel, uint8_t percent) {
    if (channel > N) return;
    if (percent > 100) percent = 100;
    if (_percent[channel] != percent) {
      _percent[channel] = percent;
      noteDirtyContent(1 << (channel + ZONE_VOLS));
    }
  }

  uint8_t getPercent(uint8_t channel) {
    if (channel > _channels) return 0;
    return _percent[channel];
  }

  uint8_t getSelectedPercent() {
    return _percent[_selected];
  }

  void up() {
    if(_selected > 0) {
      setSelected(_selected - 1);
    }
  }

  void down() {
    if(_selected < N-1) {
      setSelected(_selected + 1);
    }
  }

  void increment() {
    setPercent(_selected, _percent[_selected] + _step);
  }

  void decrement() {
    setPercent(_selected,
               _percent[_selected] > _step ? _percent[_selected] - _step : 0);
  }

  void setChannelLabel(uint8_t channel, const char *label) {
    if (channel > _channels) return;
    if (_labels[channel] != label) {
      _labels[channel] = label;
      noteDirtyContent((1 << (channel + ZONE_TEXT)));
    }
  }

  void setStep(uint8_t step) {
    if (step > 127) return;
    if (_step != step) {
      _step = step;
    }
  }

  uint8_t getStep() { return _step; }

  void setTextWidth(int w) {
    if (_textWidth != w) {
      _textWidth = w;
      noteDirtyContent(0xffff);
    }
  }

  int getTextWidth() { return _textWidth; }

  void setTextMargin(int m) {
    if (_textMargin != m) {
      _textMargin = m;
      noteDirtyContent(0xffff);
    }
  }

  int getTextMargin() { return _textMargin; }

  void setTextLeft() {
    if (_textLeft == false) {
      _textLeft = true;
      noteDirtyContent(0xffff);
    }
  }

  void setTextRight() {
    if (_textLeft == true) {
      _textLeft = false;
      noteDirtyContent(0xffff);
    }
  }

  void setSelected(uint8_t c) {
    if (c > _channels) return;
    if (c != _selected) {
      noteDirtyContent((1 << (_selected + ZONE_TEXT)) +
                       (1 << _selected + ZONE_VOLS) + (1 << (c + ZONE_TEXT)) +
                       (1 << (c + ZONE_VOLS)));
      _selected = c;
    }
  }

  int getSelected() { return _selected; }

  void setSelectedColor(uint16_t c) {
    if (_selectedColor != c) {
      _selectedColor = c;
      noteDirtyContent(0xffff);
    }
  }

  uint16_t getHighlightColor() { return _selectedColor; }

 protected:
  uint8_t _percent[N];
  uint8_t _step = 1;
  uint8_t _channels;
  uint8_t _selected = 0;
  uint16_t _selectedColor = 0;
  const char *_labels[N];
  int _textWidth = 0;
  int _textMargin = 0;
  bool _textLeft = true;

  virtual void _clearContent(bool force) {
    if (force) {
      Widget::_clearContent(true);
    } else {
      fontRenderer()->setFont(_font);
      fontRenderer()->setTextColor(_fg, _bg);

      int fontHeight = fontRenderer()->getFontHeight();

      int barLeft, barWidth, barHeight, channelHeight, channelTopOffset;
      int textLeft;

      channelHeight = _inner.height() / N;
      channelTopOffset = (channelHeight - fontHeight) / 2;

      barWidth = _inner.width() - _textWidth - _textMargin;
      barHeight = fontHeight;

      if (_textLeft) {
        barLeft = _inner.x() + _textWidth + _textMargin;
        textLeft = _inner.x();
      } else {
        barLeft = _inner.x();
        textLeft = barLeft + barWidth + _textMargin;
      }

      for (int i = 0; i < _channels; i++) {
        uint16_t fg;
        if (hasFocus() && _selected == i) {
          fg = _selectedColor;
        } else {
          fg = _fg;
        }

        int dy = i * channelHeight;
        int textTop = _inner.top() + dy + channelTopOffset;
        int barTop = _inner.top() + dy + channelTopOffset;
        int barBottom = barTop + barHeight - 1;

        if (_dirtyContent & (1 << (i + ZONE_TEXT))) {
          display()->fillRect(textLeft, textTop, _textWidth, fontHeight, _bg);
        }
        if (_dirtyContent & (1 << (i + ZONE_VOLS))) {
          display()->fillRect(barLeft, barTop, barWidth, barHeight, _bg);
        }
      }
    }
  }

  /*
   * override
   * Draw label on the left, then a volume bar
   */
  virtual void _drawContent(bool force) {
    Adafruit_GFX *d = display();
    const GFXfont *f = fontRenderer()->getFont();
    if (_font != nullptr) {
      fontRenderer()->setFont(_font);
    }
    fontRenderer()->setTextColor(_fg, _bg);

    int fontHeight = fontRenderer()->getFontHeight();

    int barLeft, barWidth, barHeight, channelHeight, channelTopOffset;
    int textLeft;

    channelHeight = _inner.height() / N;
    channelTopOffset = (channelHeight - fontHeight) / 2;

    barWidth = _inner.width() - _textWidth - _textMargin;
    barHeight = fontHeight;

    if (_textLeft) {
      barLeft = _inner.x() + _textWidth + _textMargin;
      textLeft = _inner.x();
    } else {
      barLeft = _inner.x();
      textLeft = barLeft + barWidth + _textMargin;
    }

    for (int i = 0; i < _channels; i++) {
      uint16_t fg;
      if (hasFocus() && _selected == i) {
        fg = _selectedColor;
      } else {
        fg = _fg;
      }

      int dy = i * channelHeight;
      int textTop = _inner.top() + dy + channelTopOffset;
      int barTop = _inner.top() + dy + channelTopOffset;
      int barBottom = barTop + barHeight - 1;

      if (force || _dirty || (_dirtyContent & (1 << (i + ZONE_TEXT))))
        _drawText(_labels[i], textLeft, textTop, _textWidth, fontHeight, fg);

      if (force || (_dirtyContent & (1 << (i + ZONE_VOLS))) || _dirty) {
        d->fillRect(barLeft, barTop, barWidth, barHeight, _bg);
        VolumeBarRenderer::render(_context, barLeft, barTop, barWidth,
                                  barHeight, _percent[i], fg);
      }
    }
    fontRenderer()->setFont(f);
  }

  virtual void _drawText(const char *t, int x, int y, int w, int h,
                         uint16_t c) {
    fontRenderer()->setTextWindow(x, y, w, h);
    fontRenderer()->setTextColor(c, _bg);
    fontRenderer()->print(t);
  }
};

}  // namespace bassmate

#endif