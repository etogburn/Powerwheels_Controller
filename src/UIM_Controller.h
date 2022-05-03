#ifndef UIM_CONTROLLER_h
#define UIM_CONTROLLER_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "Button_Controller.h"
#include "../config.h"

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

#define TIME_LCD_UPDATE 20 //ms between display updates
#define TIME_BUTTON_READ 10 //ms between button reads

class UIM_Controller : public Adafruit_RGBLCDShield {
public:
  UIM_Controller(String _startMsg = "");

  // @brief setups and starts the display with the welcome message.
  void Begin();

  // @brief Updates the display in the main loop.
  void HandleEvents();
  
private:

  String startMessage_ = "";

  long lastLCDUpdate_ = 0;
  long lastButtonRead_ = 0;

  long val = 0;

  void UpdateLCD();
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