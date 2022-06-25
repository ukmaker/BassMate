#ifndef BM_I_VIEW_H
#define BM_I_VIEW_H
#include <Arduino.h>
#include "VS1053/Voice.h"
#include "Storage.h"

namespace bassmate {

/*
 * Intteface defining a UI view
 * **/
class IView {
public:
    enum PlayPauseStop {
        PLAY,
        PAUSE,
        STOP
    };

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
        const Voice selectedInstrument)
        = 0;
    virtual void setChannelFamily(uint8_t channel, const Bank* bank,
        const Family selectedFamily)
        = 0;
    virtual void setPresetPreview(Storage::Preset p) = 0;
    virtual void notifyPresetExists(Storage::Preset p) =0;
    virtual void notifyPresetSaved(Storage::Preset p) = 0;
    virtual void notifyPresetSaveFailed(Storage::Preset p) = 0;
};
} // namespace bassmate
#endif