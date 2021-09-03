import { React, useEffect, useState } from 'react';
import './MainScene.css';
import './MainSceneCards/Chamber'
import Chamber from './MainSceneCards/Chamber';
const { ipcRenderer } = window.require('electron');

function MainScene()
{
    const [chamberReadings, setchamberReadings] = useState(undefined);
    const [updater, setupdater] = useState(null);

    useEffect(() => {
        
        // todo: This is bad. There is potential for multiple  calls to invoke
        let routine = setInterval(async () => {
            let reading = await ipcRenderer.invoke('read-chamber', undefined);
            console.log('Invoking!');
            console.log(reading);
            setchamberReadings(reading);
        }, 1000);
        
        window.updater = setupdater;
        window.setNavName?.('Main');
        
        return () => {
            clearInterval(routine);
        }
    }, [])
    
    var chambers = [undefined, undefined, undefined];
    if (chamberReadings !== undefined) {
        chambers = [chamberReadings[0], chamberReadings[1], chamberReadings[2]];
    }
    else {
        console.log('I don\'t have any data!');
    }
    
    return(
        <div className="MainScene main">
            <div className="box">
                { chambers.map((value, index) => {

                    {/* console.log(`This is the index: ${index}`); */}
                    {/* console.log(value); */}
                    {/* console.log(chamberReadings); */}

                    if (parseInt(value) !== NaN && value !== undefined) {
                        value.time = chamberReadings.time;
                        return(
                            <Chamber id={ index } object={ value }/>
                        )
                    }
                    else {
                        // nothing
                    }
                    
                })}
                {/* <Chamber id={0} contents="mello!"/> */}
                {/* <Chamber id={1} contents="mello1!"/> */}
                {/* <Chamber id={2} contents="mello2!"/> */}
                {/* Hello! */}
            </div>
        </div>
    )
}

export default MainScene;
