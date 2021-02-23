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