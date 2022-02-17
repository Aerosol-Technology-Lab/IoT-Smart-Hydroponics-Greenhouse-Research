import React, { Component } from 'react'
import PropTypes from 'prop-types'
import '../../../style.css'
import '../NotificationIcon.css'
const { ipcRenderer } = window.require('electron');

class ArduinoState extends Component {
    constructor(props) {
        super(props);
        // this.init();
        this.state = {
            connected: 'disconnected',
        }
        
        ipcRenderer.removeAllListeners('serialport');
        ipcRenderer.on('serialport', (event, args) => {
            window.serialportargs = args;

        });
    }

    async init() {
        this.setState({
            serialState: await ipcRenderer.invoke('serialport', {command: 'status'})
        })
    }
    
    render() {
        if (this.state.connected === 'connected') {
            return (null)
        }
        
        var color = {};
        switch(this.state.connected) {
            case 'permission-error':
                color.color = 'yellow';
                break;
            case 'disconnected':
                color.color = 'red';
                break;
            default:
                color.color = 'orange';
        }
        
        return (
            <span className='icon icon-usb1 NotificationIcon' style={color}></span>
        )
    }
}

export default ArduinoState;
