import React, { useEffect } from 'react'
import { Link } from 'react-router-dom'
import './Navigation.css'
import './NavigationButton.css'

function NavigationButton({ id=undefined, href, icon, focus=false, registerButton=undefined, unregisterButton=undefined, unfocusAll=undefined }) {
    var className = 'icon ' + icon;
    
    const onClick = () => {
        if (unfocusAll) unfocusAll();
        document.getElementById(id)?.classList.add('focus');
    }
    
    useEffect(() => {
        if (registerButton) registerButton(id);
        if (focus) onClick();
        
        return () => {
            if (unregisterButton) unregisterButton(id);
        }
    }, [])
    
    return (
        <Link to={href} className='NavigationButton' onClick={onClick}>
            <span id={id} className={ className } ></span>
        </Link>
    )
}

export default NavigationButton
