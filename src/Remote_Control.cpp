
#include "Remote_Control.h"

Remote_Control::Remote_Control(Remote_Channel channels[NUM_OF_CHANNELS]) {
    for(uint8_t i = 0; i < NUM_OF_CHANNELS; i++) {
        ch[i] = &channels[i];
    }
}

void Remote_Control::Setup() {
    
}

int16_t Remote_Control::Read(uint8_t index) {
    int16_t output = map(ch[index]->Read(), MIN_PULSE_VALUE, MAX_PULSE_VALUE, -1 * PWM_MAX, PWM_MAX);
    return constrain(output, -1* PWM_MAX, PWM_MAX);
}
    
void Remote_Control::Listen() {
    for(uint8_t i = 0; i < NUM_OF_CHANNELS; i++) {
        ch[i]->Listen();
    }
}