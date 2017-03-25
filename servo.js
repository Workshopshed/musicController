var fs = require("fs")
var queue = require('queue');
 
var Servo = function(channel) {

  this.channel = channel;
  this.pipe = '/var/servoDaemon/servo' + channel + '/position';
  this.curPos = 0;

  this.position = function(pos) {
      fs.open(this.pipe, 'w', (err, fd) => {
          if (err == null) {
            fs.write(fd,pos);
            this.curPos = pos;
          }
          else {
            console.log(err);
          }
  	  });
  };

  this.q = queue();
  this.q.concurrency = 1;

  //Move from A to B in duration T
  this.moveTo = function(startPos,endPos,time) {
     var steps = time / 10;
     var stepSize = (endPos - startPos) / steps;
     var stepDur = time / steps;
     var servo = this;

     var i;
     for (i = 0; i < steps ; i++) {
        (function(pos) {
            servo.q.push(function(done) {
                setTimeout(function() {
                        servo.position(pos);
                        done();
                    }, stepDur);
                });
        })(startPos + (i * stepSize));
	 };

     this.run();
  }

   this.run = function() {
       this.q.start(function(err) {
          if (err != null) {
            console.log(err);
          }
	});
   }
};
module.exports = Servo;