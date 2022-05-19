#ifndef TEMP_SENSOR_h
#define TEMP_SENSOR_h

#include <Arduino.h>
#include "../config.h"

class Temp_Sensor {
public:
    Temp_Sensor(uint8_t pin);
    uint16_t GetTemp();
    uint16_t ReadPin();

private:
    void Initialize();
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
            {  595,  73 }, //
            {  811,  69 },
            {  665,  64 }, //
            {  871,  57 },
            {  730,  56 }, //
            {  790,  48 }, //
            {  921,  45 },
            {  941,  39 },
            {  882,  32 }, //
            {  981,  23 },
            {  938,  20 }, //
            { 1001,   9 },
            { 1024,   0 }
        };
};

#endif