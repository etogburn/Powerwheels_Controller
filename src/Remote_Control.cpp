
#include "Remote_Control.h"

Remote_Control::Remote_Control() {
    pinMode(REMOTE_FEEDBACK_PIN, OUTPUT);
    SetFeedbackVal(0);
}

void Remote_Control::SetFeedbackVal(int16_t val) {
    if(val >= MIN_FEEDBACK_VOLTAGE && val <= MAX_FEEDBACK_VOLTAGE) {
        analogWrite(REMOTE_FEEDBACK_PIN, (val*255+MAX_FEEDBACK_VOLTAGE/2)/MAX_FEEDBACK_VOLTAGE);
    }
}

bool Remote_Control::IsAvailable() {
    return true;
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

int16_t Remote_Control::GetThrottle() {
    return mapControlChannel(Read(THROTTLE_IDX));
}

int16_t Remote_Control::GetSteering() {
    return mapControlChannel(Read(STEER_IDX));
}

bool Remote_Control::GetEStop() {
    static bool estopActive;
    static unsigned long lastTimeActive;
    static bool readyForReset;

    unsigned long now = millis();
    bool estopVal = Read(ESTOP_IDX) > ESTOP_THRESHOLD ? true : false;

    if(now-lastTimeActive > ESTOP_RESET_TIME) {
        if(!estopVal && estopActive) {
            readyForReset = true;
        } else if(estopVal && estopActive && readyForReset) {
            estopActive = false;
        } else if(estopVal && !estopActive) {
            estopActive = true;
        }
    }
    
    if(estopVal) {
        lastTimeActive = now;
        readyForReset = false;
    }

    return estopActive;
}

int16_t Remote_Control::GetChannel(uint8_t channel) {
    return channel <= NUM_OF_CHANNELS ? mapKnobChannel(Read(channel)) : -1;
}

Remote Remote_Control::GetRemote() {
    Remote remote;
    remote.throttle = GetThrottle();
    remote.steer = GetSteering();
    remote.estop = GetEStop();
    remote.channel4 = GetChannel(CH4_IDX);
    remote.channel5 = GetChannel(CH5_IDX);
    remote.channel6 = GetChannel(CH6_IDX);
    remote.channel7 = GetChannel(CH7_IDX);

    return remote;
}