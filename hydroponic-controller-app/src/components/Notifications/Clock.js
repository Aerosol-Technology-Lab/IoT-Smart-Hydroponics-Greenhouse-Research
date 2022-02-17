import React from 'react';
import './Clock.css'
const { ipcRenderer } = window.require('electron');

class Clock extends React.Component {
    constructor(props) {
        super(props);
        this.getTime();
    }    

    async getTime() {
        this.referenceDate = await ipcRenderer.invoke('get-time');
        this.recordedAt = Date.now();
        setTimeout(this.firstTick.bind(this), 1000 - this.referenceDate.getMilliseconds());
    }
    
    componentDidMount() {
        this.getTime();
    }

    componentWillUnmount() {
        if (this.clockInterval) {
            clearInterval(this.clockInterval);
            this.clearInterval = null;
        }
    }

    firstTick() {
        setInterval(this.tick.bind(this), 1000);
        this.tick();
    }
    
    tick() {
        let delta = Date.now() - this.recordedAt;
        let newDate = new Date(this.referenceDate / 1 + delta);
        this.setState({ date: newDate });
    }
    
    render() {
        let time;
        if (this.state === null) {
            time = '--:--';
        }
        else {
            let hour = this.state.date.getHours() % 12;
            hour = hour ? hour : 12;
            let minutes = this.state.date.getMinutes();
            let seconds = this.state.date.getSeconds();
            let ampm = this.state.date.getHours() < 12 ? 'am' : 'pm';

            if (hour === 0) hour = 12;
            if (minutes < 10) minutes = '0' + minutes;
            if (seconds < 10) seconds = '0' + seconds;
            
            time = `${hour}:${minutes} ${ampm}`;
            // time = this.state.date.toString();
        }
        
        return(
            <div className='Clock'>{time}</div>
        );
    }
}

export default Clock;
