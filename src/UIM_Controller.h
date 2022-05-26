#ifndef UIM_CONTROLLER_h
#define UIM_CONTROLLER_h

#include <Arduino.h>
#include <Wire.h>
#include "lib/Adafruit_RGB_LCD_Shield_Library/Adafruit_RGBLCDShield.h"
#include "lib/Adafruit_RGB_LCD_Shield_Library/utility/Adafruit_MCP23017.h"
#include "Button_Controller.h"
#include "../config.h"
#include "../CarStats.h"

//LCD Colors
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define BTN_NUMBER 5
#define BTN_UP 0
#define BTN_DOWN 1
#define BTN_LEFT 2
#define BTN_RIGHT 3
#define BTN_SELECT 4

#define NUM_OF_SCREENS 2 //1 based index. does not include the welcome screen.

class UIM_Controller : public Adafruit_RGBLCDShield {
public:
  UIM_Controller(String _startMsg = "");

  // @brief setups and starts the display with the welcome message.
  void Begin();

  // @brief Screen 1 - FWD, REV, HILO
  void HandleEvents(CarStats car);
  
private:
  String _startMessage = "";

  long _lastScreenChange = 0;
  long _lastLCDUpdate = 0;
  long _lastButtonRead = 0;

  uint8_t _screenCount = 0;
  bool _scrollScreens = true;
  bool _bootScreen = true;

  CarStats _car;

  void SetScreen();
  void SetScreenWelcome();
  void SetScreenInCar();
  void SetScreenRemote();

  void ReadButtons();

  void ButtonPressed(uint8_t);
  
  bool IsPressed(uint8_t, uint8_t);
  
  Button_Controller Btn[BTN_NUMBER] = {
                                        Button_Controller(BUTTON_UP), 
                                        Button_Controller(BUTTON_DOWN), 
                                        Button_Controller(BUTTON_LEFT), 
                                        Button_Controller(BUTTON_RIGHT), 
                                        Button_Controller(BUTTON_SELECT)
                                      };
};

#endif