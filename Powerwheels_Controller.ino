/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn

*/

#include "config.h"
#include "src/includes.h"

UIM_Controller uim ("Powerwheels");

Car car = Car();

Remote_Channel channels[NUM_OF_CHANNELS] = {
                            Remote_Channel(STEER_PIN),
                            Remote_Channel(THROTTLE_PIN),
                            Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(CH5_PIN),
                            Remote_Channel(CH6_PIN)
                            };

Remote_Control remote = Remote_Control(channels);

void setup() {
  setupChannels();
	uim.Begin();
  setupTimer();
}

// the loop function runs over and over again until power down or reset
void loop() {

 // if(remote.GetMode() == MODE_HIGH) {
    //car.SetSteeringSpeedAdj(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, STEERING_SPEED_ADJUST_MIN, STEERING_SPEED_ADJUST_MAX));
  //} else {
    car.SetAcceleration(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));
  //}
  
  car.SetMaxSpeed(map(remote.GetRKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, 0, PWM_MAX));
  car.SetRemote(remote.GetRemote());

  remote.Listen();
  uim.HandleEvents(car.GetStats());
}

void setupChannels() {
  channels[STEER_IDX].Startup([]{channels[STEER_IDX].ListenInterrupt();});
  channels[THROTTLE_IDX].Startup([]{channels[THROTTLE_IDX].ListenInterrupt();});
  channels[ESTOP_IDX].Startup([]{channels[ESTOP_IDX].ListenInterrupt();});
  channels[MODE_IDX].Startup([]{channels[MODE_IDX].ListenInterrupt();});
  channels[CH5_IDX].Startup([]{channels[CH5_IDX].ListenInterrupt();});
  channels[CH6_IDX].Startup([]{channels[CH6_IDX].ListenInterrupt();});
}

void setupTimer() {

  noInterrupts(); // disable all interrupts
  TCCR5A = 0;
  TCCR5B = 0;

  TCNT5 = TIMER_PRELOAD; 
  TCCR5B |= TIMER_PRESCALER;    
  TIMSK5 |= (1 << TOIE5); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
}

ISR(TIMER5_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  noInterrupts();
  car.Run();
  TCNT5 = TIMER_PRELOAD;  // preload timer
  interrupts();
}