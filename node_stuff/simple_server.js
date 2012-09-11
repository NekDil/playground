var http = require('http');

http.createServer(function (req, res) {
<<<<<<< HEAD
    res.writeHead(200, {'Content-Type': 'text/html'});
    res.end('<html><head><title>Hello World</title></head><body><h1>Hello World</h1></body></html>\n');
}).listen(1337, '10.239.66.73');

console.log('Server running on 10.239.66.73:1337');
=======
    res.writeHead(200, {'Content-Type': 'text/plain'});
    res.end('Hello World\n');
}).listen(1337, '127.0.0.1');

console.log('Server running on 127.0.0.1:1337');
>>>>>>> b65e8a0f1c6c75eb1ee20d2d7fd57b705e6bfc12
