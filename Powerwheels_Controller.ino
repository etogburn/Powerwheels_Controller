/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn

Todo:
position Motor class
eeprom saved settings
settings adjustable remotely
  - Modes?
  - second knob?
hall effect pedal
position based steering

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

long now = 0;
long lastRun = 0;

// the loop function runs over and over again until power down or reset
void loop() {
  now = millis();

  if(now > lastRun + TIME_LCD_UPDATE) {
    uim.home();
    // uim.print("FWD:");
    // uim.print(fwdSwitch.IsActive());
    // uim.print(" ");
    // uim.print("REV:");
    // uim.print(revSwitch.IsActive());
    // uim.print(" ");
    // uim.setCursor(0,1);
    // uim.print("HILO:");
    // uim.print(hiLoSwitch.IsActive());
    // uim.print(" ");

    //uim.print("8: ");
    // uim.print(temp1.ReadPin());
    // uim.print(remote.GetThrottle());
    // uim.print(" 9: ");
    // uim.print(temp2.ReadPin());
    // uim.print(remote.GetThrottle());
    //uim.print(" E:");
    //uim.print(remote.GetEStop());
    car.IsOverTemp() ? uim.print("Disabled") : uim.print("Enabled");
    uim.print("           ");
    uim.setCursor(0,1);
    uim.print("St Temp: ");
    uim.print(car.GetTemp());
    // uim.print(remote.GetMode());
    // uim.print(" L:" );
    // uim.print(remote.GetLKnob());
    // uim.print(" R:");
    // uim.print(remote.GetRKnob());
    uim.print("            ");
  }

  car.SetEStop(remote.GetEStop());
  car.SetMode(remote.GetMode());
  
  car.SetAcceleration(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));
  car.SetMaxSpeed(map(remote.GetRKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, 0, PWM_MAX));

  car.SetThrottle(remote.GetThrottle());

  car.SetSteer(remote.GetSteering());

  remote.Listen();
  uim.HandleEvents();
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