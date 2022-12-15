#include "HardwareSerial.h"
#include "InstrumentBank.h"
#include "MIDI.h"
#include "MelodicVoices.h"
#include "PercussionVoices.h"
#include "SPI.h"
#include "Voice.h"
#ifndef VS1053_H
#define VS1053_H

namespace bassmate {

#define VS_LINE1_IN 1
#define VS_LINE2_IN 2

#define VS_INSTRUCTION_READ 3
#define VS_INSTRUCTION_WRITE 2

#define VS_REG_MODE 0x0
#define VS_REG_STATUS 0x1
#define VS_REG_BASS 0x2
#define VS_REG_CLOCKF 0x3
#define VS_REG_DECODE_TIME 0x4
#define VS_REG_AUDATA 0x5
#define VS_REG_WRAM 0X6
#define VS_REG_WRAMADDR 0X7
#define VS_REG_HDAT0 0X8
#define VS_REG_HDAT1 0X9
#define VS_REG_AIADDR 0XA
#define VS_REG_VOL 0XB
#define VS_REG_AICTRL0 0XC
#define VS_REG_AICTRL1 0XD
#define VS_REG_AICTRL2 0XE
#define VS_REG_AICTRL3 0XF

// BIT POSITIONS IN VS_REG_MODE
#define VS_SM_DIFF 0
#define VS_SM_LAYER12 1
#define VS_SM_RESET 2
#define VS_SM_CANCEL 3
#define VS_SM_EARSPEAKER_LO 4
#define VS_SM_TESTS 5
#define VS_SM_STREAM 6
#define VS_SM_EARSPEAKER_HI 7
#define VS_SM_DACT 8
#define VS_SM_SDIORD 9
#define VS_SM_SDISHARE 10
#define VS_SM_SDINEW 11
#define VS_SM_ADPCM 12
#define VS_SM_LINE1 14
#define VS_SM_CLK_RANGE 15

// BIT POSITIONS IN VS_REG_STATUS
#define VS_SS_DO_NOT_JUMP 15
#define VS_SS_SWING 12
#define VS_SS_VCM_OVERLOAD 11
#define VS_SS_VCM_DISABLE 10
#define VS_SS_VER 4
#define VS_SS_APDOWN 2
#define VS_SS_CLOCK 1
#define VS_SS_REFERENCE_SEL 0

// BIT POSITIONS IN VS_REG_BASS
#define VS_ST_AMPLITUDE 12
#define VS_ST_FREQLIMIT 8
#define VS_SB_AMPLITUDE 4
#define VS_SB_FREQLIMIT 0

// BIT POSITIONS IN VS_REG_CLOCKF
#define VS_SC_MULT 13
#define VS_SC_ADD 11
#define VS_SC_FREQ 0

// values for the clock multiplier
#define VS_CLOCK_MULT_1_0 0
#define VS_CLOCK_MULT_2_0 1
#define VS_CLOCK_MULT_2_5 2
#define VS_CLOCK_MULT_3_0 3
#define VS_CLOCK_MULT_3_5 4
#define VS_CLOCK_MULT_4_0 5
#define VS_CLOCK_MULT_4_5 6
#define VS_CLOCK_MULT_5_0 7

class VS1053 {
 public:
  VS1053(uint8_t cs, uint8_t dcs, uint8_t reset, SPIClass &spi,
         HardwareSerial &serial)
      : _cs(cs), _dcs(dcs), _reset(reset), _spi(spi), _serial(serial), _percussion(&percussionBank), _melodic(&melodicBank) {
    pinMode(_cs, OUTPUT);
    digitalWrite(_cs, HIGH);
    pinMode(_dcs, OUTPUT);
    digitalWrite(_dcs, HIGH);
    pinMode(_reset, OUTPUT);
    digitalWrite(_reset, HIGH);
  }

  void enableMIDI() {
    digitalWrite(_reset, LOW);
    delay(10);
    digitalWrite(_reset, HIGH);
    delay(10);
    // now update the clock speed to 5X
    sendCommand(VS_REG_CLOCKF, (VS_CLOCK_MULT_5_0 << VS_SC_MULT));
    // let it settle
    delay(100);
    setVolume(0, 0);
    setVolume(1, 0);
    setVolume(2, 0);
    setVolume(3, 0);
    delay(10);
    setBank(0, _percussion.getBank()->id);
    setBank(1, _percussion.getBank()->id);
    setBank(2, _percussion.getBank()->id);
    setBank(3, _percussion.getBank()->id);
    delay(10);
    setVolume(0, 15);
    setVolume(1, 15);
    setVolume(2, 15);
    setVolume(3, 15);
  }


  void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    sendMIDIByte(MIDI_NOTE_ON | (channel & 0x0f));
    sendMIDIByte(note & 0x7f);
    sendMIDIByte(velocity & 0x7f);
  }

  void noteOn(MIDINote note) {
    sendMIDIByte(MIDI_NOTE_ON | (note.channel & 0x0f));
    sendMIDIByte(note.note & 0x7f);
    sendMIDIByte(note.velocity & 0x7f);
  }

  void noteOff(MIDINote note) {
    sendMIDIByte(MIDI_NOTE_OFF | (note.channel & 0x0f));
    sendMIDIByte(note.note & 0x7f);
    sendMIDIByte(note.velocity & 0x7f);
  }

  /**
   * Set the overall volume
   * @param vol - from 0 to 100
  */
  void setVolume(uint8_t vol) {
    // SCI_VOL register is actually attenuation in 0.5dB steps
    // MSB is left channel, LSB is right channel
    // silence is 0xFEFE
    // sending 0xFFFF activates analog powerdown
    // vol is thus 0 to 254
    uint8_t v = 0xfe - vol;
    sendCommand(VS_REG_VOL, v + (v << 8));
  }

