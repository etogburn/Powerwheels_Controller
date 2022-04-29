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

Motor driveMotor = Motor(30, 31, 12);
// the setup function runs once when you press reset or power the board
void setup() {

	setupTimer();

	uim.Begin();

}

// the loop function runs over and over again until power down or reset
void loop() {


  
  uim.HandleEvents();
}

void interruptTasks() {
  driveMotor.Run();
}



void setupTimer() {

#define TIMER_PRELOAD 63536
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