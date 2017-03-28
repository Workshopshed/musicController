#!/usr/bin/env node

// Listens for messages on a queue and plays notes

var mqtt = require('mqtt')
var servo = require('./servo.js');
var b = require('bonescript');

var servos = [];

console.log('Configuring Servos')

for (var i; i < 8; i++) {
    servos.push(new servo(i));
}

console.log('Configuring LEDs')

var leds = ["USR0"];

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
    
    if (message.substring(0,4) == 'PLAY') {
        var note = message.substring(4, 1); 
        
        //Flash light
        b.digitalWrite(leds[0], 1);
	    setTimeout(function() { b.digitalWrite("USR0", 0); }, 1000);	
        
        //Move servo

    }
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

