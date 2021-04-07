#pragma once

#include "SensorInterface.h"
#include <stdint.h>
#include "utils.h"
#include "DFRobot_PH.h"
#include "WaterTemperature.h"

class PH : public SensorInterface
{
private:
    uint8_t pin;
    DFRobot_PH sensor;
    WaterTemperature *temp;
    
public:
    PH();

    PH(uint8_t pin, WaterTemperature *temp=nullptr, bool initialize=true);

    void init();
    
    /**
     * @brief returns ph reading
     * 
     * @param _ unused
     * @return float ph reading
     */
    float read(uint8_t _=0);

    size_t write(char *buffer, const char *id=nullptr);

    virtual float readTemp();
};
