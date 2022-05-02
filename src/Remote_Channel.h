#ifndef REMOTE_CHANNEL_h
#define REMOTE_CHANNEL_h

#include <Arduino.h>

#define PULSEIN_TIMEOUT 25000
#define CENTER_PULSE_VALUE 1500

#define VALUES_TO_AVERAGE 1

class Remote_Channel {
public:
    Remote_Channel(uint8_t recievePin, long timeBetweenReads);
    int16_t Read();
    void Listen();
    void Startup();

private:
    uint8_t _recievePin;
    int16_t _avgValue = CENTER_PULSE_VALUE;
    uint8_t _lastReadIdx = VALUES_TO_AVERAGE;
    int16_t _rawValues[VALUES_TO_AVERAGE] = {
                                                // CENTER_PULSE_VALUE, 
                                                // CENTER_PULSE_VALUE,
                                                // CENTER_PULSE_VALUE,
                                                // CENTER_PULSE_VALUE,
                                                CENTER_PULSE_VALUE
                                            };
    long _lastTimeRead = 0;
    long _timeBetweenReads = 0;
    void ReadPulseWidth();
    void CalculateAverage();
};


#endif
