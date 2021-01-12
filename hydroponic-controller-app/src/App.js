import React from 'react';
import './App.css';
import Navigation from './components/Navigation';
import Main from './components/Main'

const { ipcRenderer } = window.require('electron');
// const serialport = window.require('serialport');


function App() {
  
  return (
    <div className="App">
      <Navigation />
      <Main />
    </div>
  );
}

export default App;
