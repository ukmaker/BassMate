#ifndef BM_PRESENTER_H
#define BM_PRESENTER_H

#include <EEPROM.h>

#include "Model.h"
#include "NoteGrid.h"
#include "PresetHandler.h"
#include "View.h"

namespace bassmate
{

  /*
   * The presenter interfaces the hardware to the View
   * Keeps track of the UI state
   **/
  class Presenter
  {
  public:
    /*
     * Keep track of which UI element has focus
     */
    enum State
    {
      HOME,

      CONTEXT_PRESETS,
      // Save/Load tabs
      PRESETS_LOAD,
      PRESETS_SAVE,
      // And the actual UIs in the tabs
      PRESETS_LOAD_UI,
      PRESETS_SAVE_UI,
      SAVING,
      VIEW_PRESET,
      LOADING,

      CONTEXT_SEQUENCER,
      // Volume and tempo bar
      SEQUENCER_VOLUME,
      SEQUENCER_TEMPO,
      // Channel tabs
      SEQUENCER_CHANNEL_VOLUME,
      SEQUENCER_CHANNEL_INSTRUMENT,
      SEQUENCER_CHANNEL_INSTRUMENT_GROUP
    };

    Presenter(Model &model, View &view, NoteGrid &noteGrid)
        : _model(model), _view(view), _noteGrid(noteGrid)
    {
      _state = CONTEXT_SEQUENCER;
    }

    ~Presenter() {}

    void begin()
    {
      for (uint8_t i = 0; i < 4; i++)
      {
        _setInstrument(i, _model._midi.getChannelVoice(i));
        _view.channelVols->setPercent(i, 100);
        _model._midi.setVolume(i, 127);
      }

      _view.tempoWidget->setValue(90);
      _updateTempo();

      _view.volume->setPercent(80);
      _model._midi.setVolume(225 + (_view.volume->getValue() * 29) / 100);
      _model._midi.setBass(15);
      _view.keyboard->onCancel(this, &Presenter::handleKeyboardCancel);
      _view.keyboard->onOK(this, &Presenter::handleKeyboardOK);
      _view.storageSelect->onChange(this, &Presenter::handleStorageChange);
      _state = SEQUENCER_VOLUME;
      updateFocus();
    }

    void tick()
    {
      if (_selectStarted && (millis() - now) > 500)
      {
        _selectStarted = false;
        _state = HOME;
        updateFocus();
      }

      if (_state == SAVING)
      {
        _savingTicks--;
        if (_savingTicks == 0)
        {
          _state = SEQUENCER_VOLUME;
          _view.hideSaving();
          updateFocus();
        }
      }
    }

    void handleSelectStart()
    {
      if (_state != SAVING)
      {
        _selectStarted = true;
        now = millis();
      }
    }

    /*
     * Sequencer events
     */

    void handleBeat(uint8_t beat) { _noteGrid.beat(beat); }

    void handleNoteOn(uint8_t beat, MIDINote note) { _model._midi.noteOn(note); }

    void handleNoteOff(uint8_t beat, MIDINote note)
    {
      _model._midi.noteOff(note);
    }

    /*
     * NoteGrid events
     */
    void handleNoteGridEvent(keyEvent e) { _noteGrid.handleEvent(e); }

    void handleNoteGridKeyPress(uint8_t x, uint8_t y)
    {
      if (_model._sequencer.toggle(x, y))
      {
        _noteGrid.setActive(x, y);
      }
      else
      {
        _noteGrid.setInactive(x, y);
      }
    }

    /**
     * View widget callbacks
     * */
    void handleKeyboardCancel()
    {
      _state = PRESETS_SAVE;
      _view.keyboard->blur();
      updateFocus();
    }
    void handleKeyboardOK()
    {
      Storage::Preset p =
          PresetHandler::getActivePreset(&_model._midi, &_model._sequencer);
      p.setName(_view.keyboard->getText(), strlen(_view.keyboard->getText()));
      if (_model._storage->exists(p))
      {
        _view.savingDialog->setBackground(RED);
        _view.savingDialog->setText("A preset with this name already exists");
      }
      else
      {
        _view.savingDialog->setBackground(BLACK);
        _view.savingDialog->setText("Saving...");
        _model._storage->save(p);
      }
      _state = SAVING;
      _savingTicks = 400000;
      _view.showSaving();
    }

    /**
     * Called when a preset is selected
     * Triggers a load/delete/cancel dialog
     */
    void handleStorageChange(Widget *w)
    {
      _state = VIEW_PRESET;
      Storage::Preset p = _view.storageSelect->getSelectedValue();
     //_view.selectionText->setText(p.name);
    }

    uint8_t getChannel() {
      return _view.channelVols->getSelected();
    }

