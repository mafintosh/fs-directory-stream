var binding = require('node-gyp-build')(__dirname)
var from = require('from2')

module.exports = createDirectoryStream

function createDirectoryStream (name) {
  var stream = from.obj(function (size, cb) {
    if (stream.struct) read(cb)
    else openAndRead(cb)
  })

  stream.struct = null
  stream.name = Buffer.alloc(Buffer.byteLength(name) + 1)
  stream.name.write(name)

  return stream

  function read (cb) {
    var next = binding.uv_fs_scandir_next(stream.struct)
    if (!next.length) return cb(null, null)
    for (var i = 0; i < next.length - 1; i++) stream.push(next[i])
    cb(null, next[next.length - 1])
  }

  function openAndRead (cb) {
    stream.struct = Buffer.allocUnsafe(binding.SIZEOF_FS_DIRECTORY_STREAM_T)
    binding.uv_fs_scandir(stream.struct, stream.name, function (err) {
      if (err) return cb(err)
      read(cb)
    })
  }
}
