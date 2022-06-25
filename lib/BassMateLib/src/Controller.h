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

    /*
     * View callbacks
     */

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

    void channelVolumeChange(uint8_t channel, int volume)
    {
        _model._midi.setVolume(channel, (volume * 127) / 100);
    }

    void voiceChange(uint8_t channel, int voiceId)
    {
        Instrument i = _model._midi.getChannelBank(channel).getInstrumentById(voiceId);
        _model._sequencer.setInstrument(channel, i);
        MIDINote note = { channel, i.defaultNote, 127 };
        _model._midi.setInstrument(channel, voiceId);
        _model._midi.noteOn(note);
    }

    void voicePreviewChange(uint8_t channel, int voiceId)
    {
        Instrument currentVoice = _model._midi.getChannelInstrument(channel);
        Instrument i = _model._midi.getChannelBank(channel).getInstrumentById(voiceId);
        MIDINote note = { channel, i.defaultNote, 127 };
        _model._midi.setInstrument(channel, voiceId);
        _model._midi.noteOn(note);
        _model._midi.setInstrument(channel, currentVoice.voiceId);
    }

    void channelRefresh(uint8_t channel)
    {
        _view.setChannelInstruments(channel,
            _model._midi.getChannelFamily(channel),
            _model._midi.getChannelVoice(channel));
    }

    void familyChange(uint8_t channel, int familyId)
    {
        if (familyId != _model._midi.getChannelFamily(channel).id) {
            // assign the default instrument for the family
            // this also assigns the family as a side-effect
            Family family = _model._midi.getPercussionBank().getFamilyById(familyId);
            voiceChange(channel, family.voices[0].id);
        }
    }

    void familyRefresh(uint8_t channel)
    {
        _view.setChannelFamily(
            channel,
            _model._midi.getChannelBank(channel).getBank(),
            _model._midi.getChannelFamily(channel));
    }

    void previewPreset(uint8_t idx)
    {
        Storage::Preset p = _model._storage->get(idx);
        _view.setPresetPreview(p);
    }

    void loadPreset(uint8_t idx)
    {
        Storage::Preset p = _model._storage->get(idx);
    }

    void deletePreset(uint8_t idx) { }
    void savePreset(char* name, uint8_t nameLen, bool overwrite)
    {
        Storage::Preset p = PresetHandler::getActivePreset(&_model._midi, &_model._sequencer);
        p.setName(name, nameLen);
        if (_model._storage->exists(p) && !overwrite) {
            _view.notifyPresetExists(p);
        } else {
            if(_model._storage->save(p)) {
                _view.notifyPresetSaved(p);
            } else {
                _view.notifyPresetSaveFailed(p);
            }
        }
    }
    void clearPresets() { }
    void presetsRefresh() { }

protected:
    IView& _view;
    Model& _model;
    NoteGrid& _noteGrid;
};
}
#endif