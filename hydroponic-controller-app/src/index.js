import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import reportWebVitals from './reportWebVitals';
import { BrowserRouter } from 'react-router-dom';
const { ipcRenderer } = window.require('electron');

// global key listener
window.addEventListener('keydown', (key) => {
  
  // Ctrl + K + <Another Key>   Command keys
  if (key.ctrlKey && key.shiftKey && key.code === 'KeyK') {
    
    // Listener for next stage of command
    window.addEventListener('keydown', (nextKey) => {
      // shutdowns the app
      if (nextKey.ctrlKey && nextKey.shiftKey && nextKey.code === 'KeyC') {
        ipcRenderer.invoke('app-shutdown');
      }

    }, {once: true});
  }
});

ReactDOM.render(
  <React.StrictMode>
    <BrowserRouter>
      <App />
    </BrowserRouter>
  </React.StrictMode>,
  document.getElementById('root')
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals
reportWebVitals();
