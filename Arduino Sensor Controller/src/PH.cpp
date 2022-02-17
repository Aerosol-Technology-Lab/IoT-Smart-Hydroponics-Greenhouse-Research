#include "PH.h"

#include "Arduino.h"
#include "utils.h"

PH::PH() : temp(nullptr) {}

PH::PH(uint8_t pin, WaterTemperature *temp, bool initialize)
    : pin(pin), temp(temp)
{
    if (initialize) init();
}

void PH::init()
{
    sensor.begin();
    pinMode(pin, INPUT);
}

float PH::read(uint8_t _)
{
    float voltage = analogRead(pin) / ANALOG_RESOLUTION * VREF_MILLI;
    return sensor.readPH(voltage, readTemp());
}

float PH::readTemp()
{
    return temp->read();
}

size_t PH::write(char *buffer, uint8_t idx)
{
    sprintf(buffer, "\"ph\": %.6f,", read());
    return strlen(buffer);
}
