import React, { useState, useEffect } from 'react';
import './SettingsPage.css';
import { Link, useHistory } from 'react-router-dom'
const { ipcRenderer } = window.require('electron');
const { exec } = window.require('child_process')
// const { networkInterfaces } = window.require('os');

const SettingsPage = () => {

    /*
    const [ipAddress, setipAddress] = useState('');
    
    const findIPAddress = (functionSetter) => {

        const nets = networkInterfaces();
        const results = {};

        for (const name of Object.keys(nets)) {
          for (const net of nets[name]) {
            if (net.family === "IPv4" && !net.internal) {
              if (!results[name]) {
                results[name] = [];
              }
              results[name].push(net.address);
            }
          }
        }
        console.log(results)
        functionSetter(results['ham0']);
    }

    */
    
    const onClickShutdown = () => {
        ipcRenderer.invoke('app-shutdown');
    };

    const onClickUpdate = () => {
        exec('git pull', (error, stdout, sterr) => {
            if (error) {
                console.log(`error : ${error.message}`);
            }
            console.log(`output: ${stdout}`);
        });
    };
        
    useEffect(() => {
        window.setNavName?.('Settings');
        
        return () => {
            // window.removeNavName?.();
        }
    }, [])
    
    return (
        <div className='SettingsScene'>
            <div className='slideInFromRight'>
                <h1>About</h1>
                <h2>IoT Hydroponics Research</h2>
                <h3>BAEN - Biological & Agricultural Engineering</h3>
                <h3>Aerosol Lab</h3>
                <h3>Written by: Charlemagne Wong</h3>
                <div className='options'>
                    <button>Turn Off Rasbperry Pi</button>
                    <button>Restart Rasbperry Pi</button>
                    <Link to={'/update/'}>
                        <button>Update</button>
                    </Link>
                    <button onClick={ onClickShutdown }>Shutdown</button>
                    sfs
                </div>

            </div>
        </div>
    )
}

export default SettingsPage;
