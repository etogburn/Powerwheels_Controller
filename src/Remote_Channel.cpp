#include "Remote_Channel.h"

Remote_Channel::Remote_Channel(uint8_t recievePin, long timeBetweenReads) {
    _recievePin = recievePin;
    _timeBetweenReads = timeBetweenReads;

    //Startup();
}

int16_t Remote_Channel::Read() {
    uint16_t output = 0;
    for(uint8_t i = 0; i < NUM_TO_AVG; i++) {
        output += _rawValue[i];
    }
    return output/NUM_TO_AVG;
}

void Remote_Channel::Startup(void (*ISR_callback)(void)) {
    pinMode(_recievePin, INPUT);
    #ifdef SERIAL_DEBUG 
    Serial.print(_recievePin);
    #endif

    if(digitalPinToInterrupt(_recievePin) == NOT_AN_INTERRUPT) {
        _useInterrupt = false;
        #ifdef SERIAL_DEBUG 
        Serial.println(" doesn't use interrupt");
        #endif
    } else {
        #ifdef SERIAL_DEBUG 
        Serial.println(" uses interrupt");
        #endif
        _useInterrupt = true;
        noInterrupts();
        #ifdef MEGA_BOARD
            attachInterrupt(digitalPinToInterrupt(_recievePin), ISR_callback, CHANGE);
        #endif
        #ifdef DUE_BOARD
            attachInterrupt(_recievePin, ISR_callback, CHANGE);
        #endif
        interrupts();
    }
}

void Remote_Channel::Listen() {
    if(!_useInterrupt) {
        CalcPulseWidthPulseIn();
        #ifdef SERIAL_DEBUG 
        Serial.print("Regular Listen on ");
        Serial.println(_recievePin);
        #endif
    }
}

void Remote_Channel::ListenInterrupt() {
    if(_useInterrupt) {
        CalcPulseWidthInterrupt();
    }
}
void Remote_Channel::CalcPulseWidthPulseIn() {
    long now = millis();

    if(now > _lastTimeRead + _timeBetweenReads) {
        _rawValue[_idx] = pulseIn(_recievePin, HIGH, PULSEIN_TIMEOUT);
        IncrementIDX();
        _lastTimeRead = now;
    }
}

void Remote_Channel::CalcPulseWidthInterrupt() {
    if(digitalRead(_recievePin) == HIGH){
        _interrupt_timer_start = micros();
    }
    else if(_interrupt_timer_start > 0){
        _rawValue[_idx] = ((volatile int)micros() - _interrupt_timer_start);
        IncrementIDX();
        _interrupt_timer_start = 0;
    }
}

void Remote_Channel::IncrementIDX() {
    _idx++;
    _idx == NUM_TO_AVG ? _idx = 0 : 0;
}
