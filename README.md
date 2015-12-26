itunes-win
=======================
[![npm version](https://badge.fury.io/js/itunes-win.svg)](https://badge.fury.io/js/itunes-win)

Control iTunes app for Windows.

## Requirement

- Windows
- Node.js
- [node-gyp](https://github.com/nodejs/node-gyp)

## Configure

```
$ node-gyp configure
```

## API

### `getCurrentTrack(callback)`
Get current playing track info.

#### `callback`: `function(err, track)`

```
err: null
track: { name: 'Dimension tripper!!!!',
  artist: 'nao',
  artwork:
   { format: 'JPEG',
     data: <Buffer ff d8 ff ... > } }
```
