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
  UIM_Controller uim ("Powerwheels");
#endif

Remote_Control remote = Remote_Control();

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    Serial.println("Starting...");
  #endif
  #ifdef USE_UIM
	  uim.Begin();
  #endif
  remote.Setup();
  setupTimer();
}

long now;
long lastRun = 0;


// the loop function runs over and over again until power down or reset
void loop() {
  now = millis();
  #ifdef SERIAL_DEBUG
  
  if(now > lastRun + 500) {
    SerialDebug();
    lastRun = now;
  }
  #endif

//  if(remote.GetChannel4() == MODE_MED) {
    //car.SetSteeringSpeedAdj(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, STEERING_SPEED_ADJUST_MIN, STEERING_SPEED_ADJUST_MAX));
  // } else {
    car.SetAcceleration(map(remote.GetChannel7(), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));
    car.SetMaxSpeed(map(remote.GetChannel5(), MIN_KNOB_VAL, MAX_KNOB_VAL, 0, PWM_MAX));
  // }

  car.SetRemote(remote.GetRemote());

  #if defined(USE_UIM)
    uim.HandleEvents(car.GetStats());
  #endif
  car.Calculate();
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

#ifdef SERIAL_DEBUG
void SerialDebug() {
    Serial.print("Raw Channels: ");
    for(uint8_t i = 0; i < NUM_OF_CHANNELS; i++) {
      Serial.print(remote.Read(i));
      Serial.print(", ");
    }
    Serial.println(" ");
    // Serial.print("Estop = ");
    // Serial.print(car.GetStats().estop);
    // Serial.print(", Mode = ");
    // Serial.print(car.GetStats().mode);
    // Serial.print(", Ped = ");
    // Serial.println(car.GetStats().pedal);
    // Serial.print("Left Motor= ");
    // Serial.print(car.GetStats().motorDriveL.speed);
    // Serial.print(", ");
    // Serial.println(car.GetStats().motorDriveL.temp);
    // Serial.print("Right Motor= ");
    // Serial.print(car.GetStats().motorDriveR.speed);
    // Serial.print(", ");
    // Serial.println(car.GetStats().motorDriveR.temp);
    // Serial.print("Steer Motor= ");
    // Serial.print(car.GetStats().motorDriveR.speed);
    // Serial.print(", ");
    // Serial.println(car.GetStats().motorDriveR.temp);
}
#endif