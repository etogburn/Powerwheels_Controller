#ifndef REMOTE_CHANNEL_h
#define REMOTE_CHANNEL_h

#include <Arduino.h>
#include "../config.h"

#define NUM_TO_AVG 1

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
    volatile int16_t _rawValue[NUM_TO_AVG] = {CENTER_PULSE_VALUE};//, CENTER_PULSE_VALUE, CENTER_PULSE_VALUE};
    uint8_t _idx = 0;
    long _lastTimeRead = 0;
    long _timeBetweenReads = 0;
    volatile int _interrupt_timer_start = 0;
    void CalcPulseWidthPulseIn();
    void CalcPulseWidthInterrupt();
    void IncrementIDX();
};

#endif
