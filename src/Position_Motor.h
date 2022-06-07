#ifndef POSITION_MOTOR_h
#define POSITION_MOTOR_h

// https://forum.arduino.cc/t/faster-analog-read/6604/16

#include "motor.h"
#include <Arduino.h>

class Position_Motor: private Motor {
public:
    Position_Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin, uint8_t tempPin, uint8_t feedbackPin);
    void Run();
    void Calculate();

    void SetPosition(int16_t);

    using Motor::GetTemp;
    using Motor::Stop;
    using Motor::Start;
};

#endif