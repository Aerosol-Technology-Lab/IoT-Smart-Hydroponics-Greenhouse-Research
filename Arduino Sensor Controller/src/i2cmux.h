#pragma once

#include <Arduino.h>
#include <Wire.h>

#ifndef DEFAULT_I2C_MUX_ADDRESS
#define DEFAULT_I2C_MUX_ADDRESS 0x70
#endif

/**
 * @brief Selects port to address on the i2c mux
 * 
 * @param bus bus index to communicate on the i2c mux
 * @param muxAddress address of the mux. Default is 0x70
 */
void i2cmux(uint8_t bus, unsigned char muxAddress=DEFAULT_I2C_MUX_ADDRESS)
{
    Wire.beginTransmission(muxAddress);
    Wire.write(1 << bus);
    Wire.endTransmission();
}
