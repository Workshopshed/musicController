//From https://www.npmjs.com/package/queue
//For sending motion commands to the servos

var queue = require('queue');
 
var q = queue();
q.concurrency = 1;

// add jobs using the Array API 
 
q.on('timeout', function(next, job) {
  console.log('job timed out:', job.toString().replace(/\n/g, ''));
  next();
});
 
q.push(function(done) {
  setTimeout(function() {
    console.log('slow job 1');
    done();
  }, 3000);
});
 
q.push(function(done) {
  setTimeout(function() {
    console.log('slow job 2');
    done();
  }, 3000);
});

q.push(function(done) {
  setTimeout(function() {
    console.log('slow job 3');
    done();
  }, 2000);
});
 
// get notified when jobs complete 
 
q.on('success', function(result, job) {
  console.log('job finished processing:');
});
 
// begin processing, get notified on end / failure 
 
q.start(function(err) {
  console.log('all done:');
});