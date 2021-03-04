const sql3 = require('sqlite3').verbose();

const DEFAULT_CALLBACK = (err) => {
    if (err) {
        return console.error(err);
    }
};

module.exports = class DBHandler {
    constructor(dbpath=':memory:', creation_callback=DEFAULT_CALLBACK) {
        this.filepath = dbpath;
        var db = new sql3.Database(this.filepath, creation_callback);
        db.serialize(() => {
            db.prepare(`CREATE TABLE IF NOT EXISTS Info (
                key TEXT PRIMARY KEY,
                value BLOB
            )`).run().finalize();
            
            let versionQuery = `SELECT key, value FROM Info WHERE key = "version"`;
            db.get(versionQuery, (err, row) => {
                if (err) {
                    return console.error(err.message);
                }
                
                if (!row) {
                    let cdb = new sql3.Database(this.filepath);
                    cdb.serialize(() => {
                        cdb.run('INSERT INTO Info(key, value) VALUES ("version", "0.2")');
                    });

                    cdb.close();
                }
            })
            
            db.run(`CREATE TABLE IF NOT EXISTS Logs (
                time NUMERIC PRIMARY KEY,
                log TEXT NOT NULL
            )`);
            db.prepare(`INSERT INTO Logs(time, log) VALUES(?, ?)`).run([new Date().toISOString(), 'Starting DBHandler']).finalize();
            
            // SELECT * from logs where DATE(time) between DATE('2021-02-21T06:20:03.333Z') and '2022';
            
            db.prepare(`CREATE TABLE IF NOT EXISTS Sensors (
                time TEXT PRIMARY KEY,
                tmp1 REAL,
                tmp2 REAL,
                tmp3 REAL
            )`).run().finalize();

            db.close();
        });
    }

    getDBInstance() {
        return new sql3.Database(this.filepath);
    }

    async storeReading(readings, creation_callback=DEFAULT_CALLBACK) {
        if (!('template' in readings)) {
            return false;
        }

        var db = new sql3.Database(this.filepath, creation_callback);
        db.serialize(() => {
            let insertion = `INSERT INTO Sensors (time, tmp1, tmp2, tmp3) VALUES('${readings.time}', ${readings.temperature[0]}, ${readings.temperature[1]}, ${readings.temperature[2]})`
            console.log();
            db.run(insertion);
            db.close();
        })

    }

    async log(message, creation_callback=DEFAULT_CALLBACK) {
        var db = new sql3.Database(this.filepath, creation_callback);
        db.serialize(() => {
            db.prepare('INSERT INTO Logs(time, log) VALUES(?, ?)').run([new Date().toISOString(), message]).finalize();
            db.close();
        });
    }
    
    static getTemplate() {
        return {
            template: 'TEMPLATE_READING',
            time: '',
            temperature: {
              '0': '',
              '1': '',
              '2': ''
            },
          }
    }
}
