
#include "Remote_Control.h"

Remote_Control::Remote_Control() {
    
}

int16_t Remote_Control::Read(uint8_t index) {
    uint16_t channelVal = _channelsIn.rawChannelValue(index);
    return channelVal == 0 ? CENTER_PULSE_VALUE : channelVal;
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

int16_t Remote_Control::mapModeChannel(int16_t rawValue, int8_t numOfModes) {
    
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

int16_t Remote_Control::GetChannel4() {
    if(Read(CH4_IDX) > MODE_SWITCH_MID_HIGH) {
        return MODE_HIGH;
    } else if(Read(CH4_IDX) < MODE_SWITCH_LOW_MID) {
        return MODE_LOW;
    }
    
    return MODE_MED;
}

int16_t Remote_Control::GetChannel5() {
    return mapKnobChannel(Read(CH5_IDX));
}

int16_t Remote_Control::GetChannel6() {
    return mapKnobChannel(Read(CH6_IDX));
}

int16_t Remote_Control::GetChannel7() {
    return mapKnobChannel(Read(CH7_IDX));
}

Remote Remote_Control::GetRemote() {
    Remote remote;
    remote.throttle = GetThrottle();
    remote.steer = GetSteering();
    remote.estop = GetEStop();
    remote.channel4 = GetChannel4();
    remote.channel5 = GetChannel5();
    remote.channel6 = GetChannel6();
    remote.channel7 = GetChannel7();

    return remote;
}