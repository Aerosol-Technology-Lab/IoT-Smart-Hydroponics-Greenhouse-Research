import React, { useState, useEffect } from 'react';
import './App.css';
import Navigation from './components/Navigation';
import Main from './components/Main';
import Modal from './components/GlobalModal';
import { initializeAppGlobals, getGlobals } from './AppGlobals';

const { ipcRenderer } = window.require('electron');
// const serialport = window.require('serialport');


function App() {
  
  const [className, setClassName] = useState("App")
  
  // checks if cursor should be hidden
  useEffect(() => {

    // default globals that can be used by all React components
    initializeAppGlobals();
    const AppGlobal = getGlobals();
    
    // initialize default app globals
    AppGlobal.app = document.getElementById('App');
    
    ipcRenderer.invoke('enable-cursor').then((res) => {
      if (!res) {
        setClassName(className + ' no-cursor');
      }
    });

    
    window.addEventListener('keydown', (key) => {
      if (key.ctrlKey && key.shiftKey && key.code === 'KeyM') {
        let app = document.getElementById('App');
        app.classList.toggle('no-cursor');

      }

      return () => {
        // removes global safe space
        window.app_globals = null;
      }
    });

    window.globalIPCRenderer = ipcRenderer;

    return () => {
      window.globalIPCRenderer = undefined;
    }
  // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);
  
  return (
    <div id='App' className={className}>
      <Modal/>
      <Navigation />
      <Main />
    </div>
  );
}

export default App;
