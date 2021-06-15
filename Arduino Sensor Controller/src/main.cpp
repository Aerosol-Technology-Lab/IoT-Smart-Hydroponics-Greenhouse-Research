#include <Arduino.h>
#include "utils.h"
#include "DefinableBuffer.h"
#include "StaticBuffer.h"
#include "sensors.h"
#include <ArduinoJson.h>

#ifndef BAUDRATE
    #define BAUDRATE 57600
#endif
#define IN_BUFFER_SIZE 32
#define OUT_BUFFER_SIZE 32
#ifndef ALL_BUFFER
    #define ALL_BUFFER 128
#endif
#define COMMAND_SIZE 8

// Size of JSON document
#ifndef JSON_DOCUMENT_SIZE
    #define JSON_DOCUMENT_SIZE 4096
#endif

#define NUM_CHAMBERS 3

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
    
        // JSON Document for all returns
        StaticJsonDocument<2048> jsonDocument;

        // temperature command
        if (strcmp(command, "ALL")) {
            JsonObject sensorReadings = jsonDocument.createNestedObject("sensors");
            JsonObject chamber = sensorReadings.createNestedObject("chamber");
            
            for (int i = 0; i < NUM_CHAMBERS; ++i) {
                // create nested object
                char buffer[4];
                sprintf(buffer, "%d", i);
                JsonObject currentChamber = chamber.createNestedObject(buffer);

                // get sensor readings for each chamber

                // bme
                {
                    JsonObject bme280 = currentChamber.createNestedObject("bme280");
                    Sensors::bme280(bme280, i);
                }
                
                // water temp
                {
                    JsonObject waterTemp = currentChamber.createNestedObject("water");
                    Sensors::waterTemperature(waterTemp, i);
                }

                // ambient light
                {
                    JsonObject ambientLight = currentChamber.createNestedObject("light");
                    Sensors::waterTemperature(ambientLight, i);
                }
            }
            
            // send via usb
            serializeJson(jsonDocument, Serial);
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