#ifndef UIM_CONTROLLER_h
#define UIM_CONTROLLER_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>


class UIM_Controller {
public:
  UIM_Controller(String _startMsg = "");

  // @brief setups and starts the display with the welcome message.
  void Begin();
  void Display();
  
private:
  Adafruit_RGBLCDShield lcd_;
  String startMessage_ = "";
};

#endif