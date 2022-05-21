#include "Car.h"

Car::Car() {
    _steerMotor.setAcceleration(FASTEST_ACCEL);
}

void Car::Run() {
    if(_estop) {
        Brake();
    } else if (IsOverTemp()) {
        Stop();
    } else {
        if(GetPedal() != 0) {
            Go();
        } else {
            Stop();
        }

        if(RemoteOverride(_external_throttle)) {
            int16_t steerVal = 0;
            Start();
            if(_mode == MODE_HIGH) {
                steerVal = _external_steering;
            } 

            SetSpeed(_external_throttle, steerVal);
            
        }

        if(RemoteOverride(_external_steering)) {
            _steerMotor.Start();
            _steerMotor.setSpeed(_external_steering);
            
        } else {
            _steerMotor.Stop();
            _external_steering = 0;
        }


    }

    _driveMotorL.Run();
    _driveMotorR.Run();
    _steerMotor.Run();
}

int8_t Car::GetPedal() {
    if(_fwdSwitch.IsActive()) {
        return 1;
    } else if (_revSwitch.IsActive()) {
        return -1;
    }
    
    return 0;
}

void Car::Go() {
    Start();
    SetSpeed(_maxSpeed * GetPedal(), 0);
}

void Car::Brake() {
    _driveMotorR.EStop();
    _driveMotorL.EStop();
}

bool Car::RemoteOverride(int16_t input) {
    return input != 0 ? true : false;
}

void Car::Stop() {
    SetSpeed(0, 0);
    _driveMotorR.Stop();
    _driveMotorL.Stop();
}

void Car::Start() {
    _driveMotorR.Start();
    _driveMotorL.Start();
}

void Car::SetSpeed(int16_t speed, int16_t steer = 0) {
    int16_t speedLeft = speed;
    int16_t speedRight = speed;

    if(steer < 0) {
        //Turning right
        speedRight -= AdjustSpeedForSteering(speedRight, steer);
    } else if (steer > 0) {
        //Turning Left
        speedLeft -= AdjustSpeedForSteering(speedLeft, steer);
    }

    _driveMotorR.setSpeed(speedRight);
    _driveMotorL.setSpeed(speedLeft);
}

int32_t Car::AdjustSpeedForSteering(int16_t speed, int16_t steer) {
    return (int32_t)speed * (int32_t)abs(steer)/PWM_MAX*(int32_t)_steeringSpeedAdj/STEERING_SPEED_ADJUST_MAX;
}

void Car::SetEStop(bool estop) {
    _estop = estop;
}

void Car::SetSteer(int16_t steer) {
    _external_steering = steer;
}

void Car::SetSteeringSpeedAdj(int16_t val) {
    _steeringSpeedAdj = val;
}

void Car::SetThrottle(int16_t throttle) {
    _external_throttle = throttle;
}

void Car::SetMaxSpeed(int16_t speed) {
    speed >=0 && speed <= PWM_MAX ? _maxSpeed = speed : NULL;
}

void Car::SetAcceleration(int16_t accel) {
    _driveMotorR.setAcceleration(accel);
    _driveMotorL.setAcceleration(accel);
}

void Car::SetMode(int8_t mode) {
    _mode = mode;
}

void Car::SetMode(int8_t mode) {
    _mode = mode;
}

uint16_t Car::GetTemp() {
    return _steerMotor.GetTemp();
}

bool Car::IsOverTemp() {
    return _steerMotor.IsOverTemp() &&
             _driveMotorL.IsOverTemp() && 
             _driveMotorR.IsOverTemp();
}