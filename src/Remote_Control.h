#ifndef REMOTE_CONTROL_h
#define REMOTE_CONTROL_h

#include <Arduino.h>
#include "includes.h"
#include "../config.h"
#include "../CarStats.h"

class Remote_Control  {
public:
    Remote_Control(Remote_Channel channels[]);
    void Setup();
    void Listen();
    int16_t GetThrottle();
    int16_t GetSteering();
    int16_t GetLKnob();
    int16_t GetRKnob();
    bool GetEStop();
    int8_t GetMode();
    CarStats GetRemote();
private:
    Remote_Channel* ch[NUM_OF_CHANNELS];

    int16_t Read(uint8_t);
    int16_t mapControlChannel(int16_t);
    int16_t mapKnobChannel(int16_t);
};

#endif
