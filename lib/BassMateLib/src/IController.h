#ifndef BASSMATE_ICONTROLLER_H
#define BASSMATE_ICONTROLLER_H

#include "FunctionArgs.h"

namespace bassmate {
/**
 * @brief Interface defining the methods use by a View to call back to
 * a controller when a change occurs
 *
 */
class IController {
public:
    virtual void begin() = 0;

    virtual void volumeChange(uint8_t volume) = 0;
    virtual void tempoChange(int tempo) = 0;
    virtual void playPauseStopChange(uint8_t pps) = 0;
    virtual void channelVolumeChange(uint8_t channel, int volume) = 0;

    virtual void voiceChange(uint8_t channel, int voiceId) = 0;
    virtual void voicePreviewChange(uint8_t channel, int voiceId) = 0;
    virtual void familyChange(uint8_t channel, int familyId) = 0;

    virtual void previewPreset(uint8_t idx) = 0;
    virtual void loadPreset(uint8_t idx) = 0;
    virtual void deletePreset(uint8_t idx) = 0;
    virtual void savePreset(char* name, uint8_t nameLen, bool overwrite) = 0;
    virtual void clearPresets() = 0;

    virtual void channelRefresh(uint8_t channel) = 0;
    virtual void familyRefresh(uint8_t channel) = 0;

};
}
#endif