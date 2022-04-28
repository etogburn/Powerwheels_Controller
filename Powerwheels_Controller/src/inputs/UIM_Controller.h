#ifndef UIM_CONTROLLER_h
#define UIM_CONTROLLER_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include "Button_Controller.h"

//LCD Colors
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

#define TIME_LCD_UPDATE 20 //ms between display updates
#define TIME_BUTTON_READ 10 //ms between button reads

class UIM_Controller {
public:
  UIM_Controller(String _startMsg = "");

  // @brief setups and starts the display with the welcome message.
  void Begin();

  // @brief Updates the display in the main loop.
  void HandleEvents();
  
private:
  Adafruit_RGBLCDShield* lcd_;
  String startMessage_ = "";

  long lastLCDUpdate_ = 0;
  long lastButtonRead_ = 0;

  void UpdateLCD();
  void ReadButtons();
  
  Button_Controller upBtn = Button_Controller(lcd_,BUTTON_UP);
  Button_Controller downBtn = Button_Controller(lcd_,BUTTON_DOWN);
  Button_Controller leftBtn = Button_Controller(lcd_,BUTTON_LEFT);
  Button_Controller rightBtn = Button_Controller(lcd_,BUTTON_RIGHT);
  Button_Controller selectBtn = Button_Controller(lcd_,BUTTON_SELECT);

  void ButtonUpPressed();
  void ButtonDownPressed();
  void ButtonLeftPressed();
  void ButtonRightPressed();
  void ButtonSelectPressed();
};

#endif