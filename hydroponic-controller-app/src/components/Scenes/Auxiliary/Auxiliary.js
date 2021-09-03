import { React } from 'react';
import { render } from 'react-dom';
import './Auxiliary.css';
const { ipcRenderer } = window.require('electron');

function Auxiliary()
{
    const recordTurbidity = () => {

        ipcRenderer.invoke('arduino-turbidity', undefined);
    };

    return (
        <div className='auxiliary'>
            <button onClick={recordTurbidity}>Record Turbidity Value</button>
            <p>todo: improve UI</p>
        </div>
    );
};

export default Auxiliary;
