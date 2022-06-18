#ifndef BM_I_VIEW_H
#define BM_I_VIEW_H
#include <Arduino.h>
#include "VS1053/Voice.h"
#include "Core/FunctionPointer.h"

namespace bassmate
{

  /*
   * Intteface defining a UI view
   * **/
  class IView
  {
  public:
    enum PlayPauseStop
    {
      PLAY,
      PAUSE,
      STOP
    };

    typedef struct ChannelArg
    {
      uint8_t channel;
      uint8_t arg;
    } ChannelArg;

    typedef struct StringArg
    {
      char *str;
      uint8_t strLen;
    } StringArg;

    /*
     * Process any input events captured by the implementation's event mechanism
     */
    virtual void run();

    /********************************************************************
     * Methods to supply values to the UI
     ********************************************************************/
    virtual void setVolume(uint8_t volume) = 0;
    virtual void setTempo(uint8_t tempo) = 0;
    virtual void setPlayPauseStop(PlayPauseStop pps) = 0;
    virtual void setChannelVolume(uint8_t channel, uint8_t volume) = 0;
    virtual void setChannelInstruments(uint8_t channel, const Family instruments,
                                       const Voice selectedInstrument) = 0;
    virtual void setChannelFamily(uint8_t channel, const Bank *bank,
                                  const Family selectedFamily) = 0;

    /********************************************************************
     * The following callbacks are to communicate a change in the value
     * of a component
     ********************************************************************/

    template <class T>
    void onVolumeChange(T *tptr, void (T::*mptr)(uint8_t volume))
    {
      _volumeChange.attach(tptr, mptr);
    }

    template <class T>
    void onChannelVolumeChange(T *tptr, void (T::*mptr)(ChannelArg arg))
    {
      _channelVolumeChange.attach(tptr, mptr);
    }

    template <class T>
    void onTempoChange(T *tptr, void (T::*mptr)(uint8_t tempo))
    {
      _tempoChange.attach(tptr, mptr);
    }

    template <class T>
    void onPlayPauseStop(T *tptr, void (T::*mptr)(PlayPauseStop pps))
    {
      _playPauseStopChange.attach(tptr, mptr);
    }

    template <class T>
    void onVoiceChange(T *tptr,
                       void (T::*mptr)(ChannelArg arg))
    {
      _voiceChange.attach(tptr, mptr);
    }

    template <class T>
    void onVoicePreview(T *tptr,
                        void (T::*mptr)(ChannelArg arg))
    {
      _voicePreviewChange.attach(tptr, mptr);
    }

    template <class T>
    void onFamilyChange(T *tptr,
                        void (T::*mptr)(ChannelArg arg))
    {
      _familyChange.attach(tptr, mptr);
    }

    template <class T>
    void onLoadPreset(T *tptr, void (T::*mptr)(uint8_t presetId))
    {
      _loadPreset.attach(tptr, mptr);
    }

    template <class T>
    void onDeletePreset(T *tptr, void (T::*mptr)(uint8_t presetId))
    {
      _deletePreset.attach(tptr, mptr);
    }

    template <class T>
    void onSavePreset(T *tptr, void (T::*mptr)(StringArg arg))
    {
      _savePreset.attach(tptr, mptr);
    }

    template <class T>
    void onClearPresets(T *tptr, void (T::*mptr)())
    {
      _clearPresets.attach(tptr, mptr);
    }

    /********************************************************************
     * This group of callbacks is to ask for a component's state to be
     * refreshed e.g. because it has focus
     ********************************************************************/

    /*
     * @brief Called when the channel dialog is show to ask for the correct
     * set of instrument choices
     */
    template <class T>
    void onChannelRefresh(T *tptr, void (T::*mptr)(uint8_t channel))
    {
      _channelRefresh.attach(tptr, mptr);
    }

    /*
     * @brief Called when the load dialog is displayed to refresh the list
     * of saved presets
     */
    template <class T>
    void onLoadRefresh(T *tptr, void (T::*mptr)())
    {
      _loadRefresh.attach(tptr, mptr);
    }

  protected:
    FunctionPointerArg1<void, uint8_t> _volumeChange;
    FunctionPointerArg1<void, uint8_t> _tempoChange;
    FunctionPointerArg1<void, PlayPauseStop> _playPauseStopChange;
    FunctionPointerArg1<void, ChannelArg> _channelVolumeChange;
    FunctionPointerArg1<void, ChannelArg> _voiceChange;
    FunctionPointerArg1<void, ChannelArg> _voicePreviewChange;
    FunctionPointerArg1<void, ChannelArg> _familyChange;

    FunctionPointerArg1<void, uint8_t> _loadPreset;
    FunctionPointerArg1<void, uint8_t> _deletePreset;
    FunctionPointerArg1<void, StringArg> _savePreset;
    FunctionPointerArg1<void, void> _clearPresets;

    FunctionPointerArg1<void, uint8_t> _channelRefresh;
    FunctionPointerArg1<void, void> _loadRefresh;
  };
} // namespace bassmate
#endif