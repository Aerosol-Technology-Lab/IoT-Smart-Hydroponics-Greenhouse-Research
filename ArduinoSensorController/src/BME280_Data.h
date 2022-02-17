#pragma once

#include <stdint.h>

struct BME280_Data
{
public:
    unsigned long time = 0;
    
    // temperature in Celcius
    float tempetrature;

    float humidity;
    
    float pressure;
};
