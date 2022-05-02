#ifndef REMOTE_CONTROL_h
#define REMOTE_CONTROL_h

#include <Arduino.h>
#include "Remote_Channel.h"


class Remote_Control  {
public:
    Remote_Control();
private:
    Remote_Channel ch[NUM_OF_CHANNELS] = {
                            Remote_Channel(STEER_PIN, CHANNEL_TIMEOUT_FAST),
                            Remote_Channel(THROTTLE_PIN, CHANNEL_TIMEOUT_FAST),
                            Remote_Channel(ESTOP_PIN, CHANNEL_TIMEOUT_MED),
                            Remote_Channel(MODE_PIN, CHANNEL_TIMEOUT_SLOW),
                            Remote_Channel(CH5_PIN, CHANNEL_TIMEOUT_SLOW),
                            Remote_Channel(CH6_PIN, CHANNEL_TIMEOUT_SLOW)
                            };
};


#endif
