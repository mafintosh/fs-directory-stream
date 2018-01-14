var tape = require('tape')
var createDirectoryStream = require('./')
var fs = require('fs')

tape('streams a dir', function (t) {
  var expected = fs.readdirSync(__dirname)

  createDirectoryStream(__dirname)
    .on('data', function (data) {
      t.same(data, expected.shift(), 'same dir entry')
    })
    .on('end', function () {
      t.same(expected.length, 0, 'no more entries')
      t.end()
    })
})

tape('ENOENT', function (t) {
  createDirectoryStream('.PHONY')
    .on('error', function (err) {
      t.same(err.code, 'ENOENT')
      t.same(err.path, '.PHONY')
      t.end()
    })
    .resume()
})

tape('ENOTDIR', function (t) {
  createDirectoryStream(__filename)
    .on('error', function (err) {
      t.same(err.code, 'ENOTDIR')
      t.same(err.path, __filename)
      t.end()
    })
    .resume()
})

tape('running GC after creating a stream works fine', function (t) {
  createDirectoryStream(__dirname)
    .on('data', function dummy () {})
    .on('end', function () {
      setTimeout(function () {
        global.gc()
        global.gc(true)
        t.end()
      }, 10)
    })
})
