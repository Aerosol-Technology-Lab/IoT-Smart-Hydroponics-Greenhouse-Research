#ifndef SIMULATOR

#define I2C_BAUD 5000

#include "sensors.h"
#include <Arduino.h>
#include <stdlib.h>
#include "utils.h"
#include "WaterTemperature.h"
#include "PH.h"
#include <ArduinoJson.h>

namespace Sensors {
    WaterTemperature *tmpSensors[NUM_TEMP_SENSORS];
    Pair<I2CBUS, Adafruit_BME280> bmeSensors[NUM_BME_SENSORS];
    BME280_Data bme280Data[NUM_BME_SENSORS];
    Pair<I2CBUS, CCS811*> ccsSensors[NUM_CCS_SENSORS];
    WaterTemperature sharedProbeWaterTemp(PIN_SHARED_PROBE_WATERTEMP, false);
    TDS tdsSensor(PIN_TDS_SENSOR, &sharedProbeWaterTemp);
    PH pHSensor(PIN_PH_SENSOR);
    EC ecSensor(PIN_EC_SENSOR, &sharedProbeWaterTemp);
}

void Sensors::init() {
    randomSeed(analogRead(A0));
    Serial.print("Analog read: ");
    Serial.print(analogRead(A11));

    // initialize I2C
    Wire.begin();
    Wire.setClock(I2C_BAUD);

    // initialize temperature sensors
    for (int i = 0; i < NUM_TEMP_SENSORS; ++i) {
        tmpSensors[i] = new WaterTemperature(TMP_PINS[i], true);
        // tmpSensors[i] = WaterTemperature(TMP_PINS[i], true);
    }

    // initializes ccs811 sensors
    for (int i = 0; i < NUM_CCS_SENSORS; ++i) {

        ccsSensors[i] = Pair<I2CBUS, CCS811*>(Sensors::CHAMBER_I2C_MUX_MAP[i], new CCS811(CCS811_ADDR));
        i2cmux(ccsSensors[i].first);
        if (!ccsSensors[i].second->begin()) {
            char buffer[128];
            sprintf(buffer, "ERROR: CO2 sensor (CCS811) at chamber #%d failed to initialize.\n"
                            "       === Detailed Info ===\n"
                            "       Index:             %2d\n"
                            "       I2C MUX Address: 0x%02x\n"
                , i + 1, i, ccsSensors[i].first
                );
            Serial.print(buffer);
            ccsSensors[i].first = -1;
        }
    }


    // initilize bme280 sensors
    for (int i = 0; i < NUM_BME_SENSORS; ++i) {
        bmeSensors[i] = Pair<I2CBUS, Adafruit_BME280>(Sensors::CHAMBER_I2C_MUX_MAP[i], Adafruit_BME280());
        Adafruit_BME280 &bme = bmeSensors[i].second;
        i2cmux(bmeSensors[i].first);
        bool initialized = bme.begin(BME280_ADDRESS_LIST[i]);
        if (!initialized) {
            char buff[64];
            sprintf(buff, "Failed to initialized BME ID: %d", i);
            Serial.print(buff);
        }
    }

    // initializes water sensors
    // nothing because there is no setup needed for analog pins

    // initialize probes
    sharedProbeWaterTemp.init();
    tdsSensor.init();
    pHSensor.init();
    ecSensor.init();
    Serial.println("-> Done initializing all sensors");
}

void Sensors::waterTemperature(const char *buffer, size_t buffer_size, bool unused) {
    // char next[8];
    // Utils::nextWord(buffer, 5, 8, next, 8);

    float temp = tmpSensors[0]->read();
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
        WaterTemperature *temp = tmpSensors[sensorIdx];
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

    #ifdef DEBUG
    static WaterTemperature *prev = nullptr;
    #endif
    
    // error handling
    if (sensorIdx < 0 || sensorIdx >= NUM_TEMP_SENSORS) {
        obj["error"] = true;
    }
    
    // if (sensorIdx == 2) sensorIdx = 1;
    dev_printf("Attempting to get water temperature...");
    dev_delay(3000);
    dev_println("Getting it now...");
    dev_printf("The water temperature index is: %d %d\n", sensorIdx, TMP_PINS[sensorIdx]);
    
    WaterTemperature &waterTemperatureSensor = *(tmpSensors[sensorIdx]);
    // Serial.print("Sensor Index: ");
    // Serial.println(sensorIdx);
    
    #ifdef DEBUG
    if (prev == &waterTemperatureSensor) {
        dev_println("-> Yep, this is the same as before");
    }
    #endif
    dev_printf("I got the reference");
    dev_delay(2000);
    dev_printf("Starting");
    dev_delay(1000);
    
    obj["temp"] = waterTemperatureSensor.read();
    
    dev_printf("I got the value");
    #ifdef DEBUG
    prev = &waterTemperatureSensor;
    #endif
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
        sprintf(buffer, "%d:{\"temperature\":%.6f,\"pressure\":%.6f,\"humidity\":%.6f}", sensorIdx, temperature, pressure, humidity);
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
    // deprecated feature
    if (sensorIdx < 0) {
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
        
        _pollBME280(sensorIdx);
        obj["temp"] = (bme280Data[sensorIdx].tempetrature * 9.0f / 5.0f ) + 32.0f;
        obj["pres"] =  bme280Data[sensorIdx].pressure     / 1000.0f;
        obj["humd"] =  bme280Data[sensorIdx].humidity;
    }
}

