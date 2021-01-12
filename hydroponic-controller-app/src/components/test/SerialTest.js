import React from 'react';
const { ipcRenderer } = window.require('electron');

class SerialTest extends React.Component {
    constructor(props) {
        super(props);
        console.log('Im here');
        this.state = { serialState: null };
        this.init();

        ipcRenderer.removeAllListeners('serialport');
        ipcRenderer.on('serialport', (event, ...args) => {
            console.log(args);
            this.init();
        })
    }

    async init() {
        this.setState({
            serialState: await ipcRenderer.invoke('serialport', {command: 'status'})
        });
        console.log(`SerialTest: The state of the serial port is [${this.state.serialState}]`);
    }
    
    componentWillUnmount() {

    }
    
    render() {
        let test = this.state.serialState;
        if (test == null) test = 'Undefined';
        
        return(
            <div>
                Status of serial port: {test}
            </div>
        );
    }
}

export default SerialTest;
