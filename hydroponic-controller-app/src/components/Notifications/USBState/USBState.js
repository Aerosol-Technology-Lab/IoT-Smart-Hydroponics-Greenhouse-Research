import React, { useEffect, useState } from 'react'
import '../../../style.css'
import '../NotificationIcon.css'
const { ipcRenderer } = window.require('electron');

const USBState = () => {
    const [connected, setConnected] = useState(false);
    
    useEffect(() => {
        
        ipcRenderer.on('usb', (event, args) => {
            console.log(`USB State: ${args}`);
            if (args === 'connected') {
                setConnected(true);
            }
            else {
                setConnected(false);
            }
        })

        // request for USB state
        ipcRenderer.invoke('usb').then((res) => {
            setConnected(res === 'connected' ? true : false);
        });
    }, [])
    
    if (connected) {
        return (
            <span className='icon icon-flash NotificationIcon'></span>
        )
    }
    
    return (
        null
    )
}

export default USBState

