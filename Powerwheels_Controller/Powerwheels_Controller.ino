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

// the setup function runs once when you press reset or power the board
void setup() {
	// set up the LCD's number of columns and rows: 
	uim.Begin();

}

// the loop function runs over and over again until power down or reset
void loop() {

  uim.Display();
}
