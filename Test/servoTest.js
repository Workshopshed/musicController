#!/usr/bin/env node

var Servo = require('../servo.js');

var servos = [];

for (i=1; i < 8; i++) {
    var x = new Servo(i);
    servos.push(x);
}

console.log(servos[0].pipe);

for (s in servos) {
    console.log(servos[s].pipe);
    servos[s].position(10);
}
