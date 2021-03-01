#ifndef SIMULATOR

#include "sensors.h"
#include <Arduino.h>
#include "utils.h"
#include <stdlib.h>

#if defined(DTOSTRF_EXT_LIB) || defined(DUE)
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

#endif

namespace Sensors {
    OneWire oneWire0(PIN_TMP0);
    DallasTemperature tmp0(&oneWire0);
}

void Sensors::init() {
    randomSeed(analogRead(0));

    // initialize temperature sensors
    oneWire0 = OneWire(PIN_TMP0);
    tmp0 = DallasTemperature(&oneWire0);
    tmp0.begin();
}

void Sensors::temperature(const char *buffer, size_t buffer_size) {
    // char next[8];
    // Utils::nextWord(buffer, 5, 8, next, 8);

    tmp0.requestTemperatures();
    float temp = tmp0.getTempFByIndex(0);
    char send[64];
    char str_tmp[16];
    dtostrf(temp, 4, 2, str_tmp);
    sprintf(send, "TEMP %s", str_tmp);

    Utils::sendSerial(send);
}

size_t Sensors::temperature(int sensorIdx, char *buffer) {
    // bytes written to buffer during this function call
    size_t totalWritten;
    
    // Writes values of all available temperature sensors to the buffer
    if (sensorIdx == -1) {
        size_t written = temperature(0, buffer);
        buffer = Utils::movePointer(buffer, written);
        totalWritten += written;

        written = temperature(1, buffer);
        buffer = Utils::movePointer(buffer, written);
        totalWritten += written;

        written = temperature(2, buffer);
        totalWritten += written;
    }
    // Writes value of particular sensor index to the buffer
    else if (sensorIdx >= 0 && sensorIdx < 3) {
        char tmpBuff[32];
        DallasTemperature *temp;
        switch (sensorIdx)
        {
        case 0:
            temp = &tmp0;
            break;
        case 1:
            temp = &tmp1;
            break;
        case 2:
            temp = &tmp2;
            break;
        }

        temp->requestTemperatures();
        float reading = temp->getTempFByIndex(0);
        sprintf(buffer, "TMP:0 %.2f", temp);
        totalWritten = strlen(buffer) + 1;
    }
    
    return totalWritten;
}

// this is still simulated
void Sensors::ph(const char *buffer, size_t buffer_size) {
    float rand = random(0,1401) * 0.01;
    char send[16];
    char cstring_rand[6];
    dtostrf(rand, 4, 2, cstring_rand);
    sprintf(send, "PHVAL %s", cstring_rand);
    Utils::sendSerial(send);
}

void Sensors::ping(const char *buffer, size_t buffer_size) {
    Utils::sendSerial("PONG");
}

void Sensors::echo(const char *buffer, size_t buffer_size) {
    if (buffer[4] == 0) Utils::sendSerial(0);
    const char *send = buffer + 5;
    // Utils::sendSerial(send);
    Serial.println(send);
}


#endif