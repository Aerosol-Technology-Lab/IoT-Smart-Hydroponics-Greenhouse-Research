import React, { useState, useEffect, useRef } from 'react';
import './App.css';
import Navigation from './components/Navigation';
import Main from './components/Main';
import Modal from './components/GlobalModal';

const { ipcRenderer } = window.require('electron');
// const serialport = window.require('serialport');


function App() {
  
  const [className, setClassName] = useState("App")
  
  // checks if cursor should be hidden
  useEffect(() => {
    window.application = document.getElementById('App');
    
    ipcRenderer.invoke('enable-cursor').then((res) => {
      if (!res) {
        setClassName(className + ' no-cursor');
      }
    })

    
    window.addEventListener('keydown', (key) => {
      if (key.ctrlKey && key.shiftKey && key.code === 'KeyM') {
        let app = document.getElementById('App');
        app.classList.toggle('no-cursor');

        // a safespace for app globals
        window.app_globals = {};
      }

      return () => {
        // removes global safe space
        window.app_globals = null;
      }
    })
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
