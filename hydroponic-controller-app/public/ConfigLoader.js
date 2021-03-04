const fs = require('fs');
const path = require('path');

const DEFAUTL_CONFIG_PATH = './DefaultConfig.json';

class ConfigLoader
{
    constructor(filepath='./Config.json') {
        this.filepath = filepath;
    }

    /**
     * Checks if file exists
     * Note: I know this is inefficient, but it is only used once
     */
    exists() {
        try {
            require(this.filepath);
            return true;
        }
        catch (err) {
            return false;
        }
    }
    
    load() {
        if (this.exists()) {
            return require(this.filepath);
        }
        else {
            let data = require(DEFAUTL_CONFIG_PATH);
            this.write(data);
            return data;
        }
    }

    async write(data, callback=()=>{}) {
        if (typeof data == 'object') {
            data = JSON.stringify(data, null, 4);
        }
        fs.writeFile(path.join(__dirname, this.filepath), data, callback);
    }
}

module.exports = ConfigLoader;
