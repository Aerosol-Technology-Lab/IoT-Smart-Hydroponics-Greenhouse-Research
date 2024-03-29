// set this flag in platform.ini under build_flags
#ifdef SIMULATOR

#include "sensors.h"
#include <Arduino.h>
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#if (defined(DTOSTRF_EXT_LIB) || defined(DUE)) && !defined(ESP32)
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

#endif

void Sensors::init() {
randomSeed(analogRead(0));
}

void Sensors::bme280(JsonObject &obj, int sensorIdx)
{
    if (sensorIdx < 0 || sensorIdx >= 3) {
        obj["error"] = true;
        return;
    }

    obj["temp"] = rand() % 10000 / 100.0f + 20;
    obj["pres"] = rand() % 10000 / 100.0f;;
    obj["hum"]  = rand() % 10000 / 100.0f;;
}

void Sensors::waterTemperature(const char *buffer, size_t buffer_size, bool unused) {
    char next[8];
    Utils::nextWord(buffer, 5, 8, next, 8);
}

// size_t Sensors::waterTemperature(int sensorIdx, char *buffer) {
//     sprintf(buffer, "TMP:0 %.2f TMP:1 %.2f TMP:2 %.2f", random(5000, 10000) * 0.01, random(5000, 10000) * 0.01, random(5000, 10000) * 0.01);
//     return strlen(buffer);
// }

// size_t Sensors::bme280(char *buffer, uint8_t sensorIdx) {
    
// }

void Sensors::waterTemperature(JsonObject &obj, int sensorIdx)
{
    if (sensorIdx < 0 || sensorIdx >= 3) {
        obj["error"] = true;
        return;
    }

    obj["temp"] = rand() % 10000 / 100.0f + 20;
}

void Sensors::ambientLight(JsonObject &obj, int sensorIdx)
{
    if (sensorIdx < 0 || sensorIdx >= 3) {
        obj["error"] = true;
        return;
    }

    obj["light"] = rand() % 4096;
}


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