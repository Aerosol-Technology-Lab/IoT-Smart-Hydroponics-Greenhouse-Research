const { assert } = require('console');
const SingleInstance = require('single-instance');
const locker = new SingleInstance('Hydroponics-Touch-App');
const DBPATH = 'hydro.db';

locker.lock().then(() => {

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
  const sqlite3 = require('sqlite3');

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
  // var currentReadings = undefined;
  var currentReadings = {
    'time': new Date(0),
    '0': {
      'temp': 80,
      'light': 90,
      'bme280': {
        'temp': 76,
        'humd': 78,
        'pres': 103.23,
      }
    },'1': {
      'temp': 76,
      'light': 88,
      'bme280': {
        'temp': 70,
        'humd': 68,
        'pres': 103.76,
      }
    },'2': {
      'temp': 81,
      'light': 95,
      'bme280': {
        'temp': 63,
        'humd': 41,
        'pres': 103.56,
      }
    },
  }
  var buffer = Buffer.alloc(0);

  const processData = async(data) => {

    console.log(`Data is processed! We have ${data.toString()}`);
    var response;
    
    try {
      response = JSON.parse(data.toString());
    } catch (error) {
      // nothing to do, cannot process this data
      return;
    }

    // cannot process if it does not have anything
    if (!('response' in response)) {
      return;
    }
    if (response.response === 'ALL') {
      response['sensors']['chamber']['time'] = new Date();
      currentReadings = response.sensors.chamber;
    }
  };
  
  const processBuffer = async () => {
    // console.log(`-> All data is: ${buffer.toString()}`);
    let termination = -1;
    for (let i = 0; i < buffer.length; ++i) {
      // console.log(`${buffer[i]} `);
      if (buffer[i] == 0) {
        // console.log('-> I found the terminator');
        termination = i;
        break;
      }
    }
    
    // console.log(`-> Processing buffer stage 1, data is: ${buffer.toString()}`);
    // console.log(buffer.subarray(buffer.length - 50));
    // console.log('-> Stage 1 Done');
    
    if (termination >= 0) {
      // console.log('-> Processing Buffer!');
      let subData = buffer.subarray(0, termination);
      console.log(`-> Termination: ${subData}`);
      processData(subData); // get all characters up to the null terminator
      buffer = buffer.subarray(termination + 1);    // get all characters after the null terminator
      processBuffer();  
      // console.log('-> Processing Buffer Done!');                            // process buffer until all of buffer is processed
    }
  };
  
  const storeDataToDB = async (data) => {
    let db = new sqlite3.Database(DBPATH, sqlite3.OPEN_READWRITE, (err) => {
      if (err) {
        console.error(err.message);
        return;
      }

      // assert(false);
    });

    
    db.run(`INSERT INTO chamber (
      time,
      wtmp0,
      pres0,
      humd0,
      atmp0,
      lght0,
      wtmp1,
      pres1,
      humd1,
      atmp1,
      lght1,
      wtmp2,
      pres2,
      humd2,
      atmp2,
      lght2
      )
    VALUES(
      ${data['time'].getTime()},
      ${data['0']['bme280']['temp']},
      ${data['0']['bme280']['pres']},
      ${data['0']['bme280']['humd']},
      ${data['0']['temp']},
      ${data['0']['light']},
      ${data['1']['bme280']['temp']},
      ${data['1']['bme280']['pres']},
      ${data['1']['bme280']['humd']},
      ${data['1']['temp']},
      ${data['1']['light']},
      ${data['2']['bme280']['temp']},
      ${data['2']['bme280']['pres']},
      ${data['2']['bme280']['humd']},
      ${data['2']['temp']},
      ${data['2']['light']}
    )
    `);
    db.close();
  };
  
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

  // current sensor reading
  var readings = {};

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

  ipcMain.handle('app-shutdown', async(event, ...args) => {
    app.quit();
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

      // parser.on('data', (data) => {
      //   console.log("Got data");
      //   console.log(data);
      //   let newReadings = JSON.parse(data);

      //   if ('com' in newReadings && newReadings.com === 'ALL') {
      //     newReadings['time'] = new Date();
      //     currentReadings = newReadings;
      //     storeDataToDB(newReadings);
      //   }
      //   /*
      //   if (comms[0] === 'ALL') {
      //     let newReadings = {};
          
      //     newReadings['time'] = new Date().toISOString();

      //     // parse the remaining input from the arduino
      //     for (let i = 1; i < comms.length; ++i) {
      //       let testCommand = comms[i];

      //       if (testCommand.startsWith('TMP')) {
      //         let sensorIdx = testCommand.split(':')[1];
      //         newReadings[sensorIdx] = comms[i + 1];
      //         ++i;
      //       }
      //     }
      //   }
      //   else if (comms[0] === 'PHVAL') {
      //     // console.log('Got here');
      //     mainWindow?.webContents?.send('ph', comms[1]);
      //   }
      //   else if (comms[0] === 'TEMP') {
      //     // console.log('Got TEMP');
      //     mainWindow?.webContents?.send('tmp', comms[1]);
      //   }
      //   */
      //   // mainWindow.webContents.send('ph', )
      // });
      
      // arduino.on("data", async (data) => {
      arduino.on("readable", async () => {
        // console.log('Data event');
        // console.log(data);

        data = arduino.read();
        buffer = Buffer.concat([buffer, data]);
        await processBuffer();
        // check if null terminator is found in data
        
      });
  
      // arduino.on("readable", () => {
      //   let s = arduino.read();
      //   console.log("Readable event");
      //   try {
      //    let newReadings = JSON.parse(s.toString());

      //   }
      //   catch {
      //     console.log("Cannot parse the string: ");
      //     console.log(s.toString());
      //   }

      //   if ('com' in newReadings && newReadings.com === 'ALL') {
      //     console.log("This has valid data!");
      //     newReadings['time'] = new Date();
      //     currentReadings = newReadings;
      //     storeDataToDB(newReadings);
      //   }
      //   else {
      //     console.log("Nah");
      //   }
      //   console.log(s.toString());
      //   console.log("Done\n");


      // });
      
      mainWindow?.webContents?.send('serialport', {state: 'connected'});
      
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
          let s = JSON.stringify({com: 'ALL'}) + '\0';
          arduino.write(s);
          // arduino.write("PH\0");
          // arduino.write("TEMP\0");
        }
      }, 5000);
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

  ipcMain.handle('read-chamber', async(event, ...args) => {
    return currentReadings;
  });
  
  ipcMain.handle('arduino', async(event, obj) => {
    let s = JSON.stringify(obj) + '\0';
    console.log(s);
    if (arduino) {
      arduino.write(s);
      return true;
    }
    else {
      return false;
    }
  });
  
  ipcMain.handle('arduino-read', async(event, obj) => {
    let s = JSON.stringify(obj) + '\0';
    console.log(s);
    if (arduino) {
      arduino.write(s);
      return await arduino.read();
    }
    else {
      return false;
    }
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

}).catch(err => {
  console.log("Cannot run it because stuff is already running");
  console.log(err);
});

