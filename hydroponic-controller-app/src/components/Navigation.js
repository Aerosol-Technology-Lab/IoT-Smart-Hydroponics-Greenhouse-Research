import React, { useRef, useState, useEffect } from 'react';
import './Navigation.css';
import '../style.css'
import './NavigationButton'
import NavigationButton from './NavigationButton';
const { ipcRenderer } = window.require('electron');

function Navigation({ id=undefined }) {
    const [debugView, setDebugView] = useState(false);
    const debug = useRef(0);
    const buttonIDs = useRef([]);

    const addButtonFocus = (id) => {
        if (!(id in buttonIDs.current)) {
            console.log(`Registering id: ${id}`);
            buttonIDs.current.push(id);
            console.log(`button IDs are ${buttonIDs.current}`);
            return true;
        }
        return false;
    }

    const removeButtonFocus = (id) => {
        if (id in buttonIDs.current) {
            for (let i = 0; i < buttonIDs.current.length; ++i) {
                if (buttonIDs[i] === 'id') {
                    buttonIDs.current.splice(i, 1);
                    return true;
                }
            }
        }
        return false;
    }

    const unfocusAll = () => {
        for (id in buttonIDs.current) {
            console.log(`Removing ${buttonIDs.current[id]}`);
            document.getElementById(buttonIDs.current[id]).classList.remove('focus');
        }
    }
    
    useEffect(() => {
        debug.current = false;
        window.addEventListener('keydown', (key) => {
            if (key.ctrlKey && key.shiftKey && key.code === 'KeyD') {
                setDebugView(!debug.current);
                debug.current = !debug.current;
                console.log('stuff');
            }
        });

        return () => {

        };
    }, []);
    
    console.log('Rerender');
    
    return(
        <div id={id} className={"Navigation"}>
            { debugView ? <NavigationButton id='debug-button' href='#' icon='icon-bug'/> : null }
            <NavigationButton registerButton={ addButtonFocus } unregisterButton={ removeButtonFocus } unfocusAll={ unfocusAll } id='dashboard-button' href='./' icon='icon-dashboard'/>
            <NavigationButton registerButton={ addButtonFocus } unregisterButton={ removeButtonFocus } unfocusAll={ unfocusAll } id='data-button' href='#' icon='icon-stats-bars'/>
            <NavigationButton registerButton={ addButtonFocus } unregisterButton={ removeButtonFocus } unfocusAll={ unfocusAll } id='export-button'href='#' icon='icon-usb'/>
            <NavigationButton registerButton={ addButtonFocus } unregisterButton={ removeButtonFocus } unfocusAll={ unfocusAll } id='settings-button'href='#' icon='icon-cog'/>
        </div>
    );
};

export default Navigation;
