#include "Car.h"

Car::Car() {
    _steerMotor.setAcceleration(FASTEST_ACCEL);
}

void Car::Run() {
    if(_car.remote.estop) {
        Brake();
    } else if (_car.overTemp) {
        Stop();
    } else {
        if(_car.pedal != 0) {
            Go();
        } else {
            Stop();
        }

        if(RemoteOverride(_car.remote.throttle)) {
            int16_t steerVal = 0;
            Start();
            if(_car.mode== MODE_HIGH) {
                steerVal = _car.remote.steer;
            } 

            SetSpeed(_car.remote.throttle, steerVal);
        }

        if(RemoteOverride(_car.remote.steer)) {
            _steerMotor.Start();
            _steerMotor.setSpeed(_car.remote.steer);
            
        } else {
            _steerMotor.Stop();
            _car.remote.steer = 0;
        }
    }

    _driveMotorL.Run();
    _driveMotorR.Run();
    _steerMotor.Run();
}

void Car::Calculate() {
    _driveMotorL.Calculate();
    _driveMotorR.Calculate();
    _steerMotor.Calculate();

    GetPedal();
    IsOverTemp();
    GetSpeed();
    GetTemp();
}

void Car::Go() {
    Start();
    SetSpeed(_car.maxSpeed * _car.pedal, 0);
}

void Car::Brake() {
    _driveMotorR.EStop();
    _driveMotorL.EStop();
    _steerMotor.Stop();
}

bool Car::RemoteOverride(int16_t input) {
    return input != 0 ? true : false;
}

bool Car::IsOverTemp() {
    _car.overTemp = _driveMotorL.IsOverTemp() ||
             _driveMotorR.IsOverTemp();

    return _car.overTemp;
}

void Car::Stop() {
    SetSpeed(0, 0);
    _driveMotorR.Stop();
    _driveMotorL.Stop();
    _steerMotor.Stop();
}

void Car::Start() {
    _driveMotorR.Start();
    _driveMotorL.Start();
}

int32_t Car::AdjustSpeedForSteering(int16_t speed, int16_t steer) {
    return (int32_t)speed * (int32_t)abs(steer)/PWM_MAX*(int32_t)_car.steerSpeedAdj/STEERING_SPEED_ADJUST_MAX;
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

uint16_t Car::GetTemp() {
    _car.motorDriveL.temp = _driveMotorL.GetTemp();
    _car.motorDriveR.temp = _driveMotorR.GetTemp();
    _car.motorSteer.temp = _steerMotor.GetTemp();
    return max(max(_driveMotorL.GetTemp(), _driveMotorR.GetTemp()), _steerMotor.GetTemp());;
}

int16_t Car::GetSpeed() {
    _car.motorDriveL.speed = _driveMotorL.getSpeed();
    _car.motorDriveR.speed = _driveMotorR.getSpeed();

    if(_car.motorDriveL.speed <= 0 && _car.motorDriveR.speed <= 0) {
        _car.speed = min(_car.motorDriveL.speed, _car.motorDriveR.speed);
    } else if(_car.motorDriveL.speed >= 0 && _car.motorDriveR.speed >= 0) {
        _car.speed = max(_car.motorDriveL.speed, _car.motorDriveR.speed);
    } else {
        _car.speed = 0;
    }

    return max(_driveMotorL.getSpeed(), _driveMotorR.getSpeed());
}

uint16_t Car::GetRTemp() {
    return _car.motorDriveR.temp;
}

uint16_t Car::GetLTemp() {
    return _car.motorDriveL.temp;
}

CarStats Car::GetStats() {
    return _car;
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

void Car::SetMaxSpeed(int16_t speed) {
    speed >=0 && speed <= PWM_MAX ? _car.maxSpeed = speed : 0;
}

void Car::SetAcceleration(int16_t accel) {
    _car.acceleration = accel;
    _driveMotorR.setAcceleration(accel);
    _driveMotorL.setAcceleration(accel);
}

void Car::SetRemote(Remote remote) {
    SetMode(remote.channel4);
    SetEStop(remote.estop);
    SetThrottle(remote.throttle);
    SetSteer(remote.steer);
    _car.remote = remote;
}

void Car::SetEStop(bool estop) {
    _car.remote.estop = estop;
}

void Car::SetSteer(int16_t steer) {
    _car.remote.steer = steer;
}

void Car::SetSteeringSpeedAdj(int16_t val) {
    _car.steerSpeedAdj = val;
}

void Car::SetThrottle(int16_t throttle) {
    _car.remote.throttle = throttle;
}

void Car::SetMode(int8_t input) {
    if(input > MODE_SWITCH_MID_HIGH) {
        _car.mode = MODE_HIGH;
    } else if(input < MODE_SWITCH_LOW_MID) {
        _car.mode = MODE_LOW;
    } else {
        _car.mode = MODE_MED;
    }
}
