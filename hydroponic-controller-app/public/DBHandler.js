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
                        cdb.run('INSERT INTO Info(key, value) VALUES ("version", "0.1")');
                    });

                    cdb.close();
                }
            })
            
            db.run(`CREATE TABLE IF NOT EXISTS Logs (
                time NUMERIC PRIMARY KEY,
                log TEXT NOT NULL
            )`);
            db.prepare(`INSERT INTO Logs(time, log) VALUES(?, ?)`).run([new Date().toISOString(), 'Opened DB']).finalize();
            
            // SELECT * from logs where DATE(time) between DATE('2021-02-21T06:20:03.333Z') and '2022';
            
            db.prepare(`CREATE TABLE IF NOT EXISTS Sensors (
                time TEXT PRIMARY KEY,
                tmp1 REAL,
                tmp2 REAL,
                tmp3 REAL,
                ph REAL
            )`).run().finalize();

            db.close();
        });
    }
}
