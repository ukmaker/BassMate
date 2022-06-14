#ifndef BM_INSTRUMENTBANK_H
#define BM_INSTRUMENTBANK_H

#include "Instrument.h"
#include "Voice.h"

namespace bassmate {

/*
 * Class to wrap the instrument definitions for a bank
 * and act as a factory for Instrument structs
 */

class InstrumentBank {
 public:
  InstrumentBank(const Bank *b) : _bank(b) {}

  /*
   * Return the first Instrument in the family
   * If no match, return an Instrument with bankId of zero
   */
  Instrument getFirstFamilyInstrument(uint8_t familyId) {
    if (_bank != nullptr) {
      for (uint8_t f = 0; f < _bank->numFamilies; f++) {
        const Family family = getFamilyById(familyId);
        if (family.name > 0) {
          const Voice voice = family.voices[0];

          Instrument i = {voice.id, familyId, _bank->id, _bank->melodic, _bank->melodic ? _bank->defaultNote : voice.id};
          return i;
        }
      }
    }
    Instrument i = {0, 0, 0, false, 0};
    return i;
  }

  /*
   * Return an Instrument matching the id
   * If no match, return an Instrument with bankId of zero
   */
  Instrument getInstrumentById(uint8_t id) {
    if (_bank != nullptr) {
      for (uint8_t f = 0; f < _bank->numFamilies; f++) {
        const Family family = _bank->families[f];
        for (uint8_t v = 0; v < family.numVoices; v++) {
          const Voice voice = family.voices[v];
          if (voice.id == id) {
            Instrument i = {v, f, _bank->id, _bank->melodic,
                            _bank->melodic ? _bank->defaultNote : id};
            return i;
          }
        }
      }
    }
    Instrument i = {0, 0, 0, false, 0};
    return i;
  }
  /*
   * Return the Family of the Instrument matching the id
   * If no match, return nullptr
   */
  const Family getInstrumentFamily(uint8_t id) {
    if (_bank != nullptr) {
      for (uint8_t f = 0; f < _bank->numFamilies; f++) {
        const Family family = _bank->families[f];
        for (uint8_t v = 0; v < family.numVoices; v++) {
          const Voice voice = family.voices[v];
          if (voice.id == id) {
            return family;
          }
        }
      }
    }
    return {};
  }

  /*
   * Return nullptr if not found
   */
  const Voice getVoiceById(uint8_t id) {
    if (_bank != nullptr) {
      for (uint8_t f = 0; f < _bank->numFamilies; f++) {
        const Family family = _bank->families[f];
        for (uint8_t v = 0; v < family.numVoices; v++) {
          const Voice voice = family.voices[v];
          if (voice.id == id) {
            return voice;
          }
        }
      }
    }
    return {};
  }
  /*
   * Return nullptr if not found
   */
  const Family getFamilyById(uint8_t id) {
    if (_bank != nullptr) {
      for (uint8_t f = 0; f < _bank->numFamilies; f++) {
        const Family family = _bank->families[f];
        if (family.id == id) {
          return family;
        }
      }
    }
    return {};
  }

  const Bank *getBank() { return _bank; }

 protected:
  const Bank *_bank;
};
}  // namespace bassmate
#endif