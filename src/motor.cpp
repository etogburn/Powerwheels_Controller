#include "motor.h"

Motor::Motor(uint8_t fwdPin, uint8_t backPin) {
    _fwdPin = fwdPin;
    _backPin = backPin;

    Startup();
}

void Motor::Start() {
    _isRunning = true;
    _speed = 0;
}

void Motor::Stop() {
    _isRunning = false;
    _targetSpeed = 0;
}

void Motor::Run() {
    
    if(IsRunning()) _targetSpeed = _setSpeed;
    if(_speed != _targetSpeed) ChangeToSpeed();
}

bool Motor::IsRunning() {
    return _isRunning;
}

void Motor::setDirection(bool dir) {

    if(_goForward != dir) {
        _goForward = dir;
        _setSpeed *= -1;
    }

}

bool Motor::getDirection() {
    return _goForward;
}

void Motor::setSpeed(uint8_t speed) {
    int8_t multiplier = 1;
    if(!_goForward) multiplier = -1;

    if(speed <= _maxSpeed && speed > 0) {
        _setSpeed = speed * multiplier;
    }
    else if(speed > _maxSpeed) {
        _setSpeed = _maxSpeed * multiplier;
    }
    else {
        _setSpeed = 0;
    }
}

uint8_t Motor::getSpeed() {
    return abs(_speed);
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

    if(timeForChange < MOTOR_THREAD/2) {
        stepSize *= 4;
    } 
    else if(timeForChange < MOTOR_THREAD) {
        stepSize *= 2;
    }

    if(now > _lastSpeedChange + timeForChange && _speed != _targetSpeed) {
        _speed += stepSize;
        if(_speed > 255) _speed = 255;
        else if(_speed < -255) _speed = -255;
        WriteSpeed();
        _lastSpeedChange = now;
    }
    
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

