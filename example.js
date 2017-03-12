const tempfile = require('tempfile');
const itunes = require('./itunes-win');

itunes.getNowplaying((err, track) => {
    if (err) {
        console.error(err);
        return;
    }

    console.log("track:", track);

    if (track.artworkCount > 0) {
        const tmpArtworkPath = tempfile(`.${track.artworkFormat}`);
        itunes.saveNowplayingArtworkToFile(tmpArtworkPath, (err) => {
            if (err) {
                console.error(err);
                return;
            }
            console.log(`save artwork to: ${tmpArtworkPath}`);
        });
    }
});
