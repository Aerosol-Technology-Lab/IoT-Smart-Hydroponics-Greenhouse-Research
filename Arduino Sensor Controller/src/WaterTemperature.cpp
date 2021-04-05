#include "WaterTemperature.h"

#include <stdlib.h>
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

size_t WaterTemperature::write(char *buffer, const char *id)
{
    if (!id) {
        id = DEFAULT_ID;
    }
    
    float reading = read();
    sprintf(buffer, "TMP:%d %.2f", id, reading);
    return strlen(buffer) + 1;
}
