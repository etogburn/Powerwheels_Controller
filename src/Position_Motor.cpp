#include "Position_Motor.h"

Position_Motor::Position_Motor(uint8_t enablePin, uint8_t fwdPin, uint8_t backPin, uint8_t tempPin, uint8_t feedbackPin) : Motor(enablePin, fwdPin, backPin, tempPin) {
    setAcceleration(FASTEST_ACCEL);
}

void Position_Motor::Run() {
    Motor::Run();
}

void Position_Motor::Calculate() {
    Motor::Calculate();
}

void Position_Motor::SetPosition(int16_t position) {
    setSpeed(position);
}
