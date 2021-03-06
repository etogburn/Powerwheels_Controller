
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
    
void Remote_Control::Listen() {
    for(uint8_t i = 0; i < NUM_OF_CHANNELS; i++) {
        ch[i]->Listen();
    }
    // ch[_channelToListen]->Listen();
    // _channelToListen >= NUM_OF_CHANNELS - 1 ? _channelToListen = 0 : _channelToListen++;
}

int16_t Remote_Control::mapControlChannel(int16_t rawValue) {
    if (rawValue < DEADBAND_MIN) {
        return constrain(map(rawValue, MIN_PULSE_VALUE, DEADBAND_MIN, -1* PWM_MAX, -1), -1* PWM_MAX, -1);
    } else if(rawValue > DEADBAND_MAX) {
        return constrain(map(rawValue, DEADBAND_MAX, MAX_PULSE_VALUE, 1,  PWM_MAX), 1, PWM_MAX);
    } 

    return 0;
}

int16_t Remote_Control::mapKnobChannel(int16_t rawValue) {
    int16_t output = map(rawValue, MIN_PULSE_VALUE, MAX_PULSE_VALUE, MIN_KNOB_VAL, MAX_KNOB_VAL);
     return constrain(output, MIN_KNOB_VAL, MAX_KNOB_VAL);
}

int16_t Remote_Control::GetThrottle() {
    return mapControlChannel(Read(THROTTLE_IDX));
}

int16_t Remote_Control::GetSteering() {
    return mapControlChannel(Read(STEER_IDX));
}

bool Remote_Control::GetEStop() {
    return Read(ESTOP_IDX) > ESTOP_THRESHOLD ? true : false;
}

int8_t Remote_Control::GetMode() {
    if(Read(MODE_IDX) > MODE_SWITCH_MID_HIGH) {
        return MODE_HIGH;
    } else if(Read(MODE_IDX) < MODE_SWITCH_LOW_MID) {
        return MODE_LOW;
    }
    
    return MODE_MED;
}

int16_t Remote_Control::GetLKnob() {
    return mapKnobChannel(Read(CH5_IDX));
}

int16_t Remote_Control::GetRKnob() {
    return mapKnobChannel(Read(CH6_IDX));
}