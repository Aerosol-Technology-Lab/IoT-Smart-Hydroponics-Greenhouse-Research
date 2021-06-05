import React, { useEffect } from 'react';
import { Route } from 'react-router';
import SettingsPage from './SettingsPage';
import Update from './Update';

const Settings = () => {

    useEffect(() => {
    }, [])
    
    return (
        <div>
            <Route path='/update' exact component={Update} />
            <Route path='/settings' exact component={SettingsPage} />
        </div>
    )
}

export default Settings;
