#include "MIDI.h"
#include "VS1053/Instrument.h"

using namespace bassmate;

#ifndef BM_SEQUENCER_H
#define BM_SEQUENCER_H

typedef void (*SequenceCallback)(uint8_t beat, MIDINote note);
typedef void (*BeatCallback)(uint8_t beat);

/**
 * Class to encapsulate managing the note sequences
 */
class Sequencer {

public:
    Sequencer() { }

    MIDINote getDefaultNote(uint8_t channel)
    {
        MIDINote note = { channel, _instruments[channel].defaultNote, 127 };
        return note;
    }

    void attachNoteOnCallback(SequenceCallback cb)
    {
        _noteOnCallback = cb;
    }

    void attachBeatCallback(BeatCallback cb)
    {
        _beatCallback = cb;
    }

    void attachNoteOffCallback(SequenceCallback cb)
    {
        _noteOffCallback = cb;
    }

    void tick()
    {
        if (_running) {
            long t = millis();

            if ((t - _ticks) > _millisPerBeat) {

                _beat++;
                if (_beat > 7)
                    _beat = 0;

                if (_beatCallback != NULL) {
                    _beatCallback(_beat);
                }

                for (int y = 0; y < 4; y++) {
                    if (_lastNotes[y].velocity && (_noteOffCallback != NULL)) {
                        _noteOffCallback(_beat, _lastNotes[y]);
                    }

                    if (_active[_beat][y] && (_noteOnCallback != NULL)) {
                        _noteOnCallback(_beat, _notes[_beat][y]);
                    }

                    _lastNotes[y] = _notes[_beat][y];
                }
                _ticks = t;
            }
        }
    }

    void setRunning(bool r)
    {
        _running = r;
    }

    bool isRunning()
    {
        return _running;
    }

    void reset()
    {
        _running = false;
        _ticks = 0;
        _beat = -1;
    }

    void setInstrument(uint8_t channel, Instrument instrument)
    {
        _instruments[channel] = instrument;
        for (int x = 0; x < 8; x++) {
            _notes[x][channel] = getDefaultNote(channel);
        }
        if (_noteOnCallback != NULL) {
            _noteOnCallback(0, getDefaultNote(channel));
        }
    }

    Instrument getInstrument(uint8_t channel)
    {
        return _instruments[channel];
    }

    int getBeat()
    {
        return _beat;
    }

    void setTempo(int tempo)
    {
        // tempo range is 20 - 200 BPM
        // millis per beat (bpm / 60 * 1000
        _millisPerBeat = 60000 / tempo / 4;
    }

    bool toggle(uint8_t x, uint8_t y)
    {
        // to start with we toggle the state
        // if there's no assigned instrument already, use the current voice
        _active[x][y] = !_active[x][y];

        return _active[x][y];
    }

    void setActive(uint8_t x, uint8_t y)
    {
        _active[x][y] = true;
    }

    void clearActive(uint8_t x, uint8_t y)
    {
        _active[x][y] = false;
    }

    bool isActive(uint8_t x, uint8_t y)
    {
        return _active[x][y];
    }

    void setNoteVolume(uint8_t x, uint8_t y, uint8_t volume)
    {
        _notes[x][y].velocity = volume;
    }

    MIDINote getNote(uint8_t x, uint8_t y)
    {
        return _notes[x][y];
    }

protected:
    bool _running = false;
    int _ticks = 0;
    int _millisPerBeat = 0;
    int _beat = -1;

    MIDINote _notes[8][4];
    bool _active[8][4];
    MIDINote _lastNotes[4];
    // which instrument to use on each channel
    Instrument _instruments[4];

    SequenceCallback _noteOnCallback, _noteOffCallback;
    BeatCallback _beatCallback;
};
#endif
