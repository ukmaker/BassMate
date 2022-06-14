#ifndef BM_INSTRUMENTS_H
#define BM_INSTRUMENTS_H

#include "Voice.h"

namespace bassmate {

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

}  // namespace bassmate
#endif