  void setVolume(uint8_t channel, uint8_t vol) {
    _volume[channel] = vol;
    sendMIDIByte(MIDI_CHANNEL_MODE | (channel & 0x0f));
    sendMIDIByte(MIDI_CHANNEL_VOLUME);
    sendMIDIByte(vol & 0x7f);
  }

  uint8_t getVolume(uint8_t channel) {
    return _volume[channel];
  }

  void setInstrument(uint8_t channel, uint8_t instrument) {
    sendMIDIByte(MIDI_CHANNEL_PROGRAM | (channel & 0x0f));
    sendMIDIByte(instrument);
    _instruments[channel]= instrument;
    delay(10);
  }

  void setBank(uint8_t channel, uint8_t bank) {
    sendMIDIByte(MIDI_CHANNEL_MODE | (channel & 0x0f));
    sendMIDIByte(MIDI_MODE_SET_BANK);
    sendMIDIByte(bank & 0x7f);
    _bank[channel] = bank;
    delay(10);
  }

  void setTreble(int tone) {
    _bassReg = (_bassReg & ~0xf000) | ((tone - 8) << VS_ST_AMPLITUDE);
    sendCommand(VS_REG_BASS, _bassReg);
  }

  void setBass(int bass) {
    _bassReg = (_bassReg & ~0x00f0) | (bass << VS_SB_AMPLITUDE);
    sendCommand(VS_REG_BASS, _bassReg);
  }

  int getTreble() {
    // recover the original value by extending the sign
    int v = (_bassReg & 0xf000) >> VS_ST_AMPLITUDE;
    if (v > 7) v = -v;
    v = v + 8;
    return v;
  }
  int getBass() { return (_bassReg & 0x00f0) >> VS_SB_AMPLITUDE; }

  void loadUserCode(const int len, const unsigned char atab[],
                    const unsigned short dtab[]) {
    int i;
    for (i = 0; i < len; i++) {
      sendCommand(atab[i], dtab[i]);
    }
  }

  InstrumentBank &getMelodyBank() {
    return _melodic;
  }

  InstrumentBank &getPercussionBank() {
    return _percussion;
  }

  InstrumentBank &getChannelBank(uint8_t channel) {
    if(_bank[channel] == _melodic.getBank()->id) {
      return _melodic;
    }

    if(_bank[channel] == _percussion.getBank()->id) {
      return _percussion;
    }

    return _melodic;
  }

  Instrument getChannelInstrument(uint8_t channel) {
    return getChannelBank(channel).getInstrumentById(_instruments[channel]);
  }

  const Family getChannelFamily(uint8_t channel) {
    return getChannelBank(channel).getInstrumentFamily(_instruments[channel]);
  }

  const Voice getChannelVoice(uint8_t channel) {
    return getChannelBank(channel).getVoiceById(_instruments[channel]);
  }

 protected:
  uint8_t _cs, _dcs, _reset;
  char _name[30];
  uint8_t _bank[4];
  uint8_t _instruments[4];
  uint16_t _bassReg = (4 << VS_ST_FREQLIMIT) | 8;
  SPIClass &_spi;
  HardwareSerial &_serial;
  InstrumentBank _percussion, _melodic;
  uint8_t _volume[4];

  
  void sendCommand(uint8_t address, uint16_t data) {
    _spi.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    digitalWrite(_cs, LOW);
    delayMicroseconds(1);
    _spi.transfer(VS_INSTRUCTION_WRITE);
    _spi.transfer(address);
    _spi.transfer(data >> 8);
    _spi.transfer(data & 0xff);
    delayMicroseconds(1);
    digitalWrite(_cs, HIGH);
    delayMicroseconds(1);
    _spi.endTransaction();
  }

  void sendData(uint16_t data) {
    _spi.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    digitalWrite(_dcs, LOW);
    delayMicroseconds(1);
    _spi.transfer(data >> 8);
    _spi.transfer(data & 0xff);
    delayMicroseconds(1);
    digitalWrite(_dcs, HIGH);
    delayMicroseconds(1);
    _spi.endTransaction();
  }

  void send2Data(uint16_t data1, uint16_t data2) {
    _spi.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    digitalWrite(_dcs, LOW);
    delayMicroseconds(1);
    _spi.transfer(data1 >> 8);
    _spi.transfer(data1 & 0xff);
    _spi.transfer(data2 >> 8);
    _spi.transfer(data2 & 0xff);
    delayMicroseconds(1);
    digitalWrite(_dcs, HIGH);
    delayMicroseconds(1);
    _spi.endTransaction();
  }
  void send4Data(uint16_t data1, uint16_t data2, uint16_t data3,
                 uint16_t data4) {
    _spi.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
    digitalWrite(_dcs, LOW);
    delayMicroseconds(1);
    _spi.transfer(data1 >> 8);
    _spi.transfer(data1 & 0xff);
    _spi.transfer(data2 >> 8);
    _spi.transfer(data2 & 0xff);
    _spi.transfer(data3 >> 8);
    _spi.transfer(data3 & 0xff);
    _spi.transfer(data4 >> 8);
    _spi.transfer(data4 & 0xff);
    delayMicroseconds(1);
    digitalWrite(_dcs, HIGH);
    delayMicroseconds(1);
    _spi.endTransaction();
  }

  void sendMIDIByte(uint8_t b) { _serial.write(b); }
};

}  // namespace bassmate
#endif
