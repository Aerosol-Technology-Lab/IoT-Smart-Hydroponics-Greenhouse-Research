#pragma once

#include "SensorInterface.h"
#include "WaterTemperature.h"
#include "utils.h"

#define RES2  820.0f
#define ECREF 200.0f

class EC : public SensorInterface
{
private:
    uint8_t pin;
    WaterTemperature *waterTemperature;
    
    float kValueLow = 1.0f;
    float kValueHigh = 1.0f;
    
public:
    EC(uint8_t pin, WaterTemperature *WaterTemperature=nullptr);

    void init();

    void setWaterTemperatureSensor(WaterTemperature *sensor);

    float read(uint8_t _=0);

    size_t write(char *buffer, uint8_t idx=0);

    bool callibrate();

    void getCallibration(float &lowValue, float &highValue);

    void setCallibration(float lowValue, float highValue);

private:
    inline float rawRead()
    {
        return analogRead(pin) / ANALOG_RESOLUTION * 5000.0f;
    }
};
