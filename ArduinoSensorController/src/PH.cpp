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
    // sensor.begin();
    pinMode(pin, INPUT);
}

float PH::read(uint8_t _)
{
    float voltage = analogRead(pin) / ANALOG_RESOLUTION * VREF_MILLI;
    float slope = (7.0-4.0)/((this->neutralVoltage-1500.0)/3.0 - (this->acidVoltage-1500.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
    float intercept =  7.0 - slope*(this->neutralVoltage-1500.0)/3.0;
    //Serial.print("slope:");
    //Serial.print(slope);
    //Serial.print(",intercept:");
    //Serial.println(intercept);
    return slope*(voltage-1500.0)/3.0+intercept;  //y = k*x + b
    // return sensor.readPH(voltage, readTemp());
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
