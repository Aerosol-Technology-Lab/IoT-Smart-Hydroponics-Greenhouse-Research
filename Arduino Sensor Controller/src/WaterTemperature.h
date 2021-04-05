#pragma once

#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * @brief OneWire Temperature Sensor wrapper
 * 
 */
class WaterTemperature
{
private:

    OneWire oneWire;
    DallasTemperature sensor;
    bool initialized = false;

public:
    WaterTemperature(uint8_t pin, bool initialize=true);

    void init();
    
    float read(uint idx=0);
};
