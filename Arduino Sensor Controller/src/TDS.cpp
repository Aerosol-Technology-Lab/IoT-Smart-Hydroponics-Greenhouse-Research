#include "TDS.h"
#include <stdint.h>
#include <stdlib.h>
#include "utils.h"

TDS::TDS(uint8_t pin, WaterTemperature *temp, bool initialize)
    : pin(pin),
      temp(temp)
{
    if (initialize) {
        init();
    }
}

void TDS::init()
{
    if (!initialized) {
        pinMode(pin, INPUT);
        initialized = true;
    }
}

float TDS::read(uint8_t idx)
{
    for (int i = 0; i < SCOUNT; ++i) {
        analogBuffer[i] = analogRead(pin);
    }

    averageVoltage = getMedianNum(analogBuffer, SCOUNT) * VREF / ANALOG_RESOLUTION; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
    float compensationCoefficient = 1.0f + 0.02f * (temperature - readTemp());    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient;  //temperature compensation
    return (133.42f * compensationVolatge * compensationVolatge * compensationVolatge - 255.86f * compensationVolatge * compensationVolatge + 857.39f * compensationVolatge ) * 0.5f; //convert voltage value to tds value
}

size_t TDS::write(char *buffer, const char *id) {

    float tds = read();
    sprintf(buffer, "TDS %d", (int)(tds + 0.5f));
    return strlen(buffer) + 1;
}

float TDS::readTemp()
{
    if   (!temp) return 25.0f;        // no temperature sensor given, just use default value
    else return temp->read();
}

/**
 * Code source from DFRobot
 */
int TDS::getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];
    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }
    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
    return bTemp;
}
