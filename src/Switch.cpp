#include "Switch.h"

Switch::Switch(int inputPin, bool activeState) {
    _inputPin = inputPin;
    _activeState = activeState;

    Initialize();
}

void Switch::Initialize() {
    pinMode(_inputPin, INPUT_PULLUP);
}

bool Switch::IsActive() {
    return digitalRead(_inputPin) == _activeState ? true : false;
}