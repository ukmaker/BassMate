#ifndef BM_MODEL_H
#define BM_MODEL_H

#include "Sequencer.h"
#include "VS1053/VS1053.h"
#include "i2cNavKey.h"
#include "Storage.h"
#include "AD5204.h"

namespace bassmate {

class Model {
 public:
  Model(i2cNavKey &navkey, uint8_t navKeyInterrupt, VS1053 &midi,
        Sequencer &sequencer, Storage *storage, AD5204 &digipot)
      : _navkey(navkey),
        _nki(navKeyInterrupt),
        _midi(midi),
        _sequencer(sequencer),
        _digipot(digipot)
        {
        _storage = storage;
        }

  ~Model() {}

  void begin() {
    _midi.enableMIDI();
    delay(100);

    // midi.loadUserCode(RTMIDI_CODE_SIZE, RTMIDI_atab, RTMIDI_dtab);

    _midi.setBank(0, MIDI_BANK_PERCUSSION);
    _midi.setBank(1, MIDI_BANK_PERCUSSION);
    _midi.setBank(2, MIDI_BANK_PERCUSSION);
    _midi.setBank(3, MIDI_BANK_PERCUSSION);
    _midi.setInstrument(0, 35);
    _midi.setInstrument(1, 42);
    _midi.setInstrument(2, 31);
    _midi.setInstrument(3, 28);
    _midi.setVolume(254);
    _digipot.digitalPotWrite(2, 128);
  }

  void run() {
    _sequencer.tick();
    if (digitalRead(_nki) == LOW) {
      _navkey.updateStatus();
    }
  }

 public:
  i2cNavKey &_navkey;
  uint8_t _nki;
  VS1053 &_midi;
  Sequencer &_sequencer;
  Storage *_storage;
  AD5204 &_digipot;
};

}  // namespace bassmate
#endif