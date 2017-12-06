# fs-directory-stream

Streaming fs.readdir.

```
npm install fs-directory-stream
```

[![Build Status](https://travis-ci.org/mafintosh/fs-directory-stream.svg?branch=master)](https://travis-ci.org/mafintosh/fs-directory-stream)

Useful if your directory contains a ton of directories/files and you don't want to read them all into ram at once.

## Usage

``` js
var createDirectoryStream = require('fs-directory-stream')

createDirectoryStream('/home/maf/dev/node_modules')
  .on('data', data => console.log(data))
  .on('end', () => console.log('(no more data)'))
```

Uses [uv_fs_scandir_next](http://docs.libuv.org/en/v1.x/fs.html?highlight=scandir_next#c.uv_fs_scandir_next) behind the scenes to perform the
streaming readdir.

## API

#### `var readableStream = createDirectoryStream(path)`

Make a new stream of directory entries. Will emit an error if `path` doesn't exist, etc.

Every `data` event is the string name of a directory entry.

## License

MIT
