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
    } else {
        return 0;
    }
}

void Car::Go() {
    Start();
    SetSpeed(_maxSpeed * GetPedal());
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
    SetSpeed(0);
    if(_driveMotorR.getSpeed() == 0) _driveMotorR.Stop();
    if(_driveMotorL.getSpeed() == 0) _driveMotorL.Stop();
}

void Car::Start() {
    _driveMotorR.Start();
    _driveMotorL.Start();
}

void Car::SetSpeed(int16_t speed, int16_t steer = 0) {
    int16_t speedLeft = speed;
    int16_t speedRight = speed;

    if(steer > 0) {
        //Turning right
        speedLeft -= speedLeft*abs(steer)/PWM_MAX/STEERING_SPEED_ADJUST_DIVISOR;
    } else if (steer < 0) {
        //Turning Left
        speedLeft -= speedRight*abs(steer)/PWM_MAX/STEERING_SPEED_ADJUST_DIVISOR;
    }

    _driveMotorR.setSpeed(speedRight);
    _driveMotorL.setSpeed(speedLeft);
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
    _driveMotorR.setAcceleration(accel);
    _driveMotorL.setAcceleration(accel);
}

void Car::SetMode(int8_t mode) {
    _mode = mode;
}

