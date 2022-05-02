
#include "Remote_Control.h"

Remote_Control::Remote_Control(Remote_Channel channels[NUM_OF_CHANNELS]) {
    for(uint8_t i = 0; i < NUM_OF_CHANNELS; i++) {
        ch[i] = &channels[i];
    }
}

void Remote_Control::Setup() {
    
}

int16_t Remote_Control::Read(uint8_t index) {
    return ch[index]->Read();
}