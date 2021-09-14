#include <Arduino.h>
#include "utils.h"
#include "DefinableBuffer.h"
#include "StaticBuffer.h"
#include "sensors.h"
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BME280.h>

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

#define ESCAPE_SEQUENCE '\0'

char inBuffer[IN_BUFFER_SIZE] = { 0 };
char outBuffer[OUT_BUFFER_SIZE] = { 0 };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  analogReadResolution(RESOLUTION_BITS);
  Sensors::init();
  StaticJsonDocument<248> response;
  response["initialized"] = true;
  serializeJson(response, Serial);
}

void loop() {
    // put your main code here, to run repeatedly:
    // Serial.println("Printing from Arduino!");
    // delay(1000);
    if (Serial.available())
    {
        // Serial.println("Reading");
        StaticJsonDocument<248> inputJSON;
        {
            // contain String object with RAII
            String input = Serial.readString();
            deserializeJson(inputJSON, input);
            #ifdef DEBUG
            Serial.println("Done!");
            serializeJsonPretty(inputJSON, Serial);
            #endif
            
        }

        // if no command, do nothing
        if (!inputJSON.containsKey("com")) {
          dev_printf("This does not contain the key");
          return;
        }
        #ifdef DEBUG
        else {
          Serial.print("Found the key!");
        }
        #endif

        // JSON Document for all returns
        StaticJsonDocument<4096> jsonDocument;
        String response = inputJSON["com"];
        dev_print("Response: ");
        dev_println(response);
        jsonDocument["response"] = inputJSON["com"];
        dev_delay(1000);
        // temperature command
        if (response == "ALL") {
            JsonObject sensorReadings = jsonDocument.createNestedObject("sensors");
            JsonObject chamber = sensorReadings.createNestedObject("chamber");
            
            dev_println("Processing all chambers");
            dev_delay(100);
            for (int i = 0; i < NUM_CHAMBERS; ++i) {
              dev_printf("-> Processing chamber %d\n", i);
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
                dev_println("Done with bme280");
                dev_delay(1000);

                // ccs811
                {
                    JsonObject ccs811 = currentChamber.createNestedObject("ccs811");
                    Sensors::ccs811(ccs811, i);
                }
                dev_println("Done with ccs811");
                dev_delay(1000);
                
                // water temp
                {
                    Sensors::waterTemperature(currentChamber, i);
                }
                dev_println("Done with water temp");
                dev_delay(1000);

                // ambient light
                {
                    Sensors::ambientLight(currentChamber, i);
                }
                dev_println("Done with ambient light");
                dev_delay(1000);
            }
            dev_delay(1000);
            // send via usb
            #ifdef DEBUG
            serializeJsonPretty(jsonDocument, Serial);
            #else
            serializeJsonPretty(jsonDocument, Serial);
            #endif
            Serial.print(ESCAPE_SEQUENCE);
        }
        else if (response == "POINT") {

        }
        else if (response == "TURB") {
            
            JsonObject obj = jsonDocument.as<JsonObject>();
            Sensors::turbidity(obj);
            Serial.print(ESCAPE_SEQUENCE);
        }
        else if (response == "TEMP") {
            Sensors::waterTemperature(inBuffer, IN_BUFFER_SIZE);
        }
        // pH command
        else if (response == "PH") {
            Sensors::ph(inBuffer, IN_BUFFER_SIZE);
        }
        // ping
        else if (response == "PING") {
            jsonDocument["pong"] = "pong";
            serializeJson(jsonDocument, Serial);
            delay(50);
            Serial.print(ESCAPE_SEQUENCE);
        }
        // reset
        else if (response == "RESET") {

            // this should emulate hardware reset for Arduino Due
            #ifdef ARDUINO_SAM_DUE
            RSTC->RSTC_MR = 0xA5000801;
            RSTC->RSTC_CR = 0xA5000013;
            #else
            #error "Define a reset functionality for an equivalent board. A hardware reset is preffered"
            #endif
            // rstc_start_software_reset(RSTC); // verify that this works
        }
        // echo
        else if (response == "ECHO") {
            Sensors::echo(inBuffer, IN_BUFFER_SIZE);
        }
        else {
            jsonDocument["error"] = true;
            serializeJson(jsonDocument, Serial);
            Serial.print(ESCAPE_SEQUENCE);
        }

        #ifdef DEBUG
        jsonDocument["res"] = inputJSON["com"];
        #endif

    }
    else {
      delay(500);
    }

}
