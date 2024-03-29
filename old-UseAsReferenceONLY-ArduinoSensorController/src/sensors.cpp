#ifndef SIMULATOR_

#include "sensors.h"
#include <Arduino.h>
#include <stdlib.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include "utils.h"
#include "WaterTemperature.h"
#include "PH.h"
#include <ArduinoJson.h>

#if defined(DTOSTRF_EXT_LIB) || defined(DUE)
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

#endif

namespace Sensors {
    WaterTemperature tmpSensors[3];
    Pair<I2CBUS, Adafruit_BME280> bmeSensors[NUM_BME_SENSORS];
    WaterTemperature sharedProbeWaterTemp(PIN_TDS_SENSOR, false);
    TDS TDSSensor(PIN_TDS_SENSOR, &sharedProbeWaterTemp);
    PH pHSensor(PIN_PH_SENSOR, &sharedProbeWaterTemp);
}

void Sensors::init() {
    randomSeed(analogRead(0));

    // initialize I2C
    Wire.begin();

    // initialize temperature sensors
    for (int i = 0; i < NUM_TEMP_SENSORS; ++i) {
        // new(tmpSensors + i) WaterTemperature(TMP_PINS[i], true);
        tmpSensors[i] = WaterTemperature(TMP_PINS[i], true);
    }

    // initilize bme280 sensors
    for (int i = 0; i < NUM_BME_SENSORS; ++i) {
        // new(bmeSensors + i) Pair<I2CBUS, Adafruit_BME280>(Sensors::BME280_BUS[i], Adafruit_BME280());
        bmeSensors[i] = Pair<I2CBUS, Adafruit_BME280>(Sensors::BME280_BUS[i], Adafruit_BME280());
        Adafruit_BME280 &bme = bmeSensors[i].second;
        i2cmux(bmeSensors[i].first);
        bool initSuccess = bme.begin(BME280_ADDRESS_LIST[i]);
        if (!initSuccess) {
            char buffer[64];
            sprintf(buffer, "Failed to initialize BME280 ID:%d\n", i);
            Serial.print(buffer);
        }
    }

    // initializes water sensors
    // nothing because there is no setup needed for analog pins

    // initialize probes
    sharedProbeWaterTemp.init();
    TDSSensor.init();
    pHSensor.init();
}

void Sensors::waterTemperature(const char *buffer, size_t buffer_size, bool unused) {
    // char next[8];
    // Utils::nextWord(buffer, 5, 8, next, 8);

    float temp = tmpSensors[0].read();
    char send[64];
    char str_tmp[16];
    dtostrf(temp, 4, 2, str_tmp);
    sprintf(send, "TEMP %s", str_tmp);

    Utils::sendSerial(send);
}

size_t Sensors::waterTemperature(int sensorIdx, char *buffer, bool header) {
    // bytes written to buffer during this function call
    size_t totalWritten = 0;
    
    if (header) {
        buffer = "\"water_temperature\":{";
        size_t move = strlen(buffer);
        Utils::movePointer(buffer, move);
        totalWritten += move;
    }
    
    // Writes values of all available temperature sensors to the buffer
    if (sensorIdx == -1) {
        for (int i = 0; i < NUM_TEMP_SENSORS; ++i) {
            size_t written = waterTemperature(i, buffer, false);
            buffer = Utils::movePointer(buffer, written);
            totalWritten += written;
        }
    }
    // Writes value of particular sensor index to the buffer
    else if (sensorIdx >= 0 && sensorIdx < 3) {
        WaterTemperature *temp = tmpSensors + sensorIdx;
        totalWritten = temp->write(buffer, sensorIdx);
    }
    
    if (header) {
        buffer = "},";
        Utils::movePointer(buffer, 3);
        totalWritten += 3;       
    }
    
    return totalWritten;
}

void Sensors::waterTemperature(JsonObject &obj, int sensorIdx)
{
    // error handling
    if (sensorIdx < 0 || sensorIdx >= NUM_TEMP_SENSORS) {
        obj["error"] = true;
    }
    
    WaterTemperature &waterTemperatureSensor = tmpSensors[sensorIdx];
    obj["temp"] = waterTemperatureSensor.read(sensorIdx);
}

size_t Sensors::bme280(char *buffer, int sensorIdx, bool header)
{
    size_t bytesWritten = 0;    // bytes written to buffer
    
    if (header) {
        buffer = "\"bme280\":{";
        size_t written = strlen(buffer);
        Utils::movePointer(buffer, written);
        bytesWritten += written;
    }
    
    // if -1, return all sensors
    if (sensorIdx == -1) {
        for (unsigned int i = 0; i < NUM_BME_SENSORS - 1; ++i) {
            size_t written = bme280(buffer, i, false);
            buffer = Utils::movePointer(buffer, written);
            bytesWritten += written;
        }

        // last item, no need to move buffer pointer
        bytesWritten += bme280(buffer, NUM_BME_SENSORS - 1);
    }
    //else if 0 <= sensorIdx < MAX_NUMBER_SENSORS, return only that selected sensor
    else if (sensorIdx >= 0 && sensorIdx < NUM_BME_SENSORS) {
        // selects i2c mux
        i2cmux(bmeSensors[sensorIdx].first);

        // reads sensor
        Adafruit_BME280 &bme = bmeSensors[sensorIdx].second;
        float temperature = (bme.readTemperature() * 9.0f / 5.0f ) + 32.0f;
        float pressure = bme.readPressure() / 100.0f;
        float humidity = bme.readHumidity();
        
        // write to buffer
        sprintf(buffer, "%d:{\"temp\":%.6f,\"pres\":%.6f,\"humd\":%.6f}", sensorIdx, temperature, pressure, humidity);
        // sprintf(buffer, "BME:%d %.4f %.4f %.4f", sensorIdx, temperature, pressure, humidity);
        bytesWritten = strlen(buffer) + 1;
    }

    if (header) {
        buffer = "},";
        Utils::movePointer(buffer, 3);
        bytesWritten += 3;       
    }

    return bytesWritten;
}

void Sensors::bme280(JsonObject &obj, int sensorIdx)
{
    // requesting all sensors
    if (sensorIdx == -1) {
        for (int i = 0; i < NUM_BME_SENSORS; ++i) {
            char stringNameBuff[4];
            sprintf(stringNameBuff, "%d", sensorIdx);
            JsonObject currentSensorObj = obj.createNestedObject(stringNameBuff);
            bme280(currentSensorObj, i);
        }
    }
    // request only one specific sensor
    else {
        // error handling
        if (sensorIdx >= NUM_BME_SENSORS) {
            obj["error"] = true;
            return;
        }
        
        // selects i2c mux
        i2cmux(bmeSensors[sensorIdx].first);

        // reads sensor and saves it to json
        Adafruit_BME280 &bme = bmeSensors[sensorIdx].second;
        obj["temp"] = (bme.readTemperature() * 9.0f / 5.0f ) + 32.0f;
        obj["pres"] = bme.readPressure() / 100.0f;
        obj["humd"]  = bme.readHumidity();
    }
}

void Sensors::ambientLight(JsonObject &obj, int sensorIdx)
{
    // error handling
    if (sensorIdx < 0 || sensorIdx >= NUM_AMBIENT_LIGHT_SENSORS) {
        obj["error"] = true;
        return;
    }

    obj["light"] = analogRead(AMBIENT_LIGHT_GPIO[sensorIdx]);
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

size_t Sensors::ph(char *buffer)
{
    return pHSensor.write(buffer);
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
