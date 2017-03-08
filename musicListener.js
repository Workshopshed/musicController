#!/usr/bin/env node

// Listens for messages on a queue and plays notes

var mqtt = require('mqtt')
var b = require('bonescript');

console.log('Configuring LEDs')

var leds = ["USR0", "USR1", "USR2", "USR3"];

for(var i in leds) {
    b.pinMode(leds[i], b.OUTPUT);
}


console.log('Connecting to Queue')

var client = mqtt.connect('ws://localhost:1884');


client.on('connect', function () {
    client.subscribe('AGC_BBBL_Music/+/Commands')
})

client.on('message', function (topic, message) {
    // message is Buffer 
    if (message == 'PLAY1') {
	b.digitalWrite("USR0", 1);
	setTimeout(function() { b.digitalWrite("USR0", 0); }, 1000);	
    }
    if (message == 'PLAY2') {
	b.digitalWrite("USR1", 1);
	setTimeout(function() { b.digitalWrite("USR0", 0); }, 1000);	
    }
    if (message == 'PLAY3') {
	b.digitalWrite("USR2", 1);
	setTimeout(function() { b.digitalWrite("USR0", 0); }, 1000);	
    }
    if (message == 'PLAY4') {
	b.digitalWrite("USR3", 1);
	setTimeout(function() { b.digitalWrite("USR0", 0); }, 1000);	
    }
    console.log(message.toString())
})



process.on('SIGINT', function () {
    console.log("Shutting down SIGINT (Ctrl-C)");
    // some other closing procedures go here
    client.end()
    process.exit();
})

function doStuff() {
    // code to run
    console.log("Heartbeat...");
};

function run() {
    console.log('Running')
    setInterval(doStuff, 30000);
};

run();

