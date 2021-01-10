const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const url = require('url');
const serialport = require('serialport');

const isDev = true ? require('electron-is-dev') : false;

let mainWindow;



function createWindow() {
  mainWindow = new BrowserWindow({
    width: 900, 
    height: 680,
    webPreferences: {
      nodeIntegration: true
    }
  });
  mainWindow.loadURL(isDev ? 'http://localhost:3000' : `file://${path.join(__dirname, '../build/index.html')}`);
  mainWindow.on('closed', () => mainWindow = null);
}

ipcMain.handle('my-invokable-ipc', async (event, ...args) => {
  return "hello"
})

ipcMain.handle('cwd', async (event, ...args) => {
  return process.cwd();
});

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (mainWindow === null) {
    createWindow();
  }
});