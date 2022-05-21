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
    void SetMode(int8_t);
    void SetSteeringSpeedAdj(int16_t);

    uint16_t GetTemp();
    bool IsOverTemp();
private:
    void Brake();
    void Start();
    void Stop();
    void Go();
    int8_t GetPedal();
    int32_t AdjustSpeedForSteering(int16_t, int16_t);
    bool RemoteOverride(int16_t);
    void SetSpeed(int16_t, int16_t);

    bool _estop = false;
    bool _lastEStop = false;
    int8_t _mode = 0;
    
    volatile int16_t _external_throttle = 0;
    volatile int16_t _external_steering = 0;

    volatile int16_t _maxSpeed = PWM_MAX;
    volatile int16_t _steeringSpeedAdj = STEERING_SPEED_ADJUST_MAX/2;

    Motor _driveMotorR = Motor(DRIVE_MOTOR_R_EN_PIN, DRIVE_MOTOR_R_FWD_PIN, DRIVE_MOTOR_R_REV_PIN, DRIVE_MOTOR_R_TEMP_PIN);
    Motor _driveMotorL = Motor(DRIVE_MOTOR_L_EN_PIN, DRIVE_MOTOR_L_FWD_PIN, DRIVE_MOTOR_L_REV_PIN, DRIVE_MOTOR_L_TEMP_PIN);
    Motor _steerMotor = Motor(STEER_MOTOR_EN_PIN, STEER_MOTOR_FWD_PIN, STEER_MOTOR_REV_PIN, STEER_MOTOR_TEMP_PIN);

    Switch _fwdSwitch = Switch(FWD_SWITCH, LOW);
    Switch _revSwitch = Switch(REV_SWITCH, LOW);
    Switch _hiLoSwitch = Switch(HILO_SWITCH, LOW);
};

#endif