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

Motor driveMotor = Motor(8, 9);
Motor steerMotor = Motor(10, 11);


Remote_Channel ch1(2, 10);
Remote_Channel ch2(3, 10);
Remote_Channel ch3(4, 200);
Remote_Channel ch4(5, 1000);
Remote_Channel ch5(6, 1000);
Remote_Channel ch6(7, 1000);



void setup() {
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
	digitalWrite(12,HIGH);
  digitalWrite(13,HIGH);
  
  driveMotor.Startup();
  steerMotor.Startup();
	uim.Begin();
  driveMotor.Stop();
  steerMotor.Start();
  setupTimer();
  steerMotor.setAccelDeccel(100);
  driveMotor.setAccelDeccel(100);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
  ch1.Listen();
  ch2.Listen();
  ch3.Listen();
  ch4.Listen();
  ch5.Listen();
  ch6.Listen();

  uim.home();
  uim.print(ch1.Read());
  uim.print(" ");
  uim.print(ch2.Read());
  uim.print(" ");
  //uim.print(ch3.Read());
  uim.print("              ");
  uim.setCursor(0,1);
  uim.print(driveMotor.getSpeed());
  uim.print(" ");
  uim.print(steerMotor.getSpeed());
  uim.print(" ");
  //uim.print(ch6.Read());
  uim.print("             ");

  int16_t ch2Val = ch2.Read();
  if(ch2Val > 1500) {
    driveMotor.setDirection(true);
    driveMotor.setSpeed(constrain(map(ch2Val, 1500, 2000, 0, 255), 0, 255));
  }
  else {
    driveMotor.setDirection(false);
    driveMotor.setSpeed(constrain(map(ch2Val, 1500, 1000, 0, 255), 0, 255));
  }

  int16_t ch1Val = ch1.Read();
  if(ch1Val > 1500) {
    steerMotor.setDirection(true);
    steerMotor.setSpeed(constrain(map(ch1Val, 1500, 2000, 0, 255), 0, 255));
  }
  else {
    steerMotor.setDirection(false);
    steerMotor.setSpeed(constrain(map(ch1Val, 1500, 1000, 0, 255), 0, 255));
  }


  uim.HandleEvents();
  
}

void interruptTasks() {
  
  driveMotor.Run();
  steerMotor.Run();
  
}


void setupTimer() {

#define TIMER_PRELOAD 65536 - 16000000/8/(1000/MOTOR_THREAD) // preload timer 65536-16MHz/8/100Hz. 20000 timer counts.
#define TIMER_PRESCALER (1 << CS51)

  noInterrupts(); // disable all interrupts
  TCCR5A = 0;
  TCCR5B = 0;

  TCNT5 = TIMER_PRELOAD; // preload timer 65536-16MHz/8/1000Hz. 2000 timer counts.
  TCCR5B |= TIMER_PRESCALER;     // 8 prescaler 
  TIMSK5 |= (1 << TOIE5); // enable timer overflow interrupt
  interrupts(); // enable all interrupts
}

ISR(TIMER5_OVF_vect) // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  interruptTasks();
  TCNT5 = TIMER_PRELOAD;  // preload timer
}