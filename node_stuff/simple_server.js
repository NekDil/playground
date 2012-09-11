
var http = require('http');

http.createServer(function (req, res) {
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end('<html><head><title>Hello World</title></head><body><h1>Hello World</h1></body></html>\n');
}).listen(1337, '10.239.66.73');

console.log('Server running on 10.239.66.73:1337');