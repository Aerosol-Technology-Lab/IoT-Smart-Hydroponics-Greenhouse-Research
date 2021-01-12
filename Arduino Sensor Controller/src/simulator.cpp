#include "simulator.h"
#include <Arduino.h>
#include "utils.h"
#include <stdlib.h>

void Sensors::init() {
randomSeed(analogRead(0));
}

void Sensors::temperature(const char *buffer, size_t buffer_size) {
    char next[8];
    Utils::nextWord(buffer, 5, 8, next, 8);
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
