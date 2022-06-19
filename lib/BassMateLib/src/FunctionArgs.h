#ifndef BASSMATE_FN_ARGS_H
#define BASSMATE_FN_ARGS_H
#include <Arduino.h>

namespace bassmate {
    
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
}
#endif