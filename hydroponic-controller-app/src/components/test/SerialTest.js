import React from 'react';
import './SerialTest.css'
const { ipcRenderer } = window.require('electron');

class SerialTest extends React.Component {
    constructor(props) {
        super(props);
        console.log('Im here');
        this.state = { 
            serialState: null,
            tmp: 'none'
        };
        // this.init();

        ipcRenderer.removeAllListeners('serialport');
        // ipcRenderer.on('serialport', (event, args) => {
        //     window.arggs = args;
        //     console.log(args);
        //     this.init();
        // })

        ipcRenderer.on('ph', (event, args) => {
            console.log(`BLEH Stuff: ${args}`);
            this.setState({ph: args});
        });

        ipcRenderer.on('tmp', (event, args) => {
            console.log(`Got temp: ${args}`);
            this.setState({ tmp: args});
        })
    }

    async init() {
        this.setState({
            serialState: await ipcRenderer.invoke('serialport', {command: 'status'})
        });
        console.log(`SerialTest: The state of the serial port is [${this.state.serialState}]`);
    }

    // what was I thinking when I wrote this??
    // setValue(command, value) {
    //     this.setState({}[command] = value);
    // }

    componentDidMount() {
        console.log('I did mount!');
        window.setNavName?.('Dashboard');
    }
    
    componentWillUnmount() {

    }
    
    render() {
        let test = this.state.serialState;
        if (test == null) test = 'Undefined';
     
        let styles = {
            // background: 'red',
            color: '#121212',
            flex: 1,
        };
        
        let more = {
            // width: '90%',
            // height: '30px',
            // background: 'white',
            // 'border-radius': '5px',
            // 'justify-item': 'center'
        }
        
        return(
            <div className="SerialTest main" style={styles}>
                <div className="child" style={ more }>
                <p>Status of serial port: {test}</p>
                <p>PH: { this.state.ph }</p>
                <p>Water TEMP-1: { this.state.ph }</p>
                <p>Air TEMP-1: { this.state.tmp }</p>
                </div>
            </div>
        );
    }
}

export default SerialTest;
