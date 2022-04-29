#include "Button_Controller.h"

Button_Controller::Button_Controller(uint8_t _id, Adafruit_RGBLCDShield _lcd) {
    lcd_ = _lcd;
    id_ = _id;
};

bool Button_Controller::IsPressed() {

    uint8_t _buttons = lcd_.readButtons();

    if(_buttons & id_) {
        if(!isPressed_) {
            startPressed_ = millis();
        }
        isPressed_ = true;
    }
    else  {
        startPressed_ = -1;
        isPressed_ = false;
    }
        

    return isPressed_;

};

long Button_Controller::GetTimePressed() {

    if(isPressed_) timePressed_ = millis() - startPressed_;
    else timePressed_ = 0;

    return timePressed_;
};