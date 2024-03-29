#include "Temp_Sensor.h"

Temp_Sensor::Temp_Sensor(uint8_t pin) {
    _pin = pin;
    Initialize();   
}

void Temp_Sensor::Initialize() {
    pinMode(_pin, INPUT);
}

uint16_t Temp_Sensor::ReadPin() {
    _rawValue = analogRead(_pin);
    return _rawValue;
}

uint16_t Temp_Sensor::GetTemp() {
    long now = millis();
    
    if(now > _lastTempCalc + TEMP_CALC_FREQ) {
        ReadPin();
        if(_rawValue >= _temptable[0][ANALOG_READ_VAL_IDX]) {
        _tempReading = _temptable[0][TEMP_CELSIUS_IDX];
        } else if(_rawValue <= _temptable[TEMP_TABLE_ROWS - 1][ANALOG_READ_VAL_IDX]) {
            _tempReading = _temptable[TEMP_TABLE_ROWS - 1][TEMP_CELSIUS_IDX];
        } else {
            for(int8_t i = 1; i < TEMP_TABLE_ROWS; i++) {
                uint16_t tableVal = _temptable[i][ANALOG_READ_VAL_IDX];
                if(_rawValue == tableVal) {
                    _tempReading = _temptable[i][TEMP_CELSIUS_IDX];
                    break;
                } else if(_rawValue > tableVal) {
                    _tempReading = GetTempFromTableLocation(i);
                    break;
                }
            }
        }
    }

    return _tempReading;
}

uint16_t Temp_Sensor::GetTempFromTableLocation(uint8_t tableIndex) {
    uint16_t returnVal = 0;
    
    if(tableIndex != 0) {
        //Matched values in the temp table array and the last values to interpolate between
        int16_t lastIdxAnalog = _temptable[tableIndex - 1][ANALOG_READ_VAL_IDX];
        int16_t lastIdxTemp = _temptable[tableIndex - 1][TEMP_CELSIUS_IDX];
        int16_t thisIdxAnalog = _temptable[tableIndex][ANALOG_READ_VAL_IDX];
        int16_t thisIdxTemp = _temptable[tableIndex][TEMP_CELSIUS_IDX];

        //These values are inverted so they are always positive.
        int16_t analogReadDifference = lastIdxAnalog - thisIdxAnalog;
        int16_t tempDifference = thisIdxTemp - lastIdxTemp;

        //linear interpolation calculation to find the point between locations in the array.
        returnVal = ((_rawValue - thisIdxAnalog) * tempDifference + analogReadDifference/2);
        returnVal = thisIdxTemp - returnVal / analogReadDifference;
    } 
    
    return returnVal; //degrees C
}

// uint16_t Temp_Sensor::GetTempTable(uint8_t firstIdx, uint8_t secondIdx) {
//     const uint16_t tempTable[TEMP_TABLE_ROWS][2] PROGMEM = { //Analog reading, Deg C
//             { 1000,   0 },
//             {  942,  19 }, 
//             {  912,  25 }, 
//             {  882,  32 }, 
//             {  850,  39 },
//             {  820,  44 },
//             {  790,  48 }, 
//             {  730,  56 }, 
//             {  700,  60 },
//             {  665,  64 }, 
//             {  630,  69 },
//             {  595,  73 }, 
//             {  540,  79 }, 
//             {  485,  85 }, 
//             {  443,  90 }, 
//             {  415,  95 } 
//         };

//     return (uint16_t)*tempTable[firstIdx, secondIdx];
// }