import React, { Component } from 'react'
import PropTypes from 'prop-types'
import './NotificationIcon.css'

export class NotificationIcon extends Component {
    constructor({ icon, functionHandler, disabled }) {
        super({ icon, functionHandler, disabled });
        this.state = {
            disabled: disabled,
        }
    }
    
    render() {
        console.log('I am rendering');
        return (
            <div className="NotificationIcon disabled" disabled={true}>
                <h2>sfd</h2>
            </div>
        )
    }
}

export default NotificationIcon
