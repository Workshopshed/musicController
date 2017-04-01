#!/usr/bin/env node

// Listens for messages on a queue and plays notes

var mqtt = require('mqtt')
var servo = require('./servo.js');

var servos = [];

console.log('Configuring Servos')

var i;

for (i = 1; i <= 8; i++) {
    servos.push(new servo(i));
}

console.log('Connecting to Queue')

var client = mqtt.connect('ws://localhost:1884');

client.on('connect', function () {
    client.subscribe('AGC_BBBL_Music/+/Commands')
})

client.on('message', function (topic, message) {
    // message is Buffer
    var cmd = message.toString();
    if (cmd.substring(0, 4) == 'PLAY') {
         var channel = cmd.substring(4, 5);
	 playNote(channel);
    }
})

function playNote(channel) {
    console.log(channel);
    var servo = servos[channel - 1];
    servo.moveTo(-0.5,-0.5,10);
    servo.moveTo(-0.5,0.0,50);
    servo.moveTo(0.0,-0.5,100);
}

process.on('SIGINT', function () {
    console.log("Shutting down SIGINT (Ctrl-C)");
    client.end()
    process.exit();
})

function doStuff() {
    console.log("Heartbeat...");
};

function run() {
    console.log('Running')
    setInterval(doStuff, 30000);
};

run();

