import React from 'react';
import './Main.css';
// import Clock from './Clock'
import NotificationBar from './Notifications/NotificationBar'
import Scene from './Scenes/Scene';
import SerialTest from './test/SerialTest'
import Test from './Scenes/Test'
import App from '../App'

const { ipcRenderer } = window.require('electron');

class Main extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            Scene: SerialTest,
            SceneID: 'SerialTest'
        };
        this.Scenes = {
            Test: 'sdfs'
        }

        this.notificationBar = React.createElement(NotificationBar);
        App.notificationBar = this.notificationBar;
        // this.state.Scene = <SerialTest />
    }
    
    setScene(scene) {
        this.setState({ Scene: scene });
    }

    setSceneID(sceneID) {
        this.setState({ SceneID: sceneID });
    }
    
    render() {
        // const Scene = this.state.Scene;
        return(
            <main id='Main'>
                {/* <Clock /> */}
                { this.notificationBar }
                < Scene scene={ this.state.Scene } />
                {/* <SerialTest /> */}
                {/* <Scene Scene={ this.state.Scene } /> */}
                {/* { this.state.Scene } */}
            </main>
        );
    }
}

export default Main;
