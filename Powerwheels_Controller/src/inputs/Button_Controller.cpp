#include "Button_Controller.h"

Button_Controller::Button_Controller(Adafruit_RGBLCDShield* _lcd, uint8_t _id ) {
    lcd_ = _lcd;
    id_ = _id;
};

bool Button_Controller::IsPressed() {

    uint8_t _buttons = lcd_->readButtons();
    if(_buttons & id_) return true;
    else return false;

};

long Button_Controller::GetTimePressed() {
    return timePressed_;
};