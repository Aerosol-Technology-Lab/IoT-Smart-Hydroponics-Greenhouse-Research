#include "i2cmux.h"

void i2cmux_access_chamber(I2CBUS bus, unsigned char muxAddress)
{
    Wire.beginTransmission(muxAddress);
    Wire.write(1 << bus);
    Wire.endTransmission();
}
