dbh = require('./DBHandler');
d = new dbh('test.db');
j = dbh.getTemplate();

j.time = new Date().toISOString();
j.temperature['0'] = 48.35;
j.temperature['1'] = 48.235;
j.temperature['2'] = 68.35;

// d.storeReading(j);
