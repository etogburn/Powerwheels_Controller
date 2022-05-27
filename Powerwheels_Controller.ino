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

#ifdef IBUS_RECIEVER
  Remote_Control remote = Remote_Control(IBUS_INPUT);
#else
  Remote_Channel channels[NUM_OF_CHANNELS] = {
                              Remote_Channel(STEER_PIN),
                              Remote_Channel(THROTTLE_PIN),
                              Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                              Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_MED),
                              Remote_Channel(CH5_PIN),
                              Remote_Channel(CH6_PIN)
                              };

  Remote_Control remote = Remote_Control(channels);
#endif

void setup() {
  #ifdef SERIAL_DEBUG
    Serial.begin(115200);
    Serial.println("Starting...");
  #endif
  #ifdef USE_UIM
	  uim.Begin();
  #endif
  #ifndef IBUS_RECIEVER
    setupChannels();
  #endif
  setupTimer();
}

#if defined(SERIAL_DEBUG) || (defined(MEGA_BOARD) && defined(USE_UIM))
long now;
long lastRun = 0;
#endif

// the loop function runs over and over again until power down or reset
void loop() {
  #ifdef SERIAL_DEBUG
  now = millis();
  if(now > lastRun + 500) {
    SerialDebug();
    lastRun = now;
  }
  #endif

  #if defined(MEGA_BOARD) && defined(USE_UIM)
    if(now > lastRun + TIME_LCD_UPDATE) {
      uim.home();
      car.IsOverTemp() ? uim.print("Motor Over Temp") : uim.print("Running...");
      uim.print("     ");
      uim.setCursor(0,1);
      uim.print(car.GetLTemp());
      uim.print(" C");
      uim.setCursor(0,8);
      uim.print(car.GetRTemp());
      uim.print(" C");
    }
  #endif

 // if(remote.GetMode() == MODE_HIGH) {
    //car.SetSteeringSpeedAdj(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, STEERING_SPEED_ADJUST_MIN, STEERING_SPEED_ADJUST_MAX));
  //} else {
    car.SetAcceleration(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));
  //}
  
  car.SetMaxSpeed(map(remote.GetRKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, 0, PWM_MAX));
  // car.SetThrottle(remote.GetThrottle());
  // car.SetSteer(remote.GetSteering());
  // car.SetEStop(remote.GetEStop());
  // car.SetMode(remote.GetMode());
  car.SetRemote(remote.GetRemote());


  #if defined(USE_UIM) && defined(DUE_BOARD)
    uim.HandleEvents(car.GetStats());
  #endif

  remote.Listen();

}
#ifndef IBUS_RECIEVER
  void setupChannels() {
    channels[STEER_IDX].Startup([]{channels[STEER_IDX].ListenInterrupt();});
    channels[THROTTLE_IDX].Startup([]{channels[THROTTLE_IDX].ListenInterrupt();});
    channels[ESTOP_IDX].Startup([]{channels[ESTOP_IDX].ListenInterrupt();});
    channels[MODE_IDX].Startup([]{channels[MODE_IDX].ListenInterrupt();});
    channels[CH5_IDX].Startup([]{channels[CH5_IDX].ListenInterrupt();});
    channels[CH6_IDX].Startup([]{channels[CH6_IDX].ListenInterrupt();});
  }
#endif

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
  Timer5.attachInterrupt(InterruptFunctions);
	Timer5.start(MOTOR_THREAD*1000); //*1000 converts to microseconds
#endif
  interrupts(); // enable all interrupts
}

void InterruptFunctions() {
  car.Run();
}

#ifdef MEGA_BOARD
ISR(TIMER5_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  //noInterrupts();
  InterruptFunctions();
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
    Serial.print("Estop = ");
    Serial.print(car.GetStats().estop);
    Serial.print(", Mode = ");
    Serial.print(car.GetStats().mode);
    Serial.print(", Ped = ");
    Serial.println(car.GetStats().pedal);
    Serial.print("Left Motor= ");
    Serial.print(car.GetStats().motorDriveL.speed);
    Serial.print(", ");
    Serial.println(car.GetStats().motorDriveL.temp);
    Serial.print("Right Motor= ");
    Serial.print(car.GetStats().motorDriveR.speed);
    Serial.print(", ");
    Serial.println(car.GetStats().motorDriveR.temp);
    Serial.print("Steer Motor= ");
    Serial.print(car.GetStats().motorDriveR.speed);
    Serial.print(", ");
    Serial.println(car.GetStats().motorDriveR.temp);
}
#endif