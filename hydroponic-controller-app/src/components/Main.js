import React from 'react';
import './Main.css';
// import Clock from './Clock'
import NotificationBar from './NotificationBar';
import SerialTest from './test/SerialTest'

const { ipcRenderer } = window.require('electron');

class Main extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {
        return(
            <main id='Main'>
                {/* <Clock /> */}
                <NotificationBar />
                <SerialTest />
            </main>
        );
    }
}

export default Main;
