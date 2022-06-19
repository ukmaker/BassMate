#ifndef BASSMATE_CONTROLLER_H
#define BASSMATE_CONTROLLER_H

#include "IController.h"
#include "IView.h"
#include "Model.h"
#include "NoteGrid.h"

namespace bassmate {
/**
 * @brief Interface defining the methods use by a View to call back to
 * a controller when a change occurs
 *
 */
class Controller : public IController {
public:
    Controller(Model& model, IView& view, NoteGrid& noteGrid)
        : _model(model)
        , _view(view)
        , _noteGrid(noteGrid)
    {
        tempoChange(80);
    }

    ~Controller() { }

    /*
     * Sequencer events
     */

    void handleBeat(uint8_t beat)
    {
        _noteGrid.beat(beat);
    }

    void handleNoteOn(uint8_t beat, MIDINote note)
    {
        _model._midi.noteOn(note);
    }

    void handleNoteOff(uint8_t beat, MIDINote note)
    {
        _model._midi.noteOff(note);
    }

    /*
     * NoteGrid events
     */
    void handleNoteGridEvent(keyEvent e)
    {
        _noteGrid.handleEvent(e);
    }

    void handleNoteGridKeyPress(uint8_t x, uint8_t y)
    {
        if (_model._sequencer.toggle(x, y)) {
            _noteGrid.setActive(x, y);
        } else {
            _noteGrid.setInactive(x, y);
        }
    }

    void volumeChange(uint8_t volume)
    {
        _model._midi.setVolume(225 + (volume * 29) / 100);
    }

    void tempoChange(int tempo)
    {
        _model._sequencer.setTempo(tempo);
        // at high tempos the fade out time on the note grid has to decrease
        if (tempo > 200) {
            _noteGrid.setBeatFadeTimePercent(25);
        } else if (tempo > 100) {
            _noteGrid.setBeatFadeTimePercent(50);
        } else {
            _noteGrid.setBeatFadeTimePercent(100);
        }
    }

    void playPauseStopChange(uint8_t pps)
    {
        bool running = (pps == 2);
        bool stopped = (pps == 0);
        if (stopped) {
            _model._sequencer.reset();
        } else {
            _model._sequencer.setRunning(running);
        }

        if (!running) {
            _noteGrid.showPausedBeat(_model._sequencer.getBeat());
        }
    }

    void channelVolumeChange(uint8_t channel, int volume) { }

    void voiceChange(uint8_t channel, int voiceId) { }
    void voicePreviewChange(uint8_t channel, int voiceId) { }
    void familyChange(uint8_t channel, int familyId) { }

    void loadPreset(uint8_t idx) { }
    void deletePreset(uint8_t idx) { }
    void savePreset(char* name, uint8_t nameLen) { }
    void clearPresets() { }

    void channelRefresh(uint8_t channel) { }
    void loadRefresh() { }

protected:
    IView& _view;
    Model& _model;
    NoteGrid& _noteGrid;
};
}
#endif