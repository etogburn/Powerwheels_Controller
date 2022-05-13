#ifndef TEMP_SENSOR_h
#define TEMP_SENSOR_h

#include <Arduino.h>
#include "../config.h"

class Temp_Sensor {
public:
    Temp_Sensor(uint8_t pin);
    uint16_t GetTemp();

private:
    void Initialize();
    void ReadPin();
    uint8_t _pin = 0;
    uint16_t _tempReading = 0;
    uint16_t _rawValue = 0;
    uint8_t _lastMatchedIdx = 0;
    uint16_t temptable[20][2] = { //Analog reading, Deg C
            {  531, 111 },
            {  571, 105 },
            {  611, 100 },
            {  641,  95 },
            {  681,  90 },
            {  711,  85 },
            {  751,  79 },
            {  791,  72 },
            {  811,  69 },
            {  831,  65 },
            {  871,  57 },
            {  881,  55 },
            {  901,  51 },
            {  921,  45 },
            {  941,  39 },
            {  971,  28 },
            {  981,  23 },
            {  991,  17 },
            { 1001,   9 },
            { 1024,   0 }
        };
};

#endif