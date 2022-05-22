#include "Car.h"

Car::Car() {
    _steerMotor.setAcceleration(FASTEST_ACCEL);
}

void Car::Run() {
    if(_car.estop) {
        Brake();
    } else if (IsOverTemp()) {
        Stop();
    } else {
        if(GetPedal() != 0) {
            Go();
        } else {
            Stop();
        }

        if(RemoteOverride(_car.remoteThrottle)) {
            int16_t steerVal = 0;
            Start();
            if(_car.mode == MODE_HIGH) {
                steerVal = _car.remoteSteer;
            } 

            SetSpeed(_car.remoteThrottle, steerVal);
        }

        if(RemoteOverride(_car.remoteSteer)) {
            _steerMotor.Start();
            _steerMotor.setSpeed(_car.remoteSteer);
            
        } else {
            _steerMotor.Stop();
            _car.remoteSteer = 0;
        }
    }

    _driveMotorL.Run();
    _driveMotorR.Run();
    _steerMotor.Run();
}

int8_t Car::GetPedal() {
    if(_fwdSwitch.IsActive()) {
        _car.pedal = 1;
    } else if (_revSwitch.IsActive()) {
        _car.pedal = -1;
    } else {
        _car.pedal = 0;
    }
    
    return _car.pedal;
}

void Car::Go() {
    Start();
    SetSpeed(_car.maxSpeed * GetPedal(), 0);
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
    return (int32_t)speed * (int32_t)abs(steer)/PWM_MAX*(int32_t)_car.steerSpeedAdj/STEERING_SPEED_ADJUST_MAX;
}

void Car::SetEStop(bool estop) {
    _car.estop = estop;
}

void Car::SetSteer(int16_t steer) {
    _car.remoteSteer = steer;
}

void Car::SetSteeringSpeedAdj(int16_t val) {
    _car.steerSpeedAdj = val;
}

void Car::SetThrottle(int16_t throttle) {
    _car.remoteThrottle = throttle;
}

void Car::SetMaxSpeed(int16_t speed) {
    speed >=0 && speed <= PWM_MAX ? _car.maxSpeed = speed : 0;
}

void Car::SetAcceleration(int16_t accel) {
    _car.acceleration = accel;
    _driveMotorR.setAcceleration(accel);
    _driveMotorL.setAcceleration(accel);
}

void Car::SetMode(int8_t mode) {
    _car.mode = mode;
}

uint16_t Car::GetTemp() {
    _car.motorDriveL.temp = _driveMotorL.GetTemp();
    _car.motorDriveR.temp = _driveMotorR.GetTemp();
    _car.motorSteer.temp = _steerMotor.GetTemp();
    return max(max(_driveMotorL.GetTemp(), _driveMotorR.GetTemp()), _steerMotor.GetTemp());;
}

int16_t Car::GetSpeed() {
    _car.motorDriveL.speed = _driveMotorL.getSpeed();
    _car.motorDriveR.speed = _driveMotorR.getSpeed();
    return max(_driveMotorL.getSpeed(), _driveMotorR.getSpeed());
}

bool Car::IsOverTemp() {
    return _steerMotor.IsOverTemp() &&
             _driveMotorL.IsOverTemp() && 
             _driveMotorR.IsOverTemp();
}

CarStats Car::GetStats() {
    GetSpeed();
    GetTemp();
    return _car;
}

void Car::SetRemote(CarStats car) {
    SetMode(car.mode);
    SetEStop(car.estop);
    SetThrottle(car.remoteThrottle);
    SetSteer(car.remoteSteer);
    _car.remoteLKnob = car.remoteLKnob;
    _car.remoteRKnob = car.remoteRKnob;
}