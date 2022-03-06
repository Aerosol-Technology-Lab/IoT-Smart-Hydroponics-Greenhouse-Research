
// defaults initialized for app globals
function initializeAppGlobals() {

    window.AppGlobal = { 
        app:            undefined,             // the main app element - initialized in App.js
        ipcRenderer:    undefined,             // ipcRenderer for frontend to backend communication - initialized in App.js
    };
}

function getGlobals() {
    return window.AppGlobal;
}

export default initializeAppGlobals;
