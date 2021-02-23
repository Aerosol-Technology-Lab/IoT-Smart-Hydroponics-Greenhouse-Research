import React, { useRef, useState, useEffect } from 'react';
import './Navigation.css';
import '../style.css'
import './NavigationButton'
import NavigationButton from './NavigationButton';
const { ipcRenderer } = window.require('electron');

function Navigation(props) {
    const [debugView, setDebugView] = useState(false);
    const debug = useRef(0);

    useEffect(() => {
        debug.current = false;
        window.addEventListener('keydown', (key) => {
            if (key.ctrlKey && key.shiftKey && key.code === 'KeyD') {
                setDebugView(!debug.current);
                debug.current = !debug.current;
                console.log('stuff');
            }
        });

        return () => {

        };
    }, []);
    
    console.log('Rerender');
    
    return(
        <div className={"Navigation"}>
            { debugView ? <span className='icon icon-bug'></span> : null }
            <NavigationButton href='./' icon='icon-usb'/>
            <span className='icon icon-dashboard'></span>
            <span className='icon icon-stats-bars'></span>
            <span className='icon icon-usb'></span>
            <span className='icon icon-cog'></span>
        </div>
    );
};

export default Navigation;
