#ifndef REMOTE_CONTROL_h
#define REMOTE_CONTROL_h

#include <Arduino.h>
#include "includes.h"
#include "../config.h"
#include "../CarStats.h"

#ifdef IBUS_RECIEVER
    #include "lib/IBusBM/src/IBusBM.h"
#endif

class Remote_Control  {
public:
    #ifdef IBUS_RECIEVER
        Remote_Control(HardwareSerial &inputSerial);
    #else
        Remote_Control(Remote_Channel channels[]);
    #endif
    void Setup();
    void Listen();
    int16_t Read(uint8_t);
    int16_t GetThrottle();
    int16_t GetSteering();
    int16_t GetLKnob();
    int16_t GetRKnob();
    bool GetEStop();
    int8_t GetMode();
    CarStats GetRemote();
private:
    #ifdef IBUS_RECIEVER
        IBusBM _iBusInput;
    #else
        Remote_Channel* ch[NUM_OF_CHANNELS];
        uint8_t _channelToListen = 0;
    #endif
    int16_t mapControlChannel(int16_t);
    int16_t mapKnobChannel(int16_t);
};

#endif
