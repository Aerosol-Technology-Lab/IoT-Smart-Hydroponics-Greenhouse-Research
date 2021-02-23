const { app, BrowserWindow, ipcMain, nativeImage } = require('electron');
const os = require('os');
const process = require('process');
const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');
const url = require('url');
const uuidv4 = require('uuid/v4');
const glob = require('glob');
const SerialPort = require('serialport');
const ReadLine = require('@serialport/parser-readline');
const usb = require('usb');

// App imports
const ConfigLoader = require('./ConfigLoader');

const isDev = true ? require('electron-is-dev') : false;

// initialize temp dir
const tmpFolderName = 'IotHydroApp';
const tmpPath = path.join(os.tmpdir(), tmpFolderName);
if (!fs.existsSync(tmpPath)) fs.mkdirSync(tmpPath);

// check if other instances are running

// const uuidName = uuidv4();
// execSync(`mkfifo ${path.join(tmpPath, uuidName)}`);
// execSync(`mkfifo ${path.join(tmpPath, 'controller')}`);
// fs.writeFileSync(path.join(tmpPath, uuidName));
// let fds = fs.createReadStream(path.join(tmpPath, uuidName));
// fds.on('data', (d) => {
//   d = d.toString();
  
//   if (d !== uuidName) {
//     console.log('Another process is running.');
//     process.exit(0);
//   }
//   else {
//     fds.close();
//   }
// });



/**
 * Load configurations
 */
const configManager = new ConfigLoader('./Config.json');
var Config = configManager.load();
console.log(Config);
Config.runtime = {};    // runtime config variables


// main app window
var mainWindow;
var quitOnAllWindowClosed = true;
var appIcon = nativeImage.createFromPath(path.join(__dirname, '../src/logo.svg'));
console.log(`Dirname: ${__dirname}`);
// app window settings
var appSettings = {
  width: 800, 
  height: 480,
  webPreferences: {
    nodeIntegration: true
  },
  transparent: true,
  resizable: false,
  frame: true,
  fullscreen: false,
  icon: appIcon,
};

// check for operating system
Config.runtime.platform = os.platform();

if (Config.runtime.platform !== 'linux') {
  console.log(`Warning! The system ${Config.runtime.platform} is not officially supported.
  This has been developed for Linux operating systems.`);
}
else {
  // this is linux, so store PID in runtime
  Config.runtime.pid = process.pid;
}


// check for platform
Config.runtime.arch = process.arch;
if (Config.runtime.arch === 'arm') { // set defaults for raspberry pi
  appSettings.fullscreen = true;
  appSettings.frame = false;
  Config.runtime.cursor = false;
}
else {
  Config.runtime.cursor = true;
}

configManager.write(Config);


// Arduino
var arduino = null;
var parser = null;

function createWindow() {
  mainWindow = new BrowserWindow(appSettings);
  mainWindow.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`);
  mainWindow.on('closed', () => mainWindow = null);
  mainWindow.on('ready-to-show', () => {
    mainWindow.webContents.send('enable-cursor', Config.runtime.cursor);
  })
}

ipcMain.handle('my-invokable-ipc', async (event, ...args) => {
  return "hello"
});

ipcMain.handle('enable-cursor', async (event, ...args) => {
  return Config.runtime.cursor;
});

ipcMain.handle('cwd', async (event, ...args) => {
  return process.cwd();
});

ipcMain.handle('serialport', async (event, ...args) => {
  if (arduino) {
    return 'connected';
  }
  else {
    return 'no-connection';
  }
});

/**
 * USB
 */
var connectedUSB = 0;

var usbState = () => {
  console.log(`Connected devices: ${connectedUSB}`);
  return connectedUSB > 0 ? 'connected' : 'disconnected';
};
 
usb.on('attach', (device) => {
  console.log('USB Device is connected!');
  ++connectedUSB;
  mainWindow.send('usb', usbState());
});

usb.on('detach', (device) => {
  connectedUSB = Math.max(connectedUSB - 1, 0);

  console.log('device disconnected');
  if (connectedUSB === 0) {
    mainWindow.send('usb', usbState());
  }
});

ipcMain.handle('usb', async (event, args) => {
  return usbState();
});

const reconnectArduino = async function() {

    
  var port = new SerialPort(Config.serial_directory, {
    baudRate: Config.baudRate,
  });

  port.on("error", (err) => {
    if (err.toString().startsWith("Error: Error: Permission denied")) {
      console.log(
        `Run script to enable device [${err.toString().substr(45)}]`
      );
      mainWindow.webContents?.send('serialport', {state: 'permission-error'});
    } 
    else {
      console.log(`Connection to Arduino error: ${err}`);
    }

    if (err) {
        setTimeout(reconnectArduino, 3000);
    }
  });

  
  port.on("open", () => {
    arduino = port;
    
    console.log("Port is opened");
    parser = arduino.pipe(new ReadLine({ delimiter: '\0' }));
    parser.on('data', (data) => {
      // console.log(data);
      let comms = data.split(' ');

      if (comms[0] === 'PHVAL') {
        // console.log('Got here');
        mainWindow?.webContents?.send('ph', comms[1]);
      }
      else if (comms[0] === 'TEMP') {
        // console.log('Got TEMP');
        mainWindow?.webContents?.send('tmp', comms[1]);
      }
      // mainWindow.webContents.send('ph', )
    });
    
    mainWindow.webContents.send('serialport', {state: 'connected'});
    
    // arduino.on("data", (data) => {
    //   console.log(data);
    // });

    // arduino.on("readable", () => {
    //   let s = arduino.read();
    //   console.log(s.toString());
    // });

    arduino.on("close", () => {
      arduino = null;
      console.log('Arduino is closed');
      if (mainWindow) {
        mainWindow.webContents.send('serialport', {state: 'disconnected'});
      }
      reconnectArduino();
    });

    // port.on('data', function(data) {
    //     console.log('Data: ', data);
    // });

    arduino.write(Buffer.from("HELLO"));

    setInterval(() => {
      if (arduino) {
        arduino.write("PH\0");
        arduino.write("TEMP\0");
      }
    }, 500);
  });


}

reconnectArduino();

app.allowRendererProcessReuse = false;
app.on('ready', createWindow);


app.on('window-all-closed', () => {
  if (process.platform !== 'darwin' && quitOnAllWindowClosed) {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});

/*** IPC ***/
ipcMain.handle('get-time', async (event, ...args) => {
  // if (Config.runtime.arch == 'x64')
  return new Date();
});

/*** ROUTINES ***/

// Resets main window after a period of time defined in Config.js
// This prevents memory leak. In normal browser use, this is not
// needed because a web page does not last for a long period of time.
// This app needs to run 24/7 so this is necessary
setInterval(() => {
  if (mainWindow) {
    quitOnAllWindowClosed = false;
    mainWindow.close();
    // setTimeout(() => createWindow(), 1000);
    createWindow();
    quitOnAllWindowClosed = true;
  }
  
}, Config.refreshWindow * 1000);
