#include "WaterTemperature.h"

#include <OneWire.h>
#include <DallasTemperature.h>

WaterTemperature::WaterTemperature(uint8_t pin, bool initialize)
    : oneWire(pin), sensor(&oneWire)
{
    if (initialize) {
        init();
    }
}

void WaterTemperature::init()
{
    sensor.begin();
    initialized = true;
}

float WaterTemperature::read(uint8_t idx)
{
    sensor.requestTemperatures();
    return sensor.getTempFByIndex(idx);
}
