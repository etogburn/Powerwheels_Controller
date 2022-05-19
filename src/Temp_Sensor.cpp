#include "Temp_Sensor.h"

Temp_Sensor::Temp_Sensor(uint8_t pin) {
    _pin = pin;
    Initialize();   
}

void Temp_Sensor::Initialize() {
    pinMode(_pin, INPUT);
}

uint16_t Temp_Sensor::ReadPin() {
    _rawValue = analogRead(_pin);
    return _rawValue;
}