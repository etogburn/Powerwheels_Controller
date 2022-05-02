//Configuration values all go here

#define MOTOR_THREAD 50 //ms -- 16000000/64/65536 = 3.82 hz or ~250ms max. 250,000 hz or 4us min.


//RC Transmitter/Reciever Settings

#define CHANNEL_TIMEOUT_FAST 10
#define CHANNEL_TIMEOUT_MED 200
#define CHANNEL_TIMEOUT_SLOW 1000
#define NUM_OF_CHANNELS 6

#define STEER_PIN 2
#define THROTTLE_PIN 3
#define ESTOP_PIN 4
#define MODE_PIN 5
#define CH5_PIN 18
#define CH6_PIN 19

#define STEER_IDX 0
#define THROTTLE_IDX 1
#define ESTOP_IDX 2
#define MODE_IDX 3
#define CH5_IDX 4
#define CH6_IDX 5

#define MAX_PULSE_VALUE 2000
#define MIN_PULSE_VALUE 1000

//End RC Reciever settings

