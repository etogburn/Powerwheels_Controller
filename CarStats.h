#ifndef CARSTATS_h
#define CARSTATS_h

#include "config.h"
#include <Arduino.h>

struct MotorStats {
  int16_t speed = 0;
  int16_t temp = 0;
};

struct CarStats {

  MotorStats motorDriveL;
  MotorStats motorDriveR;
  MotorStats motorSteer;

  uint16_t maxSpeed = PWM_MAX;
  int16_t acceleration = ACCEL_DEFAULT;
  int16_t steerSpeedAdj = STEERING_SPEED_ADJUST_MAX/2;

  int8_t pedal = 0;

  bool estop = false;
  int8_t mode = MODE_LOW;

  int16_t remoteSteer = 0;
  int16_t remoteThrottle = 0;
  int16_t remoteLKnob = 0;
  int16_t remoteRKnob = 0;
};

#endif