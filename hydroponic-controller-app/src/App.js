import React, { useState, useEffect } from 'react';
import './App.css';
import Navigation from './components/Navigation';
import Main from './components/Main'

const { ipcRenderer } = window.require('electron');
// const serialport = window.require('serialport');


function App() {
  
  const [className, setClassName] = useState("App")
  
  // checks if cursor should be hidden
  useEffect(() => {
    ipcRenderer.invoke('enable-cursor').then((res) => {
      if (!res) setClassName(className + ' no-cursor');
    })
  }, []);
  
  return (
    <div className={className}>
      <Navigation />
      <Main />
    </div>
  );
}

export default App;
