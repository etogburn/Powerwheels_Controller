//Configuration values all go here

#define VERSION "1.0.0"

//In car Switches
#define FWD_SWITCH A1
#define REV_SWITCH A2
#define HILO_SWITCH A0

//Motor Settings
#define MOTOR_THREAD 50 //ms -- 16000000/64/65536 = 3.82 hz or ~250ms max. 250,000 hz or 4us min.

#define FASTEST_ACCEL 50//ms
#define SLOWEST_ACCEL 3000 //ms
#define ACCEL_DEFAULT 1000

#define MOTOR_MAX_TEMP 70 //degrees C
#define MOTOR_TEMP_HYSTERESIS 5

#define DRIVE_MOTOR_R_EN_PIN 10
#define DRIVE_MOTOR_R_FWD_PIN 8
#define DRIVE_MOTOR_R_REV_PIN 9
#define DRIVE_MOTOR_R_TEMP_PIN A8

#define DRIVE_MOTOR_L_EN_PIN 13
#define DRIVE_MOTOR_L_FWD_PIN 12
#define DRIVE_MOTOR_L_REV_PIN 11
#define DRIVE_MOTOR_L_TEMP_PIN A9

#define STEER_MOTOR_EN_PIN 7
#define STEER_MOTOR_FWD_PIN 6
#define STEER_MOTOR_REV_PIN 5
#define STEER_MOTOR_TEMP_PIN A10

//RC Transmitter/Reciever Settings
#define CHANNEL_TIMEOUT_FAST 10
#define CHANNEL_TIMEOUT_MED 100
#define CHANNEL_TIMEOUT_SLOW 1000
#define NUM_OF_CHANNELS 6

#define PULSEIN_TIMEOUT 25000
#define CENTER_PULSE_VALUE 1500
#define MAX_PULSE_VALUE 2050
#define MIN_PULSE_VALUE 950

#define STEER_PIN 2
#define THROTTLE_PIN 3
#define ESTOP_PIN 17
#define MODE_PIN 16
#define CH5_PIN 19
#define CH6_PIN 18

#define CENTER_DEADBAND 40 //for throttle/steering counts on either side of the center point
#define DEADBAND_MAX CENTER_PULSE_VALUE + CENTER_DEADBAND
#define DEADBAND_MIN CENTER_PULSE_VALUE - CENTER_DEADBAND

#define ESTOP_THRESHOLD 1500 //above this is active ~1800. ~1200 is inactive
#define MODE_SWITCH_LOW_MID 1350
#define MODE_SWITCH_MID_HIGH 1650

#define MODE_HIGH 1
#define MODE_MED 0
#define MODE_LOW -1

#define MIN_KNOB_VAL 0
#define MAX_KNOB_VAL 1000

#define STEER_IDX 0
#define THROTTLE_IDX 1
#define ESTOP_IDX 2
#define MODE_IDX 3
#define CH5_IDX 4
#define CH6_IDX 5
//End RC Reciever settings

#define PWM_MAX 255

//TIMER settings for motor interrupt thread
#define TIMER_PRESCALE 64
#define TIMER_PRELOAD 65536 - 16000000/TIMER_PRESCALE/(1000/MOTOR_THREAD) // preload timer 65536-16MHz/8/100Hz. 20000 timer counts.
#define TIMER_PRESCALER (1 << CS51) | (1 << CS50)  // 64 prescaler
#if TIMER_PRELOAD >= 65536
  #error Motor thread is too long.
#elif TIMER_PRELOAD < 0
  #error MotorThread is too short.
#endif