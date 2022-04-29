#ifndef BUTTON_CONTROLLER_h
#define BUTTON_CONTROLLER_h

#include <Arduino.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

class Button_Controller {
public:
    Button_Controller(uint8_t id);
    bool IsPressed(uint8_t);
    long GetTimePressed();

private:
    uint8_t id_ = 0;
    bool isPressed_ = false;
    long timePressed_ = 0;
    long startPressed_ = 0;

};


#endif