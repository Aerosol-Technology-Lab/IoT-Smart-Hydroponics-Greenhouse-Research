import React from 'react';
import './NotificationBar.css'
import Clock from './Clock';

class NotificationBar extends React.Component {
    constructor(props) {
        super(props);
    }

    render() {
        return (
            <div className="NotificationBar">
                <Clock />
            </div>
        );
    }
}

export default NotificationBar;
