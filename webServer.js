#!/usr/bin/env node

"use strict";

var port = 9090,
    http = require('http'),
    url = require('url'),
    fs = require('fs'),

var server = http.createServer(servePage);

server.listen(port);
console.log("listening on " + port);

function servePage(req, res) {
    var path = url.parse(req.url).pathname;
    console.log("path: " + path);

    fs.readFile(__dirname + path, function (err, data) {
        if (err) {
            return send404(res);
        }
        res.write(data,'utf8');
        res.end();
    });
}

function send404(res) {
    res.writeHead(404);
    res.write('404 - page not found');
    res.end();
}
