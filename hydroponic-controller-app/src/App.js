import logo from './logo.svg';
import './App.css';
import react from 'react';

const fs = window.require('fs');
const { ipcRenderer } = window.require('electron');
// const serialport = window.require('serialport');

const cwdSet = null;
const processCwd = async (setCwd) => {
  const cwd = await ipcRenderer.invoke('cwd');
  setCwd(cwd);
}

function App() {
  const [cwd, setCwd] = react.useState(0);

  if (!cwdSet) {
    processCwd(setCwd);
  }
  
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <p>
          {cwd}
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
