import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import reportWebVitals from './reportWebVitals';
import { BrowserRouter } from 'react-router-dom';
const { ipcRenderer } = window.require('electron');

window.addEventListener('keydown', (key) => {
  console.log(`Firing ${key.code}`)
  if (key.ctrlKey && key.shiftKey && key.code === 'KeyK') {
    
    window.addEventListener('keydown', (nextKey) => {
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
