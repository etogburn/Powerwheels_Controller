#ifndef REMOTE_CONTROL_h
#define REMOTE_CONTROL_h

#include <Arduino.h>
#include "includes.h"
#include "../config.h"


class Remote_Control  {
public:
    Remote_Control(Remote_Channel channels[]);
    void Setup();
    int16_t Read(uint8_t);

private:
    Remote_Channel* ch[NUM_OF_CHANNELS];

};

#endif
