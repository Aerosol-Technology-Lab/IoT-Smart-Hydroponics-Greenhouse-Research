# Hydroponic Controller App
This app will control the Hydoponic Greenhouse. This is intented to run on a Raspberry Pi 4 and built, developed, and tested on Linux (Ubuntu 20 x64). This application will collect and
monitor different sensors of the hydroponic system and export that data for further lab analysis.


# To run

## Development
1. `yarn` to install all node dependencies
2. `yarn rebuild` to rebuild electron with node modules
3. `yarn electron-dev` to start build environment

## Production (Raspberry Pi)
1. TODO

# Other Scripts
### `yarn build`
Build React into build directory

# Notes
- Not tested on Windows or OSX. Config.js needs to be changed to access the serialport