    void updateFocus()
    {
      switch (_state)
      {
      case CONTEXT_SEQUENCER:
        _view.selectContextSequencer();
        break;

      case HOME:
      case SEQUENCER_VOLUME:
        _view.selectTopVolume();
        _view.channelVols->blur();
        break;

      case SEQUENCER_TEMPO:
        _view.selectTopTempo();
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _view.selectChannel(getChannel());
        _view.channelVols->focus();
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
        _view.selectChannelInstrument(getChannel(),
                                      _model._midi.getChannelFamily(getChannel()),
                                      _model._midi.getChannelVoice(getChannel()));
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.selectChannelInstrumentGroup(
            getChannel(),
            _model._midi.getChannelBank(getChannel()).getBank(),
            _model._midi.getChannelFamily(getChannel()));
        break;

      case CONTEXT_PRESETS:
        _view.selectContextPresets();
        break;

      case PRESETS_SAVE:
        _view.selectPresetsSaveTab();
        break;

      case PRESETS_LOAD:
        _view.selectPresetsLoadTab();
        break;

      case PRESETS_SAVE_UI:
        _view.selectPresetsSaveUI();
        break;

      case PRESETS_LOAD_UI:
        _view.selectPresetsLoadUI();
        break;

      case SAVING:
        _view.showSaving();
        break;

      default:
        break;
      }
    }

    /*
     * navigation events
     */
    void handleLeftPress()
    {
      switch (_state)
      {
      case CONTEXT_PRESETS:
        _state = CONTEXT_SEQUENCER;
        break;

      case PRESETS_SAVE:
        _state = PRESETS_LOAD;
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->left();
        break;

      case SEQUENCER_TEMPO:
        _state = SEQUENCER_VOLUME;
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _assignFamily();
        _state = SEQUENCER_CHANNEL_INSTRUMENT;
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
      {
        // set the currently selected instrument for this channel
        _assignInstrument();
        _state = SEQUENCER_CHANNEL_VOLUME;
        break;
      }

      case SEQUENCER_CHANNEL_VOLUME:
        // go back to the top
        _state = SEQUENCER_VOLUME;
        break;

      default:
        break;
      }

      updateFocus();
    }

    void handleRightPress()
    {
      switch (_state)
      {
      case CONTEXT_SEQUENCER:
        _state = CONTEXT_PRESETS;
        break;

      case SEQUENCER_VOLUME:
        _state = SEQUENCER_TEMPO;
        break;

      case SEQUENCER_TEMPO:
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
        _state = SEQUENCER_CHANNEL_INSTRUMENT_GROUP;
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _state = SEQUENCER_CHANNEL_INSTRUMENT;
        break;

      case PRESETS_LOAD:
        _state = PRESETS_SAVE;
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->right();
        break;

      default:
        break;
      }
      updateFocus();
    }

    void handleUpPress()
    {
      switch (_state)
      {
      case SEQUENCER_VOLUME:
      case SEQUENCER_TEMPO:
        _state = CONTEXT_SEQUENCER;
        break;

      case SEQUENCER_CHANNEL_VOLUME:
      {

        if (getChannel() == 0)
        {
          _state = SEQUENCER_VOLUME;
        }
        else
        {
          _view.channelVols->up();
        }
        break;
      }

      case SEQUENCER_CHANNEL_INSTRUMENT:
      {
        _view.voiceSelect->scrollToTop();
        _previewInstrument();
        break;
      }

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.familySelect->scrollToTop();
        break;

      case PRESETS_LOAD:
      case PRESETS_SAVE:
        _state = CONTEXT_PRESETS;
        break;

      case PRESETS_LOAD_UI:
        _state = PRESETS_LOAD;
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->up();
        break;

      default:
        break;
      }
      updateFocus();
    }

    void handleDownPress()
    {
      switch (_state)
      {
      case CONTEXT_SEQUENCER:
        _state = SEQUENCER_VOLUME;
        break;

      case CONTEXT_PRESETS:
        _state = PRESETS_LOAD;
        break;

      case PRESETS_SAVE:
        _state = PRESETS_SAVE_UI;
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->down();
        break;

      case PRESETS_LOAD:
        _state = PRESETS_LOAD_UI;
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _view.channelVols->down();
        break;

      case SEQUENCER_VOLUME:
        _state = SEQUENCER_CHANNEL_VOLUME;
        break;

      case SEQUENCER_TEMPO:
        _state = SEQUENCER_CHANNEL_VOLUME;
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
      {
        _view.voiceSelect->scrollToBottom();
        _previewInstrument();
        break;
      }

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.familySelect->scrollToBottom();
        break;

      default:
        break;
      }
      updateFocus();
    }

