#ifndef BUTTON_CONTROLLER_h
#define BUTTON_CONTROLLER_h

#include <Arduino.h>
#include "lib/Adafruit_RGB_LCD_Shield_Library/Adafruit_RGBLCDShield.h"
#include "lib/Adafruit_RGB_LCD_Shield_Library/utility/Adafruit_MCP23017.h"

class Button_Controller {
public:
    Button_Controller(uint8_t id);
    bool IsPressed(uint8_t);
    long GetTimePressed();

private:
    uint8_t _id = 0;
    bool _isPressed = false;
    long _timePressed = 0;
    long _startPressed = 0;
};

#endif