void Sensors::ccs811(JsonObject &obj, int sensorIdx)
{
    if (sensorIdx < 0 || sensorIdx >= NUM_CCS_SENSORS) {
        obj["error"] = true;
        char buffer[42];
        sprintf(buffer, "CCS811 Index out of bounds at [%d].", sensorIdx, NUM_CCS_SENSORS);
        obj["err_mess"] = buffer;
        return;
    }

    if (ccsSensors[sensorIdx].first < 0) {
        // sensor not initialized, continue
        obj["initialized"] = false;
        return;
    }
    else {
        obj["initialized"] = true;
    }


    const unsigned long staleCalibration = 30 * 1000;   // the BME280 reading at the same chamber must be 30 seconds old or new
    if (millis() - bme280Data[sensorIdx].time > staleCalibration) _pollBME280(sensorIdx);

    // switch to mux
    i2cmux(ccsSensors[sensorIdx].first);

    auto &ccs = *ccsSensors[sensorIdx].second;
    while (!ccs.dataAvailable()); // wait until data is available
    ccs.setEnvironmentalData(bme280Data[sensorIdx].humidity, bme280Data[sensorIdx].tempetrature);
    ccs.readAlgorithmResults();
    obj["CO2"]  = ccs.getCO2();
    obj["TVOC"] = ccs.getTVOC();
}

void Sensors::ambientLight(JsonObject &obj, int sensorIdx)
{
    // error handling
    if (sensorIdx < 0 || sensorIdx >= NUM_AMBIENT_LIGHT_SENSORS) {
        obj["error"] = true;
        return;
    }

    obj["light"] = (float) analogRead(AMBIENT_LIGHT_GPIO[sensorIdx]) / ANALOG_RESOLUTION * 100.0f;
}

void Sensors::ph(JsonObject &obj)
{
    obj["ph"] = pHSensor.read();
}

void Sensors::phCallibrate()
{
    pHSensor.callibrate();
}

void Sensors::phGetCalibration(JsonObject &obj)
{
    float neutral, acidic;
    pHSensor.getCallibration(neutral, acidic);
    obj["neutral"] = neutral;
    obj["acidic"]  = acidic;
}

void Sensors::phSetCalibration(float neutralVoltage, float acidicVoltage)
{
    pHSensor.setCallibration(neutralVoltage, acidicVoltage);
}

void Sensors::turbidity(JsonObject &obj)
{
    const uint16_t numPreReads = 5;
    const uint32_t delayTime = 20;

    for (uint16_t i = 0; i < numPreReads - 1; ++i) {
        analogRead(PIN_TURBIDITY_SENSOR);
    }

    const float CONVERSION_FACTOR = 5.0f / float(~(~0 << RESOLUTION_BITS));
    float rawValue = analogRead(PIN_TURBIDITY_SENSOR) * CONVERSION_FACTOR;
    float turbidity = -1120.4f * rawValue * rawValue + 5742.3f * rawValue - 4352.9f;
    
    obj["turb"] = turbidity;
}

void Sensors::tds(JsonObject &obj)
{
    obj["tds"] = tdsSensor.read();
}

void Sensors::ec(JsonObject &obj)
{
    obj["ec"] = ecSensor.read();
}

void Sensors::ecCallibrate()
{
    ecSensor.callibrate();
}

void Sensors::ecSetCallibration(float low, float high)
{
    ecSensor.setCallibration(low, high);
}

void Sensors::ecGetCallibration(JsonObject &obj)
{
    float low, high;
    ecSensor.getCallibration(low, high);
    obj["low"] = low;
    obj["high"] = high;
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

void Sensors::_pollBME280(unsigned int sensorIdx)
{
    // selects i2c mux
    i2cmux(bmeSensors[sensorIdx].first);

    // reads sensor and saves it to json
    Adafruit_BME280 &bme = bmeSensors[sensorIdx].second;
    bme280Data[sensorIdx].time = millis();
    bme280Data[sensorIdx].tempetrature = bme.readTemperature();
    bme280Data[sensorIdx].pressure = bme.readPressure();
    bme280Data[sensorIdx].humidity = bme.readHumidity();
}

#endif