    /*
     * handle a click on the NavKey's central button
     */
    void handleSelect()
    {
      _selectStarted = false;
      switch (_state)
      {
      case HOME:
        _state = SEQUENCER_VOLUME;
        break;

      case SEQUENCER_TEMPO:
      {
        bool running = !_model._sequencer.isRunning();
        _model._sequencer.setRunning(running);
        if (!running)
        {
          _view.noteGrid.showPausedBeat(_model._sequencer.getBeat());
          _view.pps->setState(PlayPauseStopWidget::PAUSE);
        }
        else
        {
          _view.pps->setState(PlayPauseStopWidget::PLAY);
        }
        break;
      }

      case SEQUENCER_VOLUME:
        _view.volume->toggleMute();
        _model._midi.setVolume((_view.volume->getValue() * 254) / 100);
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _playInstrument(getChannel());
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
      {
        // set the currently selected instrument for this channel
        _view.voiceSelect->selectCurrent();
        _assignInstrument();
        break;
      }

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.familySelect->selectCurrent();
        _assignFamily();
        updateFocus();
        break;

      case PRESETS_LOAD:
        _view.storageSelect->selectCurrent();
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->select();
        break;

      default:
        break;
      }
    }

    void handleIncrement()
    {
      switch (_state)
      {
      case SEQUENCER_VOLUME:
        _view.volume->increment();
        _model._midi.setVolume(225 + (_view.volume->getValue() * 29) / 100);
        break;

      case SEQUENCER_TEMPO:
        _view.tempoWidget->increment();
        _updateTempo();
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _view.channelVols->increment();
        _model._midi.setVolume(
            getChannel(),
            (_view.channelVols->getSelectedPercent() * 127) / 100);
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
        _view.voiceSelect->scrollUp();
        _previewInstrument();
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.familySelect->scrollUp();
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->forward();
        break;

      case PRESETS_LOAD_UI:
        _view.storageSelect->scrollUp();
        break;

      default:
        break;
      }
    }

    void handleDecrement()
    {
      switch (_state)
      {
      case SEQUENCER_VOLUME:
        _view.volume->decrement();
        _model._midi.setVolume(225 + (_view.volume->getValue() * 29) / 100);
        break;

      case SEQUENCER_TEMPO:
        _view.tempoWidget->decrement();
        _updateTempo();
        break;

      case SEQUENCER_CHANNEL_VOLUME:
        _view.channelVols->decrement();
        _model._midi.setVolume(
            getChannel(), (_view.channelVols->getSelectedPercent() * 127) / 100);
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT:
        _view.voiceSelect->scrollDown();
        _previewInstrument();
        break;

      case SEQUENCER_CHANNEL_INSTRUMENT_GROUP:
        _view.familySelect->scrollDown();
        break;

      case PRESETS_SAVE_UI:
        _view.keyboard->back();
        break;

      case PRESETS_LOAD_UI:
        _view.storageSelect->scrollDown();
        break;

      default:
        break;
      }
    }

  protected:
    State _state;
    uint32_t now;
    bool _selectStarted, _skipSelect = false;
    uint32_t _savingTicks = 0;

    Model &_model;
    View &_view;
    NoteGrid &_noteGrid;

    void _playInstrument(uint8_t channel)
    {
      Instrument i = _model._midi.getChannelInstrument(channel);
      MIDINote note = {channel, i.defaultNote, 127};
      _model._midi.noteOn(note);
    }

    void _previewInstrument()
    {
      _setInstrument(getChannel(), _view.voiceSelect->getViewValue());
    }

    void _assignInstrument()
    {
      _setInstrument(getChannel(), _view.voiceSelect->getSelectedValue());
    }

    void _assignFamily()
    {
      const Family family = _view.familySelect->getSelectedValue();
      if (family.id != _model._midi.getChannelFamily(getChannel()).id)
      {
        // assign the default instrument for the family
        // this also assigns the family as a side-effect
        _setInstrument(getChannel(), family.voices[0]);
      }
    }

    void _setInstrument(uint8_t channel, Voice voice)
    {
      Instrument i =
          _model._midi.getChannelBank(channel).getInstrumentById(voice.id);
      _model._sequencer.setInstrument(channel, i);
      MIDINote note = {channel, i.defaultNote, 127};
      _model._midi.setInstrument(channel, voice.id);
      _model._midi.noteOn(note);
      _view.channelVols->setChannelLabel(channel, voice.name);
    }

    void _updateTempo()
    {
      int tempo = _view.tempoWidget->getValue();
      _model._sequencer.setTempo(tempo);
      // at high tempos the fade out time on the note grid has to decrease
      if (tempo > 200)
      {
        _view.noteGrid.setBeatFadeTimePercent(25);
      }
      else if (tempo > 100)
      {
        _view.noteGrid.setBeatFadeTimePercent(50);
      }
      else
      {
        _view.noteGrid.setBeatFadeTimePercent(100);
      }
    }
  };

} // namespace bassmate
#endif