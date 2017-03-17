#!/usr/bin/env node

// Test connection to Cayenne (note that Allocate blocks the port)
var mqtt = require('mqtt')
var data = 1;

console.log('Connecting to Queue');

var apiVersion = "v1";
var username = 'e49012a0-ab1c-11e6-839f-8bfd46afe676';
var clientId =  'f0379fb0-ab1c-11e6-a7c1-b395fc8a1540';

var rootTopic = [
  apiVersion,
  username,
  'things',
  clientId
].join('/');

var client = mqtt.connect('mqtt://mqtt.mydevices.com',{
    port: 1883,
    clientId: clientId,
    username: username,
    password: '25dd00111cff4ea6e6fb1c96c7227f3c082cbb6d',
    connectTimeout: 5000
});

client.on('connect', function () {
    console.log('Connected');
    client.subscribe(rootTopic + '/cmd/+');
    writeData();
})

client.on('message', function (topic, message) {
    const payload = message.toString().split(',');
    const topics = topic.toString().split('/');
    const seq = payload[0];
    const cmd = payload[1];
    const channel = topics[5];
    console.log(channel);
    console.log(seq);
    console.log(cmd);
    //v1/username/things/clientID/data/channel
    client.publish(rootTopic + '/data/' + channel, cmd); //Echo value back
    client.publish(rootTopic + '/response', 'ok,'+seq);
})

client.on('close', function (message) { 
    console.log('closed');
    console.log(message);
    client.end(); })
client.on('error', function (message) {
    console.log('error occurred');
    console.log(message);
    client.end(); })
client.on('disconnect', function () {
    console.log('disconnection occurred');
    client.end(); })

function writeData() {
    var topic = rootTopic + '/data/testChannel';
    var payload = data.toString();
    client.publish(topic, payload);
    data = data + 1;
}


process.on('SIGINT', function () {
    console.log("Shutting down SIGINT (Ctrl-C)");
    // some other closing procedures go here
    client.end();
    process.exit();
})

function doStuff() {
    // code to run
    console.log("Heartbeat...");
    writeData();
};

function run() {
    console.log('Running');
    setInterval(doStuff, 30000);
};

run();

