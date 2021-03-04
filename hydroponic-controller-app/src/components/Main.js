import React from 'react';
import { Route, Switch, Redirect } from 'react-router-dom'
import './Main.css';
// import Clock from './Clock'
import NotificationBar from './Notifications/NotificationBar'
import Scene from './Scenes/Scene';
import SerialTest from './test/SerialTest'
import Test from './Scenes/Test'
import Settings from './Scenes/Settings/Settings'
import App from '../App'

const { ipcRenderer } = window.require('electron');

class Main extends React.Component {
    constructor(props) {
        super(props);
        this.Scenes = {
            SerialTest: SerialTest
        }

        this.state = {
            Scene: this.Scenes['SerialTest'],
            SceneID: 'SerialTest'
        };

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
                {/* < Scene scene={ this.state.Scene } /> */}
                <Switch >
                    <Route exact path='/dashboard' component={ this.Scenes.SerialTest } />
                    <Route exact path='/settings' component={ Settings } />
                    <Route exact path='/'>
                        <Redirect to='/dashboard' />
                    </Route>
                    {/* <Route exact path='/' component={ this.state.Scene } /> */}
                </Switch>
                {/* <SerialTest /> */}
                {/* <Scene Scene={ this.state.Scene } /> */}
                {/* { this.state.Scene } */}
            </main>
        );
    }
}

export default Main;
