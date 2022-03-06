import { React, useEffect, useRef } from 'react';
import './Chamber.css'

function Chamber(props) {
    
    var contents = props.object;

    // window.contents = contents;
    // console.log("render!");
    
    var chamberData = {
        
        waterTemperature: '---',
        ambientLight:     '---',
        bme280: {
            temperature: '---',
            pressure:    '---',
            humidity:    '---',
        },
        ccs811: {
            eCO2: '---',
            tVOC: '---',
        },
    }
    
    if (contents !== undefined) {
        
        if (contents.temp !== undefined) {
            chamberData.waterTemperature = contents.temp.toFixed(1);
        }

        if (contents.light !== undefined) {
            chamberData.ambientLight = contents.light.toFixed(0);
        }

        if (contents.bme280 !== undefined) {

            if (contents.bme280.temp !== undefined) {
                chamberData.bme280.temperature = contents.bme280.temp.toFixed(1);
            }
            if (contents.bme280.pres !== undefined) {
                chamberData.bme280.pressure = contents.bme280.pres.toFixed(3);
            }
            if (contents.bme280.humd !== undefined) {
                chamberData.bme280.humidity = contents.bme280.humd.toFixed(1);
            }
        }

        if (contents.ccs811 !== undefined) {

            if (contents.ccs811.co2 !== undefined) {
                chamberData.ccs811.eCO2 = contents.ccs811.co2.toFixed(1);
            }
            if (contents.ccs811.tvoc !== undefined) {
                chamberData.ccs811.tVOC = contents.ccs811.tvoc.toFixed(1);
            }
        }
    }

    return(
        <div className="Chamber">
            <h1>Chamber <span>{props.id + 1}</span> </h1>
            <div className="environment">
                <h2>Water</h2>
                <div className='contents'>
                <p>
                    Temperature: { contents ? contents['temp'].toFixed(1) : '---' } &#176;F
                </p>
                    {/* <p>Turbidity: <span>---</span> </p> */}
                </div>
            </div>
            <div className="environment">
                <h2>Air</h2>
                <div className='contents'>
                    <p>Temperature: { chamberData.waterTemperature }&#176;F</p>
                    <p>CO2: { chamberData.ccs811.eCO2 } </p>
                    <p>tVOC: { chamberData.ccs811.tVOC } </p>
                    <p>Pressure: { contents ? contents['bme280']['pres'].toFixed(3) : '---' }kPA </p>
                    <p>Humidity: { contents ? contents['bme280']['humd'].toFixed(1) : '---' }%</p>
                    <p>Light: { contents ? contents['light'].toFixed(0) : '---' }%</p>
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
