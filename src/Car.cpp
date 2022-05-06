#include "Car.h"

Car::Car() {

}

void Car::Run() {


    if(_estop) {
        //if(_estop != _lastEStop) {
            Brake();
           // _lastEStop = _estop;
        //} 
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
            _driveMotor.Start();
            _driveMotor.setSpeed(_external_throttle);
        }
    }

    _driveMotor.Run();
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
    _driveMotor.Start();
    _driveMotor.setSpeed(_maxSpeed * GetPedal());
}

void Car::Brake() {
    _driveMotor.EStop();
}

bool Car::RemoteOverride(int16_t input) {
    if(input != 0) {
        return true;
    } else {
        return false;
    }
}

void Car::Stop() {
    _driveMotor.setSpeed(0);
    if(_driveMotor.getSpeed() == 0) _driveMotor.Stop();
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
        _driveMotor.setAcceleration(accel);
    }
}

