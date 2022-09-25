#ifndef BM_STORAGE_H
#define BM_STORAGE_H

#include <stdint.h>
#include <string.h>

#include "STM32/BufferedEEPROM.h"

namespace bassmate {

using namespace ukmaker;

/**
 * This class encapsulates the functionality needed to save and load
 * rhythm presets
 */
class Storage {
public:
    typedef struct Header {
        char magic[9]; // So we know the EEPROM has been initialised
        uint8_t records;
    };

    typedef struct Line {
        uint8_t voiceId;
        uint8_t familyId;
        uint8_t bankId;
        uint8_t volume;
        bool active[8];
    };

    typedef struct Preset {
        char name[16];
        Line lines[4];
        void setName(const char* n, const uint8_t len)
        {
            strncpy(name, n, len > 15 ? 15 : len);
            name[len > 15 ? 15 : len] = '\0';
        }
    };

    Storage()
    {
        _header.records = 0;
        _open = false;
    }

    ~Storage() { }

    void format()
    {
        BufferedEEPROM::load();
        // read the "EEPROM" to see what's there
        BufferedEEPROM::get(0, _header);
        strncpy(_header.magic, "BassMate", 8);
        _header.magic[8] = '\0';
        _header.records = 0;

        BufferedEEPROM::put(0, _header);
        BufferedEEPROM::save();
        _open = true;
    }

    uint8_t getNumPresets()
    {
        open();
        return _header.records;
    }

    uint8_t open()
    {
        if (!_open) {
            BufferedEEPROM::load();
            // read the "EEPROM" to see what's there
            BufferedEEPROM::get(0, _header);
            if (strcmp(_header.magic, "BassMate") != 0) {
                // Initialise it!
                format();
            }
            _open = true;
        }
        return _header.records;
    }

    Preset get(uint8_t idx)
    {
        Preset r;
        open();
        if (idx < _header.records) {
            int addr = sizeof(Header) + idx * sizeof(Preset);
            BufferedEEPROM::get(addr, r);
        } else {
            for (uint8_t l = 0; l < 4; l++) {
                r.lines[l].bankId = 0;
                r.lines[l].familyId = 0;
                strcpy(r.name, "----\0");
                r.lines[l].voiceId = 0;
            }
        }

        return r;
    }

    void remove(uint8_t idx)
    {
        open();
        if (_header.records > 0 && idx < _header.records) {
            if (idx == (_header.records - 1)) {
                // Remove the last one - just decrement the total
                _header.records--;
                BufferedEEPROM::put(0, _header);
            } else {
                // shuffle everything down
                for (uint8_t i = idx + 1; i < _header.records; i++) {
                    Preset r = get(idx);
                    _put(idx - 1, r);
                }
                _header.records--;
                BufferedEEPROM::put(0, _header);
            }
            BufferedEEPROM::save();
        }
    }

    bool put(uint8_t idx, Preset& r)
    {
        open();

        if (_header.records == 0) {
            _header.records = 1;
            idx = 0;
        } else if (idx >= _header.records) {
            idx = _header.records;
            _header.records++;
        }

        int addr = sizeof(Header) + idx * sizeof(Preset);
        BufferedEEPROM::put(addr, r);
        BufferedEEPROM::put(0, _header);
        BufferedEEPROM::save();

        return true;
    }

    /**
     * Returns true if a preset with the same name already exists
     */
    bool exists(Preset r)
    {
        for (uint8_t i = 0; i < _header.records; i++) {
            Preset p = get(i);
            if (strcmp(r.name, p.name) == 0) {
                return true;
            }
        }
        return false;
    }

    /**
     * Save the preset, overwriting an existing one with the same name
     * Returns false if the save fails
     */
    bool save(Preset p)
    {
        for (uint8_t i = 0; i < _header.records; i++) {
            Preset q = get(i);
            if (strcmp(q.name, p.name) == 0) {
                // overwrite
                return put(i, p);
            }
        }
        // append
        return put(_header.records, p);
    }

protected:
    Header _header;
    bool _open = false;

    bool _put(uint8_t idx, Preset& r)
    {
        if (_header.records == 0) {
            _header.records = 1;
            idx = 0;
        } else if (idx >= _header.records) {
            idx = _header.records;
            _header.records++;
        }

        int addr = sizeof(Header) + idx * sizeof(Preset);
        BufferedEEPROM::put(addr, r);
        BufferedEEPROM::put(0, _header);
        return true;
    }
};
} // namespace bassmate

#endif