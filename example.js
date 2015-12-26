var itunes = require('./itunes-win');
itunes.getCurrentTrack(function(err, track) {
    console.log("err:", err);
    console.log("track:", track);
});