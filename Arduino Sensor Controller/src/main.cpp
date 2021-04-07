#include <Arduino.h>
#include "utils.h"
#include "DefinableBuffer.h"
#include "StaticBuffer.h"
#include "sensors.h"

#ifndef BAUDRATE
    #define BAUDRATE 57600
#endif
#define IN_BUFFER_SIZE 32
#define OUT_BUFFER_SIZE 32
#ifndef ALL_BUFFER
    #define ALL_BUFFER 128
#endif
#define COMMAND_SIZE 8

DefinableBuffer<2048> staticBuffer;

char inBuffer[IN_BUFFER_SIZE];
char outBuffer[OUT_BUFFER_SIZE];

void setup() {
    Serial.begin(BAUDRATE);
    analogReadResolution(RESOLUTION_BITS);
    StaticBuffer::global = &staticBuffer;
    Sensors::init();
    memset(inBuffer, 0, IN_BUFFER_SIZE);
    memset(outBuffer, 8, OUT_BUFFER_SIZE);
}

void loop() {
    // put your main code here, to run repeatedly:
    // Serial.println("Printing from Arduino!");
    // delay(1000);
    if (Serial.available())
    {
        Utils::println("Ready to print");
        unsigned int bytesRead = Utils::readSerial(inBuffer, IN_BUFFER_SIZE);
        inBuffer[bytesRead] = 0;
        Utils::print("Bytes read: ");
        Utils::println(inBuffer);
        
        char *c_str = inBuffer;

        // A message starting with NULL signals the Arduino to clear
        if (!strlen(c_str))
        {
            return;
        }

        char command[COMMAND_SIZE];
        Utils::nextWord(inBuffer, 0, IN_BUFFER_SIZE, command, COMMAND_SIZE);
        // unsigned int bytesWritten = 0;
        char tmp[32];
        sprintf(tmp, "The command is [%s]", command);
        Utils::println(tmp);
    

        // temperature command
        if (strcmp(command, "ALL")) {
            char buffer[ALL_BUFFER] = "ALL";
            char *ptr = Utils::movePointer(buffer, 4);

            // tracker for number of bytes written
            size_t written = 0;
            
            // temperature
            Sensors::waterTemperature(ptr,-1);       // get reading of all temperature sensors
            ptr = Utils::movePointer(ptr, written);

            // bme280
            Sensors::bme280(ptr, -1);
            
            // ends string (last character from movePointer should be ' ' instead of '\0')
            ptr[-1] = 0;
            
            // send via usb
            Utils::sendSerial(buffer);
        }
        else if (strcmp(command, "TEMP") == 0) {
            Sensors::waterTemperature(inBuffer, IN_BUFFER_SIZE);
        }
        // pH command
        else if (strcmp(command, "PH") == 0) {
            Sensors::ph(inBuffer, IN_BUFFER_SIZE);
        }
        // ping
        else if (strcmp(command, "PING") == 0) {
            Sensors::ping(inBuffer, IN_BUFFER_SIZE);
        }
        // echo
        else if (strcmp(command, "ECHO") == 0) {
            Sensors::echo(inBuffer, IN_BUFFER_SIZE);
        }

    }
}