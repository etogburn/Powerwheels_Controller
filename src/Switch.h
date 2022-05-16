#ifndef SWITCH_h
#define SWITCH_h

#include <Arduino.h>

class Switch {
public:
    Switch(int inputPin, bool activeState);
    void Initialize();
    bool IsActive();
private:
    int16_t _inputPin = 0;
    bool _activeState = LOW;
};

#endif