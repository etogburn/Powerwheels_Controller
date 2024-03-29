//Configuration values all go here
#include <Arduino.h>

#define VERSION "1.1.2"
#define START_MESSAGE "Powerwheels"

//#define SERIAL_DEBUG //Unsupported on the Arduino Mega at this time.
#define USE_UIM

#define TIME_SCREEN_CHANGE 5000 //ms between going to a different screen
#define TIME_LCD_UPDATE 150 //ms between display updates
#define TIME_BUTTON_READ 25 //ms between button reads

#define PPM_STREAM_PIN 19 //must be interrupt pin.

//In car Switches
#define FWD_SWITCH A1
#define REV_SWITCH A2
#define HILO_SWITCH A0

//Motor Settings
#define MOTOR_THREAD 50 //ms -- 16000000/64/65536 = 3.82 hz or ~250ms max. 250,000 hz or 4us min.

#define FASTEST_ACCEL 50//ms
#define SLOWEST_ACCEL 4000 //ms
#define ACCEL_DEFAULT 1000

#define PWM_MAX 255
#define MAX_FWD PWM_MAX
#define MAX_REV MAX_FWD * -1

//Divisor/Dvidend for the adjustment of the inside back wheel when turning. used as a percentage.
#define STEERING_SPEED_ADJUST_MIN 0
#define STEERING_SPEED_ADJUST_MAX 1000

#define MOTOR_MAX_TEMP 70 //degrees C
#define MOTOR_TEMP_HYSTERESIS 5
#define TEMP_CALC_FREQ 1000 //ms

#define DRIVE_MOTOR_R_EN_PIN 10
#define DRIVE_MOTOR_R_FWD_PIN 8
#define DRIVE_MOTOR_R_REV_PIN 9
#define DRIVE_MOTOR_R_TEMP_PIN A9

#define DRIVE_MOTOR_L_EN_PIN 13
#define DRIVE_MOTOR_L_FWD_PIN 12
#define DRIVE_MOTOR_L_REV_PIN 11
#define DRIVE_MOTOR_L_TEMP_PIN A8

#define STEER_MOTOR_EN_PIN 7
#define STEER_MOTOR_FWD_PIN 6
#define STEER_MOTOR_REV_PIN 5
#define STEER_MOTOR_TEMP_PIN A10

//RC Transmitter/Reciever Settings
#define REMOTE_FEEDBACK_PIN 2

#define NUM_OF_CHANNELS 7
#define MAX_FEEDBACK_VOLTAGE 48 //4.8v
#define MIN_FEEDBACK_VOLTAGE 0 //0v

#define CENTER_PULSE_VALUE 1500
#define MAX_PULSE_VALUE 2000
#define MIN_PULSE_VALUE 1000

#define CENTER_DEADBAND 20 //for throttle/steering counts on either side of the center point
#define DEADBAND_MAX CENTER_PULSE_VALUE + CENTER_DEADBAND
#define DEADBAND_MIN CENTER_PULSE_VALUE - CENTER_DEADBAND

#define ESTOP_THRESHOLD 1505 //above this is active 
#define ESTOP_RESET_TIME 250 //ms - time required to read low on the estop in order to reset it
#define MODE_SWITCH_LOW_MID 250
#define MODE_SWITCH_MID_HIGH 750

#define MODE_HIGH 1
#define MODE_MED 0
#define MODE_LOW -1

#define MIN_KNOB_VAL 0
#define MAX_KNOB_VAL 1000

#define STEER_IDX 1
#define THROTTLE_IDX 2
#define ESTOP_IDX 3
#define CH4_IDX 4
#define CH5_IDX 5
#define CH6_IDX 6
#define CH7_IDX 7
//End RC Reciever settings
//TIMER settings for motor interrupt thread
#define TIMER_PRESCALE 64
#define TIMER_PRELOAD 65536 - 16000000/TIMER_PRESCALE/(1000/MOTOR_THREAD) // preload timer 65536-16MHz/8/100Hz. 20000 timer counts.

#if TIMER_PRESCALE == 1
  #define TIMER_PRESCALER (1 << CS50) 
#elif TIMER_PRESCALE == 8
  #define TIMER_PRESCALER (1 << CS51) 
#elif TIMER_PRESCALE == 64
  #define TIMER_PRESCALER (1 << CS51) | (1 << CS50) 
#elif TIMER_PRESCALE == 256
  #define TIMER_PRESCALER (1 << CS52) 
#elif TIMER_PRESCALE == 1024
  #define TIMER_PRESCALER (1 << CS52) | (1 << CS50) 
#else
  #error Invalid Timer Prescale value.
#endif

#if TIMER_PRELOAD >= 65536
  #error Motor thread is too long.
#elif TIMER_PRELOAD < 0
  #error MotorThread is too short.
#endif

#ifdef __SAM3X8E__
  #define DUE_BOARD
#endif

#ifdef __AVR_ATmega2560__ 
  #define MEGA_BOARD
#endif