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
    Serial.println("Hello world!");
    while (1) {
        Serial.println("LOOP");
    }
    #ifndef UNO
    analogReadResolution(RESOLUTION_BITS);
    #endif
    dev_println("Starting...");
    StaticBuffer::global = &staticBuffer;
    dev_println("Initializing sensors...");
    Sensors::init();
    dev_println("Done!");
    memset(inBuffer, 0, IN_BUFFER_SIZE);
    memset(outBuffer, 8, OUT_BUFFER_SIZE);
    // temperatureRead();
    StaticJsonDocument<248> response;
    response["initialized"] = true;
    serializeJson(response, Serial);
}

// void readUntil(char *buffer, char trigger, size_t bufferLength, Stream &stream)
// {
//     size_t pointer = 0;
//     while (pointer < bufferLength) {
//         while (!Serial.available()) {
//             vTaskDelay(100 / portTICK_PERIOD_MS)
//         }
//     }
// }

void loop() {
    // put your main code here, to run repeatedly:
    // Serial.println("Printing from Arduino!");
    // delay(1000);
    dev_println("Loop");
    delay(1000);
    if (Serial.available())
    {
        // Serial.println("Reading");
        StaticJsonDocument<248> inputJSON;
        {
            // contain String object with RAII
            String input = Serial.readStringUntil('\0');
            deserializeJson(inputJSON, input);
            Serial.println("Done!");
        }

        // if no command, do nothing
        if (!inputJSON.containsKey(F("com"))) return;

        // JSON Document for all returns
        StaticJsonDocument<2048> jsonDocument;
        jsonDocument["response"] = inputJSON["com"];
        
        // temperature command
        if (strcmp(inputJSON[F("com")], "ALL") == 0) {
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
                    Sensors::ambientLight(ambientLight, i);
                }
            }
            
            // send via usb
            serializeJson(jsonDocument, Serial);
            Serial.print('\0');
        }
        else if (strcmp(inputJSON[F("com")], "POINT")) {

        }
        else if (strcmp(inputJSON[F("com")], "TEMP") == 0) {
            Sensors::waterTemperature(inBuffer, IN_BUFFER_SIZE);
        }
        // pH command
        else if (strcmp(inputJSON[F("com")], "PH") == 0) {
            Sensors::ph(inBuffer, IN_BUFFER_SIZE);
        }
        // ping
        else if (strcmp(inputJSON[F("com")], "PING") == 0) {
            jsonDocument["pong"] = "pong";
            serializeJson(jsonDocument, Serial);
            Serial.print('\0');
        }
        // echo
        else if (strcmp(inputJSON[F("com")], "ECHO") == 0) {
            Sensors::echo(inBuffer, IN_BUFFER_SIZE);
        }
        else {
            jsonDocument["error"] = true;
            serializeJson(jsonDocument, Serial);
            Serial.print('\0');
        }

        #ifdef DEBUG
        jsonDocument["res"] = inputJSON["com"];
        #endif

    }

}