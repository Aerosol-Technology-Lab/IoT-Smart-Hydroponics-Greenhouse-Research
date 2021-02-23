import React from 'react'
import { Link } from 'react-router-dom'
import './Navigation.css'
import './NavigationButton.css'

function NavigationButton({href, icon}) {
    var className = 'icon ' + icon;
    
    return (
        <Link to={href} className='NavigationButton'>
            <span className={ className } ></span>
        </Link>
    )
}

export default NavigationButton
