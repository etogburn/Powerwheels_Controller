#ifndef REMOTE_CONTROL_h
#define REMOTE_CONTROL_h

#include <Arduino.h>
#include "includes.h"
#include "../config.h"
#include "../CarStats.h"

class Remote_Control  {
public:
    Remote_Control();
    int16_t Read(uint8_t);
    int16_t GetThrottle();
    int16_t GetSteering();
    int16_t GetChannel(uint8_t);
    bool GetEStop();
    bool IsAvailable();
    void SetFeedbackVal(int16_t);
    
    Remote GetRemote();
private:
    PPMReader _channelsIn = PPMReader(PPM_STREAM_PIN, NUM_OF_CHANNELS);
    int16_t mapControlChannel(int16_t);
    int16_t mapKnobChannel(int16_t);
};

#endif
