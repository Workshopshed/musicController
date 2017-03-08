/* 
https://www.npmjs.com/package/seq-queue

npm install seq-queue */



var seqqueue = require('seq-queue');
 
var queue = seqqueue.createQueue(5000);
 
queue.push(
  function(task) {
    console.log('Running task 1');
    setTimeout(function() {
      console.log('hello ');
      task.done();
    }, 2000);
  }, 
  function() {
    console.log('task timeout');
  }, 
  3000
);
 
queue.push(
  function(task) {
    console.log('Running task 2');
    setTimeout(function() {
      console.log('world~');
      task.done();
    }, 1500);
  }
);