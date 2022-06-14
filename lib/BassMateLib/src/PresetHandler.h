#ifndef BM_PRESET_HANDLER_H
#define BM_PRESET_HANDLER_H

#include "VS1053/VS1053.h"
#include "Storage.h"
#include "Sequencer.h"

namespace bassmate {

class PresetHandler {
    public:

    PresetHandler() {
        
    }

    // Return a Preset describing the current configuration
    static Storage::Preset getActivePreset(VS1053 *midi, Sequencer *sequencer) {
        
        Storage::Preset p;

        for(uint8_t y=0; y<4; y++) {
            p.lines[y].bankId = midi->getChannelBank(y).getBank()->id;
            p.lines[y].familyId = midi->getChannelFamily(y).id;
            p.lines[y].voiceId = midi->getChannelVoice(y).id;
            p.lines[y].volume = midi->getVolume(y);
            for(uint8_t x=0; x<8; x++) {
                p.lines[y].notes[x] = sequencer->getNote(x,y);
            }
        }
        return p;
    }
};
}
#endif