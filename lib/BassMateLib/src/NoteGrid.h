#ifndef BM_NOTE_GRID_H
#define BM_NOTE_GRID_H

#include "Adafruit_NeoTrellis.h"

#define NG_GLOW_TICKS 25

#define NG_NOTE_UNASSIGNED 0
#define NG_NOTE_ASSIGNED 1

#define NG_NOTE_UNASSIGNED_COLOUR 0x000000
#define NG_NOTE_ASSIGNED_COLOUR 0x00ff00

#define NG_NOTE_UNASSIGNED_COLOUR_H 0x000088
#define NG_NOTE_ASSIGNED_COLOUR_H 0x00ff88

#define NG_BEAT_FLASH_COLOUR 0x222200
#define NG_NOTE_HIGHLIGHT_COLOUR 0x000022
#define NG_VELOCITY_BLINK_COLOUR 0xff00aa

#define NG_BLINK_TICKS 250

typedef void (*KeyPressCallback)(uint8_t x, uint8_t y);

class NoteGrid {
 public:
  NoteGrid(Adafruit_MultiTrellis &trellis, int interruptPin = -1)
      : _trellis(trellis),
        _flashBeat(false),
        _lastKeyPressed(-1),
        _intPin(interruptPin) {
    for (int i = 0; i < 4; i++) {
      _highlightLine[i] = false;
    }
  }

  void attachEventHandler(TrellisCallback cb) { _eventHandler = cb; }

  void attachKeyPressCallback(KeyPressCallback cb) { _keyPressCallback = cb; }

  void attachKeyLongPressCallback(KeyPressCallback cb) {
    _keyLongPressCallback = cb;
  }

  // crufty way to get long-press handling in an instance
  TrellisCallback handleEvent(keyEvent e) {
    // RISING edge means the button has been pressed
    if (e.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
      // handle the normal key-press here
      if (_keyPressCallback != NULL) {
        int x = e.bit.NUM % 8;
        int y = e.bit.NUM / 8;
        _keyPressCallback(x, y);
      }
      _lastKeyPressed = e.bit.NUM;
      _lastKeyPressedTime = millis();
    }

    // FALLING edge means the button has been released
    if (e.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
      _lastKeyPressed = -1;
    }

    return 0;
  }

