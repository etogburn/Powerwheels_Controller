#ifndef CARSTATS_h
#define CARSTATS_h

#include "config.h"
#include <Arduino.h>

struct MotorStats {
  int16_t speed = 0;
  int16_t temp = 0;
};

struct Remote {
  int16_t steer = 0;
  int16_t throttle = 0;
  bool estop = false;
  int16_t channel4 = 0;
  int16_t channel5 = 0;
  int16_t channel6 = 0;
  int16_t channel7 = 0;
};

struct CarStats {
  MotorStats motorDriveL;
  MotorStats motorDriveR;
  MotorStats motorSteer;

  Remote remote;
  int16_t mode = 0;
  int16_t speed = 0;
  uint16_t maxSpeed = PWM_MAX;
  int16_t acceleration = ACCEL_DEFAULT;
  int16_t steerSpeedAdj = STEERING_SPEED_ADJUST_MAX/2;

  bool overTemp = false;
  int8_t pedal = 0;
};

#endif