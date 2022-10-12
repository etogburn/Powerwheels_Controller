#ifndef SWITCH_h
#define SWITCH_h

#include <Arduino.h>

class Switch {
public:
    Switch(int inputPin, bool activeState);
    // @brief Returns if the switch is Active.
    bool IsActive();
private:
    void Initialize();
    int16_t _inputPin = 0;
    bool _activeState = LOW;
};

#endif