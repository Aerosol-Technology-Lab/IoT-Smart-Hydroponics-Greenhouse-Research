# Hydroponic Controller App
This app will control the Hydoponic Greenhouse. This is intented to run on a Raspberry Pi 4 and built, developed, and tested on Linux (Ubuntu 20 x64). This application will collect and
monitor different sensors of the hydroponic system and export that data for further lab analysis.

# Toolchain
The following are required to develop and build this application:
- Visual Studio Code
- [Required Visual Studio Extensions](#required-vs-code-extensions)
- NPM version 14 (tested and working on version lts/fermium - v14.19.0)
- Yarn : `$ npm install -g yarn`

<br/>

The following are optional but may improve workflow
- [NVM - Node Version Manager](https://github.com/nvm-sh/nvm/blob/master/README.md#installing-and-updating) : This tool will help manage different versions of Node.js

# To run

## Setup
These steps must be performed before developing or building the project:
1. `$ yarn` to install all Node dependencies
1. `$ yarn elc-rebuild` to rebuild the front-end electron application with system access dependencies
1. Enable "Watch Sass" in VS Code and make sure each Sass file is watched and compiled to CSS. This button is found in the bottom bar, to the right side of VS Code.
<!-- 1. `yarn rebuild` to rebuild electron with node modules 
Is this needed??-->


## Start Application - Developer Mode
Use this mode when developing the application and for testing purposes
1. `yarn electron-dev` to start build environment

## Build Production Application
Use this mode to build a production ready application without developer tools and with performance optimizations
1. TODO

# Other Scripts
TODO
<br/>

# Extensions
## Required VS Code Extensions
- [Live Sass Compiler](https://marketplace.visualstudio.com/items?itemName=ritwickdey.live-sass)

## Optional
- [Sass](https://marketplace.visualstudio.com/items?itemName=Syler.sass-indented) - Sass syntax highlighter
- [ESLint](https://marketplace.visualstudio.com/items?itemName=dbaeumer.vscode-eslint) - Linting for JavaScript, TypeScript, HTML, and CSS

# Notes
- Not tested on non-Unix OS. Config.js needs to be changed to access the serialport
- If using MacOS with ARM CPU, the application will misassume the machine is a Raspberry Pi. The application determines if the host machine is a Raspberry Pi by checking the CPU architecture.