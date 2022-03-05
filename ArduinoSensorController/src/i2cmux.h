#pragma once

#include <Arduino.h>
#include <Wire.h>

#ifndef DEFAULT_I2C_MUX_ADDRESS
#define DEFAULT_I2C_MUX_ADDRESS 0x70
#endif

typedef int8_t I2CBUS;

/**
 * @brief Selects port to address on the i2c mux
 * 
 * @param bus bus index to communicate on the i2c mux
 * @param muxAddress address of the mux. Default is 0x70
 */
void i2cmux_access_chamber(I2CBUS bus, unsigned char muxAddress=DEFAULT_I2C_MUX_ADDRESS);