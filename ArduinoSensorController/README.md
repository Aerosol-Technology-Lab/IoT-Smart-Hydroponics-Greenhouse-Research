# Arduino Due Software
This codebase is for the Arduino component of the hydroponics system. The Arduino is the center for data acquisition for all sensors in each chamber. With the microcontroller (MCU) within the Arduino, the Arduino is better suited for sensor acquisition purposes, especially for analog sensors (e.g. ambient light sensors). Another benefit, while minimal, offloads CPU cycles from the Raspberry Pi to the Arduino for data acquisition. The Raspberry Pi polls the Arduino at a fixed schedule over UART (Raspberry Pi -> USB [via cable] -> |Arduino| -> USB to UART controller -> MCU) to acquire latest sensor readings.

## Data Polled
The Arduino polls the following values from each sensor:
- Waterproof Temperature Sensor [1-Wire]
    - Water temperature (°F)
- Ambient Light Sensor [Analog]
    - Lumens
- Air Quality Sensor (BME280) [I2C]
    - Air Temperature (°F)
    - Humidity (Liquid/Air %)
    - Air Pressure (kPa)
- CO2 / Particulate Sensor (CCS811) [I2C]
    - CO2 (TODO: add units)
    - TVOC (sent over UART but unused)

## Requests
When requesting data via UART, ensure there is at least a 500ms delay between the previous request and the new request.

### Sendable Requests
The following requests can be sent to the Arduino:

- Request for all sensor readings (reffer to [Data Format](#Data-Format))
```sdds
// Request for all sensor readings (reffer )
{
    com: 'ALL'
}
```
- Ping -> verify Arduino is alive and can process requests
```
{
    com: 'PING'
}
```

## Data Format
If the Arduino received a request to send sensor readings over UART, the Arduino 

Example successful reading:
```
{
    sensors : {
        chamber: {
            0: {
                bme280: {
                    temp:  77.9232,       // Fahrenheit
                    pres: 101.323,       // kPa
                    humd:  47.343         // humidity (percent)
                },
                ccs811: {
                    co2:  <TODO>,
                    tvoc: <TODO>
                },
                temp:  78.23435,         // Fahrenheit
                light: 78.34534          // intensity (percent)
            },
            1: {
                ...
            },
            .
            .
            .,
            n: {
                ...
            }
        }
    }
}
```

### Note
- Horizontal ellipsis represnt similar content pattern to the item above.
- Veritcal ellipsis represent repeated pattern between the top and bottom items. The key for repeated items increments by one.
- The value of `n` for the current hydroponics system is `n = 3`.
- If too many requests are sent to the Arduino at once or if a new request is sent while the Arduino is in the middle of an ongoing request, the new or most recent requests may be ignored until the Arduino completes its current request.
