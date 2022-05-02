#include "Remote_Channel.h"

Remote_Channel::Remote_Channel(uint8_t recievePin, long timeBetweenReads) {
    _recievePin = recievePin;
    _timeBetweenReads = timeBetweenReads;
    //Startup();
}

int16_t Remote_Channel::Read() {
    CalculateAverage();
    return _avgValue;
}

void Remote_Channel::Startup() {
    pinMode(_recievePin, INPUT);
}

void Remote_Channel::Listen() {
    ReadPulseWidth();
}

void Remote_Channel::ReadPulseWidth() {
    long now = millis();

    if(now > _lastTimeRead + _timeBetweenReads) {
        uint8_t idxToRead = _lastReadIdx >= VALUES_TO_AVERAGE - 1 ? 0 : _lastReadIdx + 1;

        _rawValues[idxToRead] = pulseIn(_recievePin, HIGH, PULSEIN_TIMEOUT);
        _lastReadIdx = idxToRead;
        _lastTimeRead = now;
    }
    
}

void Remote_Channel::CalculateAverage() {
    int16_t runningAvg = 0;

    for(uint8_t i = 0; i < VALUES_TO_AVERAGE; i++) {
        runningAvg += _rawValues[i]/VALUES_TO_AVERAGE;
    }
    
    _avgValue = runningAvg;
}