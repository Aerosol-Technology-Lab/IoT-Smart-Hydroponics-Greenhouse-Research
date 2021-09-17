import { React } from 'react';
import { render } from 'react-dom';
import './Auxiliary.css';
const { ipcRenderer } = window.require('electron');

const PERIPHERERAL_SENSORS = [
    ['Turbidity',               'TURB' ],
    ['pH',                      'PH'   ],
    ['Total Dissolved Solids',  'TDS'  ],
    ['Electrical Conductivity', 'EC'   ],
];

function Auxiliary()
{
    const recordPeripheral = (type) => {

        ipcRenderer.invoke('arduino-turbidity', type);
    };

    return (
        <div className='auxiliary'>
            <div className='content'>
                <p>Sensor</p>
                <button className='calibrate' onClick={ () => {} }>Do Not Press</button>
                <button onClick={ () => {} }>Do Not Press</button>
            </div>
            {
                PERIPHERERAL_SENSORS.map(([sensorName, sensorCommand]) => {
                    return(
                        <div className='content'>
                            <p>{sensorName}</p>
                            <button className='calibrate' onClick={ () => {} }>Calibrate</button>
                            <button onClick={ () => recordPeripheral(sensorCommand) }>Record</button>
                        </div>
                    );
                })
            }
        </div>
    );
};

export default Auxiliary;
