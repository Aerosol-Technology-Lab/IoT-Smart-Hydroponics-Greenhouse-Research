import { React, useEffect, useState, useRef } from 'react';
import './Export.css';
import './Sysinfo.js';
import Sysinfo from './Sysinfo.js';
const { ipcRenderer } = window.require('electron');
const fs = window.require('fs');
const os = window.require('os');
const path = window.require('path');

function Export()
{
    const ignoreDirectories = [
        // os.userInfo().username,
        'root',
    ];
    const [directories, setDirectories] = useState([]);
    const mediaPath = useRef(`/media/${os.userInfo().username}`)

    const saveToUSB = async (file) => {
        const directoryPath = path.join(mediaPath.current, file, 'hydro.csv');
        await fs.unlink(directoryPath);
        fs.copyFile('./hydro.csv', directoryPath, (err) => {
            if (err) console.log('Cannot copy file');
        })
    };
    
    useEffect(() => {

        window.setNavName?.('Export');
        
        var watcher = setInterval(() => {
            
            fs.readdir(mediaPath.current, (err, files) => {
                if (!err) {
                    setDirectories(files);
                }
              });

        }, 1000);
          
        return () => {
            clearInterval(watcher);
        }
    }, [])
    
    var drives = [];
    directories.forEach(file => {
        if (!ignoreDirectories.includes(file)) drives.push(file);
    })
    
    return (
        <div className="Export">
            <div className="content">
                { 
                    drives.length ?
                    drives.map((file, index) => {

                        return (
                            <div className="drive" onClick={ () => {saveToUSB(file)} }>
                                <img className="usb" src="https://static.thenounproject.com/png/250776-200.png" />
                                <p className="filename">{file}</p>
                            </div>
                        )
                    })
                    :
                    <div className='nousb'>
                        <div className='nousb-icon'>
                            <img src='https://static.thenounproject.com/png/933129-200.png' />
                        </div>
                        <p className='nousbtext'>no USB drive found</p>
                    </div>
                }
            </div>
        </div>
    )
}

export default Export;
