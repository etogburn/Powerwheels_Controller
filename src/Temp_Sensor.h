#ifndef TEMP_SENSOR_h
#define TEMP_SENSOR_h

#include <Arduino.h>
#include "../config.h"

#define TEMP_TABLE_ROWS 17
#define ANALOG_READ_VAL_IDX 0
#define TEMP_CELSIUS_IDX 1

class Temp_Sensor {
public:
    Temp_Sensor(uint8_t pin);
    uint16_t GetTemp();
    uint16_t ReadPin();

private:
    void Initialize();
    uint16_t GetTempFromTableLocation(uint8_t);
    uint8_t _pin = 0;
    uint16_t _tempReading = 0;
    uint16_t _rawValue = 0;
    long _lastTempCalc = 0;
    const uint16_t _temptable[TEMP_TABLE_ROWS][2] = { //Analog reading, Deg C
            { 1000,   0 },
            {  942,  19 }, 
            {  912,  25 }, //
            {  882,  32 }, 
            {  850,  39 },
            {  820,  44 },
            {  790,  48 }, 
            {  730,  56 }, 
            {  700,  60 },
            {  665,  64 }, 
            {  630,  69 },
            {  595,  73 }, 
            {  540,  79 }, 
            {  485,  85 }, 
            {  443,  90 }, 
            {  415,  95 } 
        };
};

#endif