import React, { useEffect } from 'react';
import { Route } from 'react-router';
import SettingsPage from './SettingsPage';
import Update from './Update';

const Settings = () => {

    const divStyle = {
        width: '100%',
        height: '100%',
        'background-color': 'red'
    }
    
    useEffect(() => {
    }, [])
    
    return (
        // <div style={ divStyle }>
            // <Route path='/update' exact component={Update} />
            <Route path='/settings' exact component={SettingsPage} />
        // </div>
    )
}

export default Settings;
