#include "motor.h"

Motor::Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin) {
    _fwdPin = fwdPin;
    _backPin = backPin;
    _enablePin = enablePin;

    Startup();
    Stop();
}

void Motor::Start() {
    _isRunning = true;
    _speed = 0;
    Enable();
}

void Motor::Stop() {
    _isRunning = false;
    _targetSpeed = 0;
    Disable();
}

void Motor::Run() {
    
    if(IsRunning()) _targetSpeed = _setSpeed;
    JumpToSpeed();
    //if(_speed != _targetSpeed) ChangeToSpeed();
}

bool Motor::IsRunning() {
    return _isRunning;
}

void Motor::setDirection(bool dir) {

    if(_goForward != dir) {
        _goForward = dir;    
    }

    if((_setSpeed < 0 && _goForward) || _setSpeed > 0 && !_goForward) {
        _setSpeed *= -1;
    }

}

bool Motor::getDirection() {
    return _goForward;
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
    _setSpeed > 0 ? setDirection(true) : setDirection(false);

}

uint8_t Motor::getSpeed() {
    return _speed;
}

void Motor::setAcceleration(uint16_t accel) {
    _acceleration = accel;
}

uint16_t Motor::getAcceleration() {
    return _acceleration;
}

void Motor::setDecceleration(uint16_t deccel) {
    _decceleration = deccel;
}

uint16_t Motor::getDecceleration() {
    return _decceleration;
}

void Motor::setAccelDeccel(uint16_t val) {
    setAcceleration(val);
    setDecceleration(val);
}

void Motor::Startup() {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_backPin, OUTPUT);
    pinMode(_enablePin, OUTPUT);
}

void Motor::ChangeToSpeed() {

    long now = millis();
    uint16_t accelDeccel = _decceleration;
    int8_t stepSize = 1;

    if(_targetSpeed > _speed) {
        if(_targetSpeed > 0) accelDeccel = _acceleration;
        stepSize *= 1;
    } else {
        if(_targetSpeed < 0) accelDeccel = _acceleration;
        stepSize *= -1;
    }

    uint16_t timeForChange = accelDeccel/255;

    // if(timeForChange < MOTOR_THREAD/2) {
    //     stepSize *= 4;
    // } 
    // else if(timeForChange < MOTOR_THREAD) {
    //     stepSize *= 2;
    // }

    if(now > _lastSpeedChange + timeForChange && _speed != _targetSpeed) {
        _speed += stepSize;
        if(_speed > 255) _speed = 255;
        else if(_speed < -255) _speed = -255;
        WriteSpeed();
        _lastSpeedChange = now;
    }
    
}

void Motor::JumpToSpeed() {
    _speed = _targetSpeed;
    WriteSpeed();
}

void Motor::WriteSpeed() {

    if(_speed >= 0) {
        analogWrite(_fwdPin, _speed);
        analogWrite(_backPin, 0);
    }
    else if(_speed <= 0) {
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
   if (pwm_pulse <255 & pwm_pulse >0){
     analogWrite(pin_pwm,pwm_pulse);  //set motor speed 
   }
   else{
     if (pwm_pulse>255){
       analogWrite(pin_pwm,255);
     }
     else{
       analogWrite(pin_pwm,0);
     }
   } 
   */