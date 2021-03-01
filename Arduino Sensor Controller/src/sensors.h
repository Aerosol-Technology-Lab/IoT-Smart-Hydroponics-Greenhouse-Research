#pragma once

#include <Arduino.h>
#include "utils.h"

#ifndef SIMULATOR

#include <OneWire.h>
#include <DallasTemperature.h>

#endif

namespace Sensors {
  
  /**
   * Initializes sensor simulator
   */

#ifndef SIMULATOR
  const unsigned int PIN_TMP0 = 22;
  const unsigned int PIN_TMP1 = 24;
  const unsigned int PIN_TMP2 = 26;

  // OneWire oneWire0(PIN_TMP0);
  // DallasTemperature tmp0(&oneWire0);
  // OneWire oneWire0;
  // DallasTemperature tmp0;

  extern OneWire oneWire0;
  extern OneWire oneWire1;
  extern OneWire oneWire2;
  extern DallasTemperature tmp0;
  extern DallasTemperature tmp1;
  extern DallasTemperature tmp2;

#endif
  
  void init();
  
  /**
   * Performs temperature request
   * 
   * @param buffer: message from UART
   * @param buffer_size: maximum size of buffer
   */
  size_t temperature(const char *buffer, size_t buffer_size, char *out=0, size_t offset=0);

  size_t temperature(int sensorIdx, char *buffer);
  
  size_t ph(const char *buffer, size_t buffer_size, char *out=nullptr, size_t offset=0);

  size_t ping(const char *buffer, size_t buffer_size, char *out=nullptr, size_t offset=0);

  size_t echo(const char *buffer, size_t buffer_size, char *out=nullptr, size_t offset=0);
}
