#include "Button_Controller.h"

Button_Controller::Button_Controller(uint8_t id) {
    _id = id;
};

bool Button_Controller::IsPressed(uint8_t _buttons) {
    if(_buttons & _id) {
        if(!_isPressed) {
            _startPressed = millis();
        }
        _isPressed = true;
    } else  {
        _startPressed = -1;
        _isPressed = false;
    } 
    return _isPressed;
};

long Button_Controller::GetTimePressed() {
    _isPressed ? _timePressed = millis() - _startPressed : _timePressed = 0;

    return _timePressed;
};