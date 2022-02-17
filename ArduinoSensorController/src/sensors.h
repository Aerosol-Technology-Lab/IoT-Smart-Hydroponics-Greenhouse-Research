#pragma once

#include <Arduino.h>
#include "utils.h"
#include "pair.h"
#include <ArduinoJson.h>
#include "config.h"

#ifndef SIMULATOR

// #include <OneWire.h>
// #include <DallasTemperature.h>
#include "BME280_Data.h"
#include "i2cmux.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#ifndef DISABLE_CCS811
#include <SparkFunCCS811.h>
#endif
#include "WaterTemperature.h"
#include "TDS.h"
#include "PH.h"
#include "EC.h"

#endif

#define NUM_TEMP_SENSORS 3
#define NUM_BME_SENSORS 3
#define NUM_CCS_SENSORS 3
#define NUM_AMBIENT_LIGHT_SENSORS 3
#define SEALEVELPRESSURE_HPA (1013.25)

#define CCS811_ADDR 0x5b
//#define CCS811_ADDR 0x5a  // alternate address

namespace Sensors {
  
  /**
   * Initializes sensor simulator
   */

#ifndef SIMULATOR
  const unsigned int TMP_PINS[NUM_TEMP_SENSORS + 1]             = {22, 24, 26, 26};
  const I2CBUS CHAMBER_I2C_MUX_MAP[NUM_BME_SENSORS]                  = { 0x00, 0x01, 0x02};
  const uint8_t BME280_ADDRESS_LIST[NUM_BME_SENSORS]        = { 0x76, 0x76, 0x76};
  const uint8_t AMBIENT_LIGHT_GPIO[NUM_AMBIENT_LIGHT_SENSORS] = { A0, A1, A2 };

  // sensor probes pin layout
  const uint8_t PIN_SHARED_PROBE_WATERTEMP              = 28;
  const uint8_t PIN_TDS_SENSOR                          = A4;
  const uint8_t PIN_PH_SENSOR                           = A5;
  const uint8_t PIN_TURBIDITY_SENSOR                    = A6;
  const uint8_t PIN_EC_SENSOR                           = A7;
  
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

  // bme280
  extern Pair<I2CBUS, Adafruit_BME280> bmeSensors[NUM_BME_SENSORS];
  extern BME280_Data bme280Data[NUM_BME_SENSORS];

#ifndef DISABLE_CCS811
  extern Pair<I2CBUS, CCS811*> ccsSensors[NUM_CCS_SENSORS];
#endif

  // TDS probe
  extern TDS tdsSensor;

  // pH sensor
  extern PH pHSensor;

  // EC sensor
  extern EC ecSensor;

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
   * @brief Gets CO2 and VTOC in specified chamber
   * 
   * @param obj JsonObject where CO2 and TVOC information is stored. CO2 is stored in ppm and
   *            TVOC in ppb
   * @param sensorIdx Sensor index to read
   */
  void ccs811(JsonObject &obj, int sensorIdx=-1);
  
  /**
   * @brief Returns the raw sensor reading from the ambient light sensor
   * 
   * @param obj Json Object
   * @param sensorIdx select which chamber to read
   */
  void ambientLight(JsonObject &obj, int sensorIdx);
  
  /**
   * @brief Returns PH from PH probe
   * 
   * @param obj Json Object
   */
  void ph(JsonObject &obj);
  
  /**
   * @brief Calibrates PH Sensor. Place sensor in either 7.0 or 4.0 solution and call this function.
   *        The software can automatically determine if solution is neutral or acidic.
   */
  void phCallibrate();
  
  /**
   * @brief Get callibration information of the current sensor
   * 
   * @param obj JsonObject where PH callibration is stored. Object will contain neutral and acidic
   *            voltage at 7.0PH and 4.0PH respectively.
   */
  void phGetCalibration(JsonObject &obj);
  
  /**
   * @brief Sets the calibration variable for the PH sensor
   * 
   * @param neutralVoltage neutral voltage for 7.0PH solution
   * @param acidicVoltage acidic voltage for 4.0PH solution
   */
  void phSetCalibration(float neutralVoltage, float acidicVoltage);
  
  /**
   * @brief Returns the turbidity from the turbidity sensor. Units are in NTU. This must be referenced
   *        solution to determine relationship between NTU and ppm. NTU and ppm has a linear and positive
   *        relationship.
   * 
   * @param obj JsonObject where turbidity information is stored with key "turb"
   */
  void turbidity(JsonObject &obj);
  
  void tds(JsonObject &obj);
  
  void ec(JsonObject &obj);
  
  void ecCallibrate();

  void ecSetCallibration(float kLow, float kHigh);

  void ecGetCallibration(JsonObject &obj);
  
  void  ping(const char *buffer, size_t buffer_size);

  void echo(const char *buffer, size_t buffer_size);

  /*     --- Internal Functions - do not call */
  /**
   * @brief Polls BME280 and updates values according to sensor index requested
   * @note No error handling. Make sure that sensor index is valid
   * 
   * @param sensorIdx sensor index requested
   */
  void _pollBME280(unsigned int sensorIdx);
  
}
