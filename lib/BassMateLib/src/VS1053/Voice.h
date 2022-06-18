#ifndef BM_VOICE_H
#define BM_VOICE_H

#include <stdint.h>

namespace bassmate {

typedef struct Voice {
  const char *name;
  const uint8_t id;
} Voice;


/*
 * Handy struct to make it easy to pass around the essential
 * data about an instrument selection
 */
typedef struct Instrument {
   uint8_t voiceId;
   uint8_t bankId;
   uint8_t familyId;
   bool melodic;
   uint8_t defaultNote;
} Instrument;

typedef struct Family {
  const char *name;
  const uint8_t id;
  const uint8_t numVoices;
  const Voice *voices;
} Family;

typedef struct Bank {
  const char *name;
  const uint8_t id;
  const uint8_t numFamilies;
  const Family *families;
  const bool melodic;
  const uint8_t defaultNote;
} Bank;

}  // namespace bassmate

#endif