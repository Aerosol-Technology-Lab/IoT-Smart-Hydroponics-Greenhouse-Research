import { React, useEffect, useState, useRef } from 'react';
import './Sysinfo.css'
const { ipcRenderer } = window.require('electron');
const disk = require('disk-space');

function Sysinfo() {

    const [diskInfo, setdiskInfo] = useState(null);

    useEffect(() => {

        window.diskinfo = disk;
        // disk.check('/', (err, info) => {
        //     console.log(err);
        //     console.log(info);
        //     // var data = { free: info.free, total: info.total };
        //     window.diskinfo = info;
        //     setdiskInfo(info);
        // });
    }, [])
    
    return(
        <div>
            {
                (diskInfo === null) ?
                <p>This is null!</p>
                :
                <p>This is not null</p>
            }
        </div>
    );
}

export default Sysinfo;
