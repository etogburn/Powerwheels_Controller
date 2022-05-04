/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn

 Todo:
 speed motor Class or motor class?
  - accelerate to speed
  - choose between accelerate and jump to speed
 Switch Class
  - implement pedal switch and fwd/reverse
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

Motor driveMotor = Motor(DRIVE_MOTOR_EN_PIN, DRIVE_MOTOR_FWD_PIN, DRIVE_MOTOR_REV_PIN);
Motor steerMotor = Motor(STEER_MOTOR_EN_PIN, STEER_MOTOR_FWD_PIN, STEER_MOTOR_REV_PIN);

Remote_Channel ch[NUM_OF_CHANNELS] = {
                            Remote_Channel(STEER_PIN),
                            Remote_Channel(THROTTLE_PIN),
                            Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(CH5_PIN),
                            Remote_Channel(CH6_PIN)
                            };

Remote_Control remote = Remote_Control(ch);


long now = 0;
long lastRun = 0;
long calcTime = 0;
void setup() {
 
  ch[STEER_IDX].Startup([]{ch[STEER_IDX].ListenInterrupt();});
  ch[THROTTLE_IDX].Startup([]{ch[THROTTLE_IDX].ListenInterrupt();});
  ch[CH5_IDX].Startup([]{ch[CH5_IDX].ListenInterrupt();});
  ch[CH6_IDX].Startup([]{ch[CH6_IDX].ListenInterrupt();});

  driveMotor.Startup();
  steerMotor.Startup();
	uim.Begin();
  driveMotor.Stop();
  steerMotor.Stop();
  setupTimer();

}

// the loop function runs over and over again until power down or reset
void loop() {
  
  uim.home();
  uim.print(remote.Read(0));
  uim.print(" ");
  uim.print(remote.Read(1));
  uim.print(" ");
  uim.print(" ");
  //uim.print(remote.Read(2));
  //uim.print(ch[4].Read()/10);
  uim.print(" ");
  //uim.print(ch[5].Read()/10);
  uim.print("              ");
  uim.setCursor(0,1);
  
  uim.print(steerMotor.getSpeed());
  // uim.print(remote.Read(3));
  uim.print(" ");
  uim.print(calcTime);
  uim.print(" ");
  //uim.print(remote.Read(4));
  //uim.print(ch[2].Read()/10);
  uim.print(" ");
  //uim.print(remote.Read(5));
  //uim.print(ch[3].Read()/10);
  uim.print("             ");


  if(remote.Read(THROTTLE_IDX) <= 5 && remote.Read(THROTTLE_IDX) >= -5) {
    driveMotor.Stop();
  }
  else {
    driveMotor.Start();
    driveMotor.setSpeed(remote.Read(THROTTLE_IDX));
  }


  if(remote.Read(STEER_IDX) <= 5 && remote.Read(STEER_IDX) >= -5) {
    steerMotor.Stop();
  }
  else {
    steerMotor.Start();
    steerMotor.setSpeed(remote.Read(STEER_IDX));
  }

  remote.Listen();
  uim.HandleEvents();
}

void interruptTasks() {
  
  driveMotor.Run();
  steerMotor.Run();
  
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
  interruptTasks();
  TCNT5 = TIMER_PRELOAD;  // preload timer
}