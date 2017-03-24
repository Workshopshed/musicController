var fs = require("fs")

var Servo = function(channel) {
  this.channel = channel;
  this.pipe = '/var/servoDaemon/servo' + channel;

  this.position = function(pos) {
         fs.open(this.pipe, 'w', (err, fd) => {
	fs.write(fd,pos);
	});
       };
};
module.exports = Servo;