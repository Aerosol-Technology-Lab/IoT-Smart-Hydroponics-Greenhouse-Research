import React, { useEffect } from 'react';
import './Settings.css';
const { ipcRenderer } = window.require('electron');

const Settings = () => {

    const onClickShutdown = () => {
        ipcRenderer.invoke('app-shutdown');
    }
    
    useEffect(() => {
        window.setNavName?.('Settings');
        
        return () => {
            // window.removeNavName?.();
        }
    }, [])
    
    return (
        <div className='SettingsScene'>
            <h1>About</h1>
            <h2>IoT Hydroponics Research</h2>
            <h3>BAEN - Biological & Agricultural Engineering</h3>
            <h3>Aerosol Lab</h3>
            <h3>Written by: Charlemagne Wong</h3>
            <div className='options'>
                <button>Turn Off Rasbperry Pi</button>
                <button>Restart Rasbperry Pi</button>
                <button>Update</button>
                <button onClick={ onClickShutdown }>Shutdown</button>
                sfs
            </div>
        </div>
    )
}

export default Settings;
