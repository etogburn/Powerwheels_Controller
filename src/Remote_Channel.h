#ifndef REMOTE_CHANNEL_h
#define REMOTE_CHANNEL_h

#include <Arduino.h>
#include "../config.h"

class Remote_Channel {

public:
    Remote_Channel(uint8_t recievePin, long timeBetweenReads = 0);
    int16_t Read();
    void Listen();
    void Startup(void (*ISR_callback)(void) = NULL);
    void ListenInterrupt();

private:
    uint8_t _recievePin;
    bool _useInterrupt = false;
    volatile int16_t _rawValue = CENTER_PULSE_VALUE;
    long _lastTimeRead = 0;
    long _timeBetweenReads = 0;
    volatile int _interrupt_timer_start = 0;
    void CalcPulseWidthPulseIn();
    void CalcPulseWidthInterrupt();

};


#endif
