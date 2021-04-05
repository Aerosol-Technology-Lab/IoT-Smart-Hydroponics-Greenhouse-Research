#pragma once

#include <stdlib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/**
 * @brief OneWire Temperature Sensor wrapper
 * 
 */
class WaterTemperature
{
public:
    const char const *DEFAULT_ID = "_";

private:

    OneWire oneWire;
    DallasTemperature sensor;
    bool initialized = false;

public:
    WaterTemperature(uint8_t pin, bool initialize=true);

    void init();
    
    float read(uint8_t idx=0);

    size_t write(char *buffer, const char *id=nullptr);
};
