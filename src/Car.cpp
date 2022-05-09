#include "Car.h"

Car::Car() {

}

void Car::Run() {


    if(_estop) {
        Brake();
    } 
    else {

        if(GetPedal() != 0) {
            Go();
        } else {
            Stop();
        }

        if(RemoteOverride(_external_steering)) {
            _steerMotor.Start();
            _steerMotor.setSpeed(_external_steering);
        } else {
            _steerMotor.Stop();
            _external_steering = 0;
        }

        if(RemoteOverride(_external_throttle)) {
            _driveMotorR.Start();
            _driveMotorL.Start();
            _driveMotorR.setSpeed(_external_throttle);
            _driveMotorL.setSpeed(_external_throttle);
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
    } else {
        return 0;
    }
}

void Car::Go() {
    _driveMotorR.Start();
    _driveMotorL.Start();
    _driveMotorR.setSpeed(_maxSpeed * GetPedal());
    _driveMotorL.setSpeed(_maxSpeed * GetPedal());
}

void Car::Brake() {
    _driveMotorR.EStop();
    _driveMotorL.EStop();
}

bool Car::RemoteOverride(int16_t input) {
    if(input != 0) {
        return true;
    } else {
        return false;
    }
}

void Car::Stop() {
    _driveMotorR.setSpeed(0);
    _driveMotorL.setSpeed(0);
    if(_driveMotorR.getSpeed() == 0) _driveMotorR.Stop();
    if(_driveMotorL.getSpeed() == 0) _driveMotorL.Stop();
}

void Car::SetEStop(bool estop) {
    _estop = estop;
}

void Car::SetSteer(int16_t steer) {
    _external_steering = steer;
}

void Car::SetThrottle(int16_t throttle) {
    _external_throttle = throttle;
}

void Car::SetMaxSpeed(int16_t speed) {
    if(speed >=0 && speed <= PWM_MAX) _maxSpeed = speed;
}

void Car::SetAcceleration(int16_t accel) {
    if(accel >= FASTEST_ACCEL && accel <= SLOWEST_ACCEL) {
        _driveMotorR.setAcceleration(accel);
        _driveMotorL.setAcceleration(accel);
    }
}