  void begin() {
    for (uint8_t y = 0; y < 4; y++) {
      for (uint8_t x = 0; x < 8; x++) {
        // activate rising and falling edges on all keys
        _trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_RISING, true);
        _trellis.activateKey(x, y, SEESAW_KEYPAD_EDGE_FALLING, true);
        _trellis.registerCallback(x, y, _eventHandler);
        _notes[y][x] = 0x000000;
        _trellis.setPixelColor(x, y, 0xff00aa);  // addressed with x,y
      }
      _trellis.show();
      delay(100);
      for (int x = 0; x < 8; x++) {
        _trellis.setPixelColor(x, y, 0x000000);  // addressed with x,y
      }
      _trellis.show();  // show all LEDs
    }
  }

  void setFlashBeat(bool flash) { _flashBeat = flash; }

  bool getFlashBeat() { return _flashBeat; }

  void setActive(uint8_t x, uint8_t y) {
    _notes[y][x] = true;
    _showNote(x, y);
  }

  void setInactive(uint8_t x, uint8_t y) {
    _notes[y][x] = false;
    _showNote(x, y);
  }

  void blink(uint8_t x, uint8_t y) {
    _blinkX = x;
    _blinkY = y;
    _blinkTicks = 0;
  }

  void cancelBlink() { _blinkX = -1; }

  void restoreLine(int line) {
    for (int i = 0; i < 4; i++) {
      _highlightLine[i] = false;
    }

    for (int x = 0; x < 8; x++) {
      _updateNote(x, line);
    }

    _trellis.show();
  }

  void restoreLines() {
    for (int i = 0; i < 4; i++) {
      _highlightLine[i] = false;
    }

    for (int x = 0; x < 8; x++) {
      _updateNote(x, 0);
      _updateNote(x, 1);
      _updateNote(x, 2);
      _updateNote(x, 3);
    }
    _trellis.show();
  }

  void selectLine(int line) {
    for (int i = 0; i < 4; i++) {
      if (i != line && _highlightLine[i]) {
        _highlightLine[i] = false;
        for (int x = 0; x < 8; x++) {
          _updateNote(x, i);
        }
      }
    }
    _highlightLine[line] = true;
    for (int x = 0; x < 8; x++) {
      _updateNote(x, line);
    }
    _trellis.show();
  }

  void beat(int column) {
    _ticks = NG_GLOW_TICKS * _beatFadeTimePercent / 100;
    _lastBeat = _beat;
    _beat = column;
  }

  void showPausedBeat(int column) {
    _ticks = NG_GLOW_TICKS * _beatFadeTimePercent / 100;
    _lastBeat = _beat;
    _beat = column;
    //    _pausedBeat = true;
  }

  void setBeatFadeTimePercent(uint8_t percent) {
    _beatFadeTimePercent = percent;
  }

  void tick() {
    uint32_t nc, now;

    now = millis();

    if (_lastKeyPressed != -1) {
      if ((now - _lastKeyPressedTime) > 500) {
        if (_keyLongPressCallback != NULL) {
          int x = _lastKeyPressed % 8;
          int y = _lastKeyPressed / 8;
          _keyLongPressCallback(x, y);
          _lastKeyPressed = -1;
        }
      }
    }

    if (_blinkX != -1) {
      if (_blinkTicks > NG_BLINK_TICKS) {
        _blinkTicks = 0;
      }

      nc = pixelMixColour(_blinkX, _blinkY, NG_VELOCITY_BLINK_COLOUR,
                          _blinkTicks, NG_BLINK_TICKS);
      _trellis.setPixelColor(_blinkX, _blinkY, nc);
      _trellis.show();
      _blinkTicks++;
    }

    if (_flashBeat) {
      if (_ticks > 0 && _beat >= 0 && _beat < 8) {
        _ticks--;
        if ((_ticks & 1) == 0) {
          for (int i = 0; i < 4; i++) {
            // not too bright for the flash, and brighter the faster it goes
            uint8_t brightness = _ticks / 6;
            if(_ticks > 0) brightness++; // Make sure there's something for short flashes
            nc = pixelMixColour(_beat, i, NG_BEAT_FLASH_COLOUR, brightness,
                                NG_GLOW_TICKS * _beatFadeTimePercent / 100);
            _trellis.setPixelColor(_beat, i, nc);
          }
          _trellis.show();
          delayMicroseconds(300); // allow the NeoTrellis time to latch the show command
        }
      }
    }

    if (_intPin == -1) {
      _trellis.read();
    } else {
      if (digitalRead(_intPin) == LOW) {
        _trellis.read(false);
      }
    }
  }

  uint32_t pixelMixColour(uint8_t x, uint8_t y, uint32_t colour, uint32_t per,
                          uint32_t cent) {
    uint32_t nr, ng, nb, mr, mg, mb, tr, tg, tb;
    uint32_t nc = _noteColour(x, y);
    uint32_t percent = (100 * per) / cent;

    nr = ((nc & 0xff0000) >> 16);
    ng = ((nc & 0x00ff00) >> 8);
    nb = ((nc & 0x0000ff));

    mr = ((colour & 0xff0000) >> 16);
    mg = ((colour & 0x00ff00) >> 8);
    mb = (colour & 0x0000ff);

    // now mix the colours
    // 0% -> original colour
    // 50% -> mix colour
    // 100% -> back to original colour
    if (percent > 50) {
      percent = 100 - percent;
    }

    percent = 2 * percent;
    tr = ((mr * percent) + (nr * (100 - percent))) / 100;

    if (tr > 0xff) tr = 0xff;

    tg = ((mg * percent) + (ng * (100 - percent))) / 100;
    if (tg > 0xff) tg = 0xff;

    tb = ((mb * percent) + (nb * (100 - percent))) / 100;
    if (tb > 0xff) tb = 0xff;

    return (tr << 16) + (tg << 8) + tb;
  }

 protected:
  Adafruit_MultiTrellis &_trellis;
  TrellisCallback _eventHandler;
  KeyPressCallback _keyPressCallback, _keyLongPressCallback;
  int _intPin;

  bool _notes[4][8];

  // 0 means no highlight
  bool _highlightLine[4];
  bool _flashBeat;

  int _blinkX = -1;
  int _blinkY = -1;
  uint32_t _blinkTicks = 0;
  uint8_t _beatFadeTimePercent = 100;

  uint32_t _ticks, _lastKeyPressedTime;
  int _beat, _lastBeat, _lastKeyPressed = -1;

  void _showNote(uint8_t x, uint8_t y) {
    _updateNote(x, y);
    _trellis.show();
  }

  void _updateNote(uint8_t x, uint8_t y) {
    uint32_t colour = _noteColour(x, y);
    _trellis.setPixelColor(x, y, colour);
  }

  uint32_t _noteColour(uint8_t x, uint8_t y) {
    if (_highlightLine[y]) {
      if (_notes[y][x]) {
        return NG_NOTE_ASSIGNED_COLOUR_H;
      } else {
        return NG_NOTE_UNASSIGNED_COLOUR_H;
      }
    }

    if (_notes[y][x]) {
      return NG_NOTE_ASSIGNED_COLOUR;
    } else {
      return NG_NOTE_UNASSIGNED_COLOUR;
    }
  }
};
#endif
