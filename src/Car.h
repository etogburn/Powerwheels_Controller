#ifndef CAR_h
#define CAR_h

#include "../config.h"
#include "includes.h"

class Car {
public:
    Car();
    void Run();

    void SetSteer(int16_t);
    void SetThrottle(int16_t);
    void SetAcceleration(int16_t);
    void SetMaxSpeed(int16_t);
    void SetEStop(bool);
    
private:
    void Brake();
    void Stop();
    void Go();
    int8_t GetPedal();
    bool RemoteOverride(int16_t);

    bool _estop = false;
    bool _lastEStop = false;
    
    int16_t _external_throttle = 0;
    int16_t _external_steering = 0;

    uint16_t _maxSpeed = PWM_MAX;

    Motor _driveMotor = Motor(DRIVE_MOTOR_EN_PIN, DRIVE_MOTOR_FWD_PIN, DRIVE_MOTOR_REV_PIN, ACCEL_DEFAULT);
    Motor _steerMotor = Motor(STEER_MOTOR_EN_PIN, STEER_MOTOR_FWD_PIN, STEER_MOTOR_REV_PIN, FASTEST_ACCEL);

    Switch _fwdSwitch = Switch(FWD_SWITCH, LOW);
    Switch _revSwitch = Switch(REV_SWITCH, LOW);
    Switch _hiLoSwitch = Switch(HILO_SWITCH, LOW);

};


#endif