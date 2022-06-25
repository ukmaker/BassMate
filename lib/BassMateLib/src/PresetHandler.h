#ifndef BM_PRESET_HANDLER_H
#define BM_PRESET_HANDLER_H

#include "Sequencer.h"
#include "Storage.h"
#include "VS1053/VS1053.h"
#include "NoteGrid.h"

namespace bassmate {

class PresetHandler {
public:
    PresetHandler()
    {
    }

    // Return a Preset describing the current configuration
    static Storage::Preset getActivePreset(VS1053* midi, Sequencer* sequencer)
    {

        Storage::Preset p;

        for (uint8_t y = 0; y < 4; y++) {
            p.lines[y].bankId = midi->getChannelBank(y).getBank()->id;
            p.lines[y].familyId = midi->getChannelFamily(y).id;
            p.lines[y].voiceId = midi->getChannelVoice(y).id;
            p.lines[y].volume = midi->getVolume(y);
            for (uint8_t x = 0; x < 8; x++) {
                p.lines[y].active[x] = sequencer->isActive(x, y);
            }
        }
        return p;
    }

    static void setActivePreset(VS1053* midi, Sequencer* sequencer, NoteGrid *noteGrid, Storage::Preset p)
    {

        for (uint8_t y = 0; y < 4; y++) {
            midi->setBank(y, p.lines[y].bankId);
            midi->setInstrument(y, p.lines[y].voiceId);
            midi->setVolume(y, p.lines[y].volume);

            for (uint8_t x = 0; x < 8; x++) {
                if(p.lines[y].active[x]) {
                    sequencer->setActive(x,y);
                    noteGrid->setActive(x,y);
                } else {
                    sequencer->clearActive(x,y);
                    noteGrid->setInactive(x,y);
                }
            }
        }
    }
};
}
#endif