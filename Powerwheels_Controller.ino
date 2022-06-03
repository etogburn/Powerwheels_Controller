/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn
*/

#include "config.h"
#include "src/includes.h"

#ifdef DUE_BOARD
  #include <DueTimer.h>
#endif

Car car = Car();

#ifdef USE_UIM
  UIM_Controller uim = UIM_Controller();
#endif

Remote_Control remote = Remote_Control();

void setup() {
  #ifdef USE_UIM
	  uim.Begin();
  #endif
  setupTimer();
}

// the loop function runs over and over again until power down or reset
void loop() {
  car.SetAcceleration(map(remote.GetChannel(CH7_IDX), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));
  car.SetMaxSpeed(map(remote.GetChannel(CH5_IDX), MIN_KNOB_VAL, MAX_KNOB_VAL, 0, PWM_MAX));
  remote.SetFeedbackVal(car.GetTemp()/2);
  car.Calculate();
  #if defined(USE_UIM)
    uim.HandleEvents(car.GetStats());
  #endif
}

void setupTimer() {
  noInterrupts(); // disable all interrupts
#ifdef MEGA_BOARD
  TCCR5A = 0;
  TCCR5B = 0;

  TCNT5 = TIMER_PRELOAD; 
  TCCR5B |= TIMER_PRESCALER;    
  TIMSK5 |= (1 << TOIE5); // enable timer overflow interrupt
#endif

#ifdef DUE_BOARD
  Timer5.attachInterrupt(Threads);
	Timer5.start(MOTOR_THREAD*1000); //*1000 converts to microseconds
#endif
  interrupts(); // enable all interrupts
}

void Threads() {
  car.SetRemote(remote.GetRemote());
  car.Run();
}

#ifdef MEGA_BOARD
ISR(TIMER5_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //noInterrupts();
  Threads();
  TCNT5 = TIMER_PRELOAD;  // preload timer
  //interrupts();
}
#endif