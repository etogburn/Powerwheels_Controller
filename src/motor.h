#ifndef MOTOR_h
#define MOTOR_h

#include <Arduino.h>
#include "../config.h"


class Motor {

public:

    Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin);

    void Start();
    void Stop();
    bool IsRunning();
    void Startup();

    //@brief must be called on a recurring basis to ensure the motor values change appropriately.
    void Run();

    void setSpeed(int16_t);
    int16_t getSpeed();

    void setAcceleration(uint16_t);
    uint16_t getAcceleration();

    void setMaxSpeed(int16_t);

private:
    uint8_t _fwdPin = 0;
    uint8_t _backPin = 0;
    uint8_t _enablePin = 0;

    bool _isRunning = false;

    //Actual output of the motor
    int16_t _speed = 0;
    //maximum speed at full throttle at a given time.
    int16_t _setSpeed = 0;

    int16_t _targetSpeed = 0;
    //absolute maximum speed 
    int16_t _maxSpeed = PWM_MAX;

    uint16_t _acceleration = 1000; //ms from 0 to 100 and 100 to 0

    long _lastSpeedChange = 0;

    void Enable();
    void Disable();
    void AccelToSpeed();
    void JumpToSpeed();
    void WriteSpeed();

};




#endif