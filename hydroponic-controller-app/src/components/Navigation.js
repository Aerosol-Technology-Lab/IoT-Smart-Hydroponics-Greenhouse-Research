import React from 'react';
import './Navigation.css';
import '../style.css'
class Navigation extends React.Component {
    render() {
        return(
            <div className={"Navigation"}>
                <span className='icon icon-dashboard'></span>
                <span className='icon icon-stats-dots'></span>
                <span className='icon icon-usb'></span>
                <span className='icon icon-cog'></span>
            </div>
        );
    }
};

export default Navigation;
