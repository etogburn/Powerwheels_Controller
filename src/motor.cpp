#include "Motor.h"

Motor::Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin, uint8_t tempPin) : _motorTemp(tempPin) {
    _fwdPin = fwdPin;
    _backPin = backPin;
    _enablePin = enablePin;
    setAcceleration(_acceleration);

    Startup();
    Stop();
}

void Motor::Start() {
    _isRunning = true;
    _estopActive = false;
    Enable();
}

void Motor::Stop() {
    _isRunning = false;
    _targetSpeed = 0;
}

void Motor::Run() {
    if(IsOverTemp()) {
        Stop();
    } else if(IsRunning()) {
        _targetSpeed = _setSpeed;
    } else if(!_estopActive && getSpeed() == 0) {
        Disable();
    }
    AccelToSpeed();
}

bool Motor::IsRunning() {
    return _isRunning;
}

void Motor::setSpeed(int16_t speed) {
    if(speed <= _maxSpeed && speed >= _maxSpeed * -1) {
        _setSpeed = speed;
    }
    else if(speed < 0 && speed >= _maxSpeed * -1) {
        _setSpeed = speed;
    }
    else if(speed < _maxSpeed * -1) {
        _setSpeed = _maxSpeed * -1;
    }
    else {
        _setSpeed = _maxSpeed;
    }
}

int16_t Motor::getSpeed() {
    return _speed;
}

uint16_t Motor::GetTemp() {
    return _motorTemp.GetTemp();
}

bool Motor::IsOverTemp() {
    if(GetTemp() >= MOTOR_MAX_TEMP) {
        _overTempFault = true;
    } else if (GetTemp() <= MOTOR_MAX_TEMP - MOTOR_TEMP_HYSTERESIS) {
        _overTempFault = false;
    }

    return _overTempFault;
}

void Motor::setAcceleration(uint16_t accel) {
    if(accel < FASTEST_ACCEL) {
        _acceleration = FASTEST_ACCEL;
    }
    if(accel > SLOWEST_ACCEL) {
        _acceleration = SLOWEST_ACCEL;
    }
    else {
        _acceleration = accel;
    }

     _accelStep = MOTOR_THREAD * MAX_FWD/_acceleration;
}

uint16_t Motor::getAcceleration() {
    return _acceleration;
}

void Motor::Startup() {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_backPin, OUTPUT);
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, LOW);
    digitalWrite(_backPin, LOW);
    digitalWrite(_fwdPin, LOW);
}

void Motor::setMaxSpeed(int16_t newMaxSpeed) {
    if(newMaxSpeed > MAX_FWD || newMaxSpeed <  MAX_REV) {
        _maxSpeed = MAX_FWD;
    }
    else {
        _maxSpeed = newMaxSpeed;
    }

    if(abs(_setSpeed) > _maxSpeed) {
        _setSpeed >= 0 ? _setSpeed = _maxSpeed : _setSpeed = _maxSpeed * -1;
    }
}

void Motor::AccelToSpeed() {
    volatile int16_t speedDifference = abs(_targetSpeed - _speed);

    if(speedDifference < _accelStep ) {
        _speed = _targetSpeed;
    }
    else {
        if(_targetSpeed >= _speed) {
        _speed += _accelStep;
        } else if(_targetSpeed < _speed) {
            _speed -= _accelStep;
        }
    }
    WriteSpeed();
}

void Motor::JumpToSpeed() {
    _speed = _targetSpeed;
    WriteSpeed();
}

void Motor::WriteSpeed() {
    if(_speed > MAX_FWD) {
        _speed = MAX_FWD;
    } else if(_speed <  MAX_REV) {
        _speed =  -1 * MAX_REV;
    }

    if(_speed >= 0) {
        analogWrite(_fwdPin, _speed);
        analogWrite(_backPin, 0);
    } else if(_speed <= 0) {
        analogWrite(_backPin, _speed*-1);
        analogWrite(_fwdPin, 0);
    }  
}

void Motor::Enable() {
    digitalWrite(_enablePin, HIGH);
}

void Motor::Disable() {
    digitalWrite(_enablePin, LOW);
}

void Motor::EStop() {
    _speed = 0; 
    _targetSpeed = 0;
    JumpToSpeed();
    _isRunning = false;
    _estopActive = true;
}

/*
Simple PID Control Algorithm.
if (motor_start){
     e_speed = set_speed - pv_speed;
     pwm_pulse = e_speed*kp + e_speed_sum*ki + (e_speed - e_speed_pre)*kd;
     e_speed_pre = e_speed;  //save last (previous) error
     e_speed_sum += e_speed; //sum of error
     if (e_speed_sum >4000) e_speed_sum = 4000;
     if (e_speed_sum <-4000) e_speed_sum = -4000;
   }
   else{
     e_speed = 0;
     e_speed_pre = 0;
     e_speed_sum = 0;
     pwm_pulse = 0;
   }
  //update new speed
   if (pwm_pulse  PWM_MAX & pwm_pulse >0){
     analogWrite(pin_pwm,pwm_pulse);  //set motor speed 
   }
   else{
     if (pwm_pulse PWM_MAX){
       analogWrite(pin_pwm PWM_MAX);
     }
     else{
       analogWrite(pin_pwm,0);
     }
   } 
   */