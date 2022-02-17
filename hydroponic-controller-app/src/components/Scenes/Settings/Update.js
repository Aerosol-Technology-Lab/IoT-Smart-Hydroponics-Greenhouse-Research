import React, { useEffect } from 'react';
import { useHistory } from 'react-router'
const { exec } = window.require('child_process')

const Update = () => {
    
    console.log("Rendering Update component");
    
    let successCurrentBranch = false;
    let current_branch = null;
    exec('git branch --show-current', (error, stdout, stderr) => {
        if (error) {
            console.error(error.message);
        }

        if (stderr) {
            successCurrentBranch = false;
            return;
        }

        current_branch = stdout;
    });
    
    return(
    <p>HELLO WORLDs</p>
    );
};

export default Update;
