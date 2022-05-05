/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn

 Todo:
 speed motor Class or motor class?
  - accelerate to speed
  - choose between accelerate and jump to speed
 Car Class
  - start
  - brake
  - drive
  - setspeed
Remote Class
  - channel value mapping
 position Motor class
 eeprom saved settings
settings adjustable remotely
  - Top Speed adjustible
  - stop car button
  - Modes?
  - second knob?
hall effect pedal
position based steering

*/

#include "config.h"
#include "src/includes.h"

UIM_Controller uim ("Powerwheels");

//parameters that define how the vehicle works and that are saved to eeprom
struct Settings {
  int accel = 0;
  int deccel = 0;
};

//parameters associated with driving that are dynamic and constantly changing.
struct DriveSettings {
  
};

Motor driveMotor = Motor(DRIVE_MOTOR_EN_PIN, DRIVE_MOTOR_FWD_PIN, DRIVE_MOTOR_REV_PIN, ACCEL_DEFAULT);
Motor steerMotor = Motor(STEER_MOTOR_EN_PIN, STEER_MOTOR_FWD_PIN, STEER_MOTOR_REV_PIN, FASTEST_ACCEL);

Switch fwdSwitch = Switch(FWD_SWITCH, LOW);
Switch revSwitch = Switch(REV_SWITCH, LOW);
Switch hiLoSwitch = Switch(HILO_SWITCH, LOW);

Remote_Channel ch[NUM_OF_CHANNELS] = {
                            Remote_Channel(STEER_PIN),
                            Remote_Channel(THROTTLE_PIN),
                            Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(CH5_PIN),
                            Remote_Channel(CH6_PIN)
                            };

Remote_Control remote = Remote_Control(ch);

//=========================
long now = 0;
long lastRun = 0;
long calcTime = 0;
//=========================

void setupChannels() {
  ch[STEER_IDX].Startup([]{ch[STEER_IDX].ListenInterrupt();});
  ch[THROTTLE_IDX].Startup([]{ch[THROTTLE_IDX].ListenInterrupt();});
  ch[ESTOP_IDX].Startup([]{ch[ESTOP_IDX].ListenInterrupt();});
  ch[MODE_IDX].Startup([]{ch[MODE_IDX].ListenInterrupt();});
  ch[CH5_IDX].Startup([]{ch[CH5_IDX].ListenInterrupt();});
  ch[CH6_IDX].Startup([]{ch[CH6_IDX].ListenInterrupt();});
}

void setup() {
 
  setupChannels();

  driveMotor.Startup();
  steerMotor.Startup();
	uim.Begin();
  driveMotor.Stop();
  steerMotor.Stop();
  setupTimer();

}

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

    uim.print("TH:");
    uim.print(remote.GetThrottle());
    uim.print(" ST:");
    uim.print(remote.GetSteering());
    uim.print(" E:");
    uim.print(remote.GetEStop());
    uim.print("           ");
    uim.setCursor(0,1);
    uim.print("M:");
    uim.print(remote.GetMode());
    uim.print(" L:" );
    uim.print(remote.GetLKnob());
    uim.print(" R:");
    uim.print(remote.GetRKnob());
    uim.print("            ");
  }

  if(!remote.GetEStop()) {
    driveMotor.setAcceleration(map(remote.GetLKnob(), MIN_KNOB_VAL, MAX_KNOB_VAL, FASTEST_ACCEL, SLOWEST_ACCEL));

    if(remote.GetThrottle() == 0) {
      driveMotor.Stop();
    }
    else {
      driveMotor.Start();
      driveMotor.setSpeed(remote.GetThrottle());
    }

    if(remote.GetSteering() == 0) {
      steerMotor.Stop();
    }
    else {
      steerMotor.Start();
      steerMotor.setSpeed(remote.GetSteering());
    }
  }
  else {
    driveMotor.Stop();
    steerMotor.Stop();
  }
  

  remote.Listen();
  uim.HandleEvents();
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
  driveMotor.Run();
  steerMotor.Run();
  TCNT5 = TIMER_PRELOAD;  // preload timer
}