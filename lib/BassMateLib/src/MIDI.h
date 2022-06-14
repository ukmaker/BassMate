#ifndef BASSMATE_MIDI_H
#define BASSMATE_MIDI_H

#include "Arduino.h"

#define MIDI_NOTE_OFF 0x80
#define MIDI_NOTE_ON 0x90

#define MIDI_MASTER_VOLUME 0x01

#define MIDI_CHANNEL_MODE 0xb0
#define MIDI_CHANNEL_VOLUME 0x07
#define MIDI_CHANNEL_EXPRESSION 0x0b
#define MIDI_CHANNEL_PROGRAM 0xc0

#define MIDI_MODE_SET_BANK 0
#define MIDI_BANK_MELODIC 0
#define MIDI_BANK_PERCUSSION 0x78


typedef struct MIDINote {
  
  uint8_t channel;
  uint8_t note;
  uint8_t velocity;
  
} MIDINote;

#endif