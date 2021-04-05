#pragma once

#include "utils.h"
#include "SensorInterface.h"
#include "WaterTemperature.h"

#ifdef UNO
    #ifndef VREF
        #define VREF 5.0
    #endif
    #ifndef ANALOG_RESOLUTION
        #define ANALOG_RESOLUTION 1023.0f
    #endif
#elif DUE
    #ifndef VREF
        #define VREF 3.3
    #endif
    #ifndef ANALOG_RESOLUTION
        #define ANALOG_RESOLUTION 4095.0f
    #endif
#endif

#define SCOUNT 30

#define TDS_DEFAULT_TEMP 25.0f

class TDS : public SensorInterface
{
private:
    int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
    int analogBufferIndex = 0,copyIndex = 0;
    float averageVoltage = 0,tdsValue = 0,temperature = 25;

    WaterTemperature *temp;
    uint8_t pin;
    
    bool initialized = false;

public:

    TDS(uint8_t pin, WaterTemperature *temp = nullptr, bool initialize=false);

    void init();

    float read(uint8_t idx=0);
    
    size_t write(char *buffer, const char *id=nullptr);

    float readTemp();

    static int getMedianNum(int bArray[], int iFilterLen);
};
