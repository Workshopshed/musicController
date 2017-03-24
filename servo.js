var fs = require("fs")
var queue = require('queue');
 
var Servo = function(channel) {
  this.channel = channel;
  this.pipe = '/var/servoDaemon/servo' + channel;

  this.position = function(pos) {
         fs.open(this.pipe, 'w', (err, fd) => {
	fs.write(fd,pos);
	});
       };

  this.q = queue();
  q.concurrency = 1;

  this.moveTo = function(pos,time) {
     position(pos);
     q.push(function(done) {
     setTimeout(function() {
	done();
     }, time);
});

  }

   this.run = function() {
   	q.start(function(err) {
  	        console.log('all done:');
	});
   }
};
module.exports = Servo;