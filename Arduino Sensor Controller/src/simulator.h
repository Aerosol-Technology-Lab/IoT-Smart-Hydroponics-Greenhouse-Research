#pragma once
#include <Arduino.h>
#include "utils.h"

namespace Sensors {
  
  /**
   * Initializes sensor simulator
   */
  void init();
  
  /**
   * Performs temperature request
   * 
   * @param buffer: message from UART
   * @param buffer_size: maximum size of buffer
   */
  void temperature(const char *buffer, size_t buffer_size);

  void ph(const char *buffer, size_t buffer_size);

  void ping(const char *buffer, size_t buffer_size);

  void echo(const char *buffer, size_t buffer_size);
}
