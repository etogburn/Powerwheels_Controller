#include "Motor.h"

Motor::Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin, int16_t acceleration) {
    _fwdPin = fwdPin;
    _backPin = backPin;
    _enablePin = enablePin;
    setAcceleration(acceleration);

    Startup();
    Stop();
}

void Motor::Start() {
    _isRunning = true;
    //_speed = 0;
    Enable();
}

void Motor::Stop() {
    _isRunning = false;
    _targetSpeed = 0;
    Disable();
}

void Motor::Run() {
    
    if(IsRunning()) _targetSpeed = _setSpeed;
    //if(_acceleration < 100) JumpToSpeed();
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

     _accelStep = MOTOR_THREAD/(_acceleration/PWM_MAX);
}

uint16_t Motor::getAcceleration() {
    return _acceleration;
}

void Motor::Startup() {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_backPin, OUTPUT);
    pinMode(_enablePin, OUTPUT);
}

void Motor::setMaxSpeed(int16_t newMaxSpeed) {
    if(newMaxSpeed > PWM_MAX || newMaxSpeed <  PWM_MAX * -1) {
        _maxSpeed = PWM_MAX;
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
  
    if(speedDifference < _accelStep) {
        JumpToSpeed();
    }
    else {
        if(_targetSpeed >= _speed) {
        _speed += _accelStep;
        } else if(_targetSpeed < _speed) {
            _speed -= _accelStep;
        }
        if(_speed > PWM_MAX) _speed = PWM_MAX;
        else if(_speed <  PWM_MAX * -1) _speed =  -1 * PWM_MAX;
        WriteSpeed();
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

void Motor::EStop() {
    _speed = 0; 
    _targetSpeed = 0;
    JumpToSpeed();
    _isRunning = false;
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