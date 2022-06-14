#ifndef BM_VOICE_H
#define BM_VOICE_H

#include <stdint.h>

namespace bassmate {

typedef struct Voice {
  const char *name;
  const uint8_t id;
} Voice;

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