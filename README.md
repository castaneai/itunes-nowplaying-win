itunes-nowplaying-win
=======================
[![npm version](https://badge.fury.io/js/itunes-nowplaying-win.svg)](https://badge.fury.io/js/itunes-nowplaying-win)

Get nowplaying track from iTunes for Windows.

## Requirement

- Windows
- Node.js
- [node-gyp](https://github.com/nodejs/node-gyp)

## How to debug with VisualStudio

```bash
> node-gyp configure --debug  # generate Visual Studio solution
> start build\binding.sln  # launch Visual Studio
```

Open project properties and `Configuration Properties > Debugging`

- set `Command` to /path/to/node.exe
- set `Command Arguments` to `$(ProjectDir)..\example.js`


## API

### `getNowplaying(callback(err, track))`

### `saveNowplayingArtworkToFile(path, callback(err))`

For more details, see `example.js`