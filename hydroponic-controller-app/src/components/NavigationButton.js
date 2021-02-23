import React, { useEffect } from 'react'
import { Link } from 'react-router-dom'
import './Navigation.css'
import './NavigationButton.css'

function NavigationButton({ id=undefined, href, icon, registerButton=undefined, unregisterButton=undefined, unfocusAll=undefined }) {
    var className = 'icon ' + icon;
    
    const onClick = () => {
        if (unfocusAll) unfocusAll();
        document.getElementById(id)?.classList.add('focus');
    }
    
    useEffect(() => {
        console.log(`My id is: [${id}] and class ${icon}`)
        if (registerButton) registerButton(id);
        
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
