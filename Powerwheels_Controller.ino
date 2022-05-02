/*
 Name:		Powerwheels_Controller.ino
 Created:	4/27/2022 9:17:35 PM
 Author:	Ethan Ogburn
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

Motor driveMotor = Motor(12, 8, 9);
Motor steerMotor = Motor(13, 10, 11);

Remote_Channel ch[NUM_OF_CHANNELS] = {
                            Remote_Channel(STEER_PIN),
                            Remote_Channel(THROTTLE_PIN),
                            Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_SLOW),
                            Remote_Channel(CH5_PIN),
                            Remote_Channel(CH6_PIN)
                            };

Remote_Control remote = Remote_Control(ch);

#define TIMER_PRESCALE 64
#define TIMER_PRELOAD 65536 - 16000000/TIMER_PRESCALE/(1000/MOTOR_THREAD) // preload timer 65536-16MHz/8/100Hz. 20000 timer counts.
#define TIMER_PRESCALER (1 << CS51) | (1 << CS50)  // 64 prescaler
#if TIMER_PRELOAD >= 65536
  #error Motor thread is too long.
#elif TIMER_PRELOAD < 0
  #error MotorThread is too short.
#endif


void setup() {

  ch[0].Startup([]{ch[0].Listen();});
  ch[1].Startup([]{ch[1].Listen();});
  ch[4].Startup([]{ch[4].Listen();});
  ch[5].Startup([]{ch[5].Listen();});

  driveMotor.Startup();
  steerMotor.Startup();
	uim.Begin();
  driveMotor.Stop();
  steerMotor.Start();
  setupTimer();

}

// the loop function runs over and over again until power down or reset
void loop() {
  
  ch[2].Listen();
  ch[3].Listen();

  uim.home();
  uim.print(ch[0].Read()/10);
  uim.print(" ");
  uim.print(ch[1].Read()/10);
  uim.print(" ");
  uim.print(ch[4].Read()/10);
  uim.print(" ");
  uim.print(ch[5].Read()/10);
  uim.print("              ");
  uim.setCursor(0,1);
  uim.print(driveMotor.getSpeed());
  uim.print(" ");
  uim.print(steerMotor.getSpeed());
  uim.print(" ");
  uim.print(remote.Read(0)/10);
  //uim.print(ch[2].Read()/10);
  uim.print(" ");
  uim.print(remote.Read(1)/10);
  //uim.print(ch[3].Read()/10);
  uim.print("             ");

  int16_t ch2Val = ch[1].Read();
  if(ch2Val >= 1525 || ch2Val <= 1475) {
    driveMotor.Stop();
  }
  else {
    driveMotor.Start();
    driveMotor.setSpeed(constrain(map(ch2Val, 1000, 2000, -255, 255), -255, 255));
  }

  int16_t ch1Val = ch[0].Read();

  if(ch1Val >= 1525 || ch1Val <= 1475) {
    steerMotor.Stop();
  }
  else {
    steerMotor.Start();
    steerMotor.setSpeed(constrain(map(ch1Val, 1000, 2000, -255, 255), -255, 255));
  }

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