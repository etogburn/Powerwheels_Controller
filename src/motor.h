#ifndef MOTOR_h
#define MOTOR_h

#include <Arduino.h>
#include "../config.h"
#include "Temp_Sensor.h"

class Motor {
public:
    Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin, uint8_t tempPin);

    // @brief Enables the motor drivers and allows the speed to be set.
    void Start();
    // @brief Stops the motor safely. Allows accel rate down to stop before disabling.
    void Stop();
    // @brief returns the state of the motor.
    bool IsRunning();    

    // @brief Must be called on a recurring basis to ensure the motor values change appropriately.
    void Run();
    // @brief set's the speed between FWD_MAX and REV_MAX (reverse is negative, forward is positive)
    void setSpeed(int16_t);
    // @brief returns the current speed. negative is reverse. forward is positive.
    int16_t getSpeed();
    // @brief set acceleration rate between FASTEST_ACCEL and SLOWEST_ACCEL
    void setAcceleration(uint16_t);
    // @brief returns the current acceleration
    uint16_t getAcceleration();
    // @brief Sets the max speed of the motor.
    void setMaxSpeed(int16_t);
    // @brief Locks the motor to 0 speed and leaves drivers active to prevent free rolling. Must call Start() after to reenable.
    void EStop();

    uint16_t GetTemp();
    bool IsOverTemp();
    
private:
    long _lastThread = 0;
    uint8_t _fwdPin = 0;
    uint8_t _backPin = 0;
    uint8_t _enablePin = 0;

    bool _isRunning = false;
    bool _estopActive = false;

    //Actual output of the motor
    volatile int16_t _speed = 0;
    //maximum speed at full throttle at a given time.
    int16_t _setSpeed = 0;

    volatile int16_t _targetSpeed = 0;
    //absolute maximum speed 
    int16_t _maxSpeed = MAX_FWD;

    int16_t _acceleration = ACCEL_DEFAULT; //ms from 0 to 100 and 100 to 0
    volatile int16_t _accelStep =  0;

    long _lastSpeedChange = 0;

    void Startup();
    void Enable();
    void Disable();
    void AccelToSpeed();
    void JumpToSpeed();
    void WriteSpeed();

    bool _overTempFault = false;

    Temp_Sensor _motorTemp;
};

#endif