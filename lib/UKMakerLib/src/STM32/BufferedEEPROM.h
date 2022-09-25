#ifndef UKMAKER_STM_BUFFERED_EEPROM_H
#define UKMAKER_STM_BUFFERED_EEPROM_H

#include <EEPROM.h>
#include <stdint.h>
#include <string.h>

namespace ukmaker {
class BufferedEEPROM {
public:
    static void save() { eeprom_buffer_flush(); }

    static void load() { eeprom_buffer_fill(); }

    // Functionality to 'get' and 'put' objects to and from EEPROM.
    template <typename T>
    static T& get(int idx, T& t)
    {
        uint8_t* ptr = (uint8_t*)&t;
        for (int count = sizeof(T); count; --count) {
            *ptr++ = eeprom_buffered_read_byte(idx++);
        }
        return t;
    }

    template <typename T>
    static const T& put(int idx, const T& t)
    {
        const uint8_t* ptr = (const uint8_t*)&t;
        for (int count = sizeof(T); count; --count) {
            eeprom_buffered_write_byte(idx++, *ptr++);
        }
        return t;
    }
};
}
#endif