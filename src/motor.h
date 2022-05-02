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
    
    // @brief An input of true is forward. false is backwards.
    void setDirection(bool);
    bool getDirection();

    void setSpeed(int16_t);
    uint8_t getSpeed();

    void setAcceleration(uint16_t);
    uint16_t getAcceleration();
    void setDecceleration(uint16_t);
    uint16_t getDecceleration();

    void setAccelDeccel(uint16_t);

private:
    uint8_t _fwdPin = 0;
    uint8_t _backPin = 0;
    uint8_t _enablePin = 0;

    bool _isRunning = false;
    bool _goForward = true;

    //Actual output of the motor
    int16_t _speed = 0;
    //maximum speed at full throttle at a given time.
    int16_t _setSpeed = 255;

    int16_t _targetSpeed = 0;
    //absolute maximum speed 
    int16_t _maxSpeed = 255;

    uint16_t _acceleration = 1000; //ms from 0 to 100
    uint16_t _decceleration = _acceleration; //ms from 100 to 0

    long _lastSpeedChange = 0;

    void Enable();
    void Disable();
    void ChangeToSpeed();
    void JumpToSpeed();
    void WriteSpeed();

};




#endif