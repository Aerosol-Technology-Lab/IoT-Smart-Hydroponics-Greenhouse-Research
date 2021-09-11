import { React, useEffect, useRef } from 'react';
import './Chamber.css'

function Chamber(props) {
    
    var contents = props.object;

    // window.contents = contents;
    // console.log("render!");
    
    return(
        <div className="Chamber">
            <h1>Chamber <span>{props.id + 1}</span> </h1>
            <div className="environment">
                <h2>Water</h2>
                <div className='contents'>
                <p>
                    Temperature: { contents ? contents['temp'] : '---' } &#176;F
                </p>
                    {/* <p>Turbidity: <span>---</span> </p> */}
                </div>
            </div>
            <div className="environment">
                <h2>Air</h2>
                <div className='contents'>
                    <p>Temperature: { contents ? contents['bme280']['temp'] : '---' }&#176;F</p>
                    <p>CO2: <span>---</span> </p>
                    <p>Pressure: { contents ? contents['bme280']['pres'] : '---' }kPA </p>
                    <p>Humidity: { contents ? contents['bme280']['humd'] : '---' }%</p>
                    <p>Light: { contents ? contents['light'] : '---' }%</p>
                    {/* <p>Time: { contents ? contents['time']?.toString() : '---' }%</p> */}
                </div>
            </div>
            {/* <div className='graph-box'>
                <div className='graph'>
                    <p>place holder</p>
                </div>
            </div> */}
        </div>
    );
}

export default Chamber;
