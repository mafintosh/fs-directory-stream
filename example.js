var createStream = require('./')

var path = __dirname

createStream(path)
  .on('data', data => console.log(data))
  .on('end', () => console.log('(no more data)'))
