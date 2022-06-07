#ifndef CAR_h
#define CAR_h

#include "../config.h"
#include "../CarStats.h"
#include "includes.h"

class Car {
public:
    Car();
    void Run();
    void Calculate();

    void SetSteer(int16_t);
    void SetThrottle(int16_t);
    void SetAcceleration(int16_t);
    void SetMaxSpeed(int16_t);
    void SetEStop(bool);
    void SetMode(int8_t);
    void SetSteeringSpeedAdj(int16_t);
    void SetRemote(Remote);

    uint16_t GetRTemp();
    uint16_t GetLTemp();
    uint16_t GetTemp();
    bool IsOverTemp();

    CarStats GetStats();
private:
    void Brake();
    void Start();
    void Stop();
    void Go();
    int8_t GetPedal();
    int32_t AdjustSpeedForSteering(int16_t, int16_t);
    bool RemoteOverride(int16_t);
    void SetSpeed(int16_t, int16_t);
    int16_t GetSpeed();
    
    bool _lastEStop = false;

    CarStats _car;

    Motor _driveMotorR = Motor(DRIVE_MOTOR_R_EN_PIN, DRIVE_MOTOR_R_FWD_PIN, DRIVE_MOTOR_R_REV_PIN, DRIVE_MOTOR_R_TEMP_PIN);
    Motor _driveMotorL = Motor(DRIVE_MOTOR_L_EN_PIN, DRIVE_MOTOR_L_FWD_PIN, DRIVE_MOTOR_L_REV_PIN, DRIVE_MOTOR_L_TEMP_PIN);
    Position_Motor _steerMotor = Position_Motor(STEER_MOTOR_EN_PIN, STEER_MOTOR_FWD_PIN, STEER_MOTOR_REV_PIN, STEER_MOTOR_TEMP_PIN, STEER_FEEDBACK_PIN);

    Switch _fwdSwitch = Switch(FWD_SWITCH, LOW);
    Switch _revSwitch = Switch(REV_SWITCH, LOW);
    Switch _hiLoSwitch = Switch(HILO_SWITCH, LOW);
};

#endif