#pragma once

#include <Arduino.h>
#include "utils.h"

#ifndef SIMULATOR

#include <OneWire.h>
#include <DallasTemperature.h>

#endif

#define NUM_TEMP_SENSORS 3

namespace Sensors {
  
  /**
   * Initializes sensor simulator
   */

#ifndef SIMULATOR
  const unsigned int TMP_PINS[NUM_TEMP_SENSORS] = {22, 24, 26};

  // OneWire oneWire0(PIN_TMP0);
  // DallasTemperature tmp0(&oneWire0);
  // OneWire oneWire0;
  // DallasTemperature tmp0;

  extern OneWire oneWireTMP[NUM_TEMP_SENSORS];
  extern DallasTemperature tmpSensors[3];

#endif
  
  void init();
  
  /**
   * Performs temperature request
   * 
   * @param buffer: message from UART
   * @param buffer_size: maximum size of buffer
   */
  void temperature(const char *buffer, size_t buffer_size, bool unused=false);

  size_t temperature(int sensorIdx, char *buffer);
  
  void ph(const char *buffer, size_t buffer_size);

  void  ping(const char *buffer, size_t buffer_size);

  void echo(const char *buffer, size_t buffer_size);
}
