#pragma once

#include <Arduino.h>
#include "utils.h"
#include "pair.h"
#include <ArduinoJson.h>

#ifndef SIMULATOR

// #include <OneWire.h>
// #include <DallasTemperature.h>
#include "i2cmux.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "WaterTemperature.h"
#include "TDS.h"
#include "PH.h"

#endif

#define NUM_TEMP_SENSORS 3
#define NUM_BME_SENSORS 3
#define NUM_AMBIENT_LIGHT_SENSORS 3
#define SEALEVELPRESSURE_HPA (1013.25)

namespace Sensors {
  
  /**
   * Initializes sensor simulator
   */

#ifndef SIMULATOR
  const unsigned int TMP_PINS[NUM_TEMP_SENSORS + 1]             = {22, 24, 26, 26};
  const uint8_t BME280_ADDRESS_LIST[NUM_BME_SENSORS]        = { 0x76, 0x76, 0x76};
  const I2CBUS BME280_BUS[NUM_BME_SENSORS]                  = { 0x07, 0x06, 0x05};
  const uint8_t AMBIENT_LIGHT_GPIO[NUM_AMBIENT_LIGHT_SENSORS] = { A0, A1, A2 };
  const uint8_t TURBIDITY_GPIO[1]                             = { A4 };

  // sensor probes pin layout
  const uint8_t PIN_SHARED_PROBE_WATERTEMP              = 28;
  const uint8_t PIN_TDS_SENSOR                          = A2;
  const uint8_t PIN_PH_SENSOR                           = A3;
  
  // OneWire oneWire0(PIN_TMP0);
  // DallasTemperature tmp0(&oneWire0);
  // OneWire oneWire0;
  // DallasTemperature tmp0;

  // temperature sensors
  // extern OneWire oneWireTMP[NUM_TEMP_SENSORS];
  // todo fix this - this is a hack. Making this 3 breaks the program
  extern WaterTemperature *tmpSensors[NUM_TEMP_SENSORS];

  // water temperature sensor for all the probes
  extern WaterTemperature sharedProbeWaterTemp;

  // TDS probe
  extern TDS TDSSensor;

  // bme280
  extern Pair<I2CBUS, Adafruit_BME280> bmeSensors[NUM_BME_SENSORS];

  // pH sensor
  extern PH pHSensor;

#endif
  
  void init();
  
  /**
   * @deprecated
   * Performs temperature request
   * 
   * @param buffer: message from UART
   * @param buffer_size: maximum size of buffer
   */
  void waterTemperature(const char *buffer, size_t buffer_size, bool unused=false);

  /**
   * @deprecated
   */
  size_t waterTemperature(int sensorIdx, char *buffer, bool header=true);
  
  void waterTemperature(JsonObject &obj, int sensorIndex);
  
  /**
   * @deprecated
   * @brief Writes bme280 sensor reading to byte buffer
   * 
   * @param buffer buffer where sensor readings are written to
   * @param sensorIdx sensor index to request reading. -1 for all sensors
   * @return size_t number of bytes written to sensor
   */
  size_t bme280(char *buffer, int sensorIdx, bool header=true);
  
  /**
   * @brief Writes bme280 sensor reading to a json object
   * 
   * @param obj Json Object
   * @param sensorIdx Sensor index to write to Json Object. If -1, a new Json Object is
   *  inserted to obj with key as the sensor index and all readings pertaining for that sensor
   */
  void bme280(JsonObject &obj, int sensorIdx=-1);
  
  /**
   * @brief Returns the raw sensor reading from the ambient light sensor
   * 
   * @param obj Json Object
   * @param sensorIdx select which chamber to read
   */
  void ambientLight(JsonObject &obj, int sensorIdx);
  
  void turbidity(JsonObject &obj);
  
  // @deprecated
  void ph(const char *buffer, size_t buffer_size);

  size_t ph(char *buffer);

  void  ping(const char *buffer, size_t buffer_size);

  void echo(const char *buffer, size_t buffer_size);
}
