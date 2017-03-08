/* Reference
https://blog.risingstack.com/getting-started-with-nodejs-and-mqtt/
https://www.npmjs.com/package/mqtt
https://github.com/mcollina/mosca/wiki/MQTT-over-Websockets

*/

function musicController() {
    self = this;
    self.clientID = ko.observable("musicController" + parseInt(Math.random() * 100, 10));
    self.log = ko.observableArray();
    self.status = { name: ko.observable(self.clientID), status: ko.observable("ready") };
    self.Qcommands = "AGC_BBBL_Music/" + self.clientID() + "/Commands";
    self.server = ko.observable("beaglebone.lan");
    self.port = ko.observable(1884);

  //Buttons
  self.connect = function() {
    self.client.connect({onSuccess:self.onConnect});}
  self.disconnect = function () {
      self.setstatus("disconnected", true);
      self.client.disconnect();
      self.log.push("Disconnected");
      self.machines.removeAll();
  }
  self.play = function (note) {
      message = new Paho.MQTT.Message("PLAY" + note);
      message.destinationName = self.Qcommands;
      self.client.send(message);
  }

  self.setstatus = function (status, retained) {
      self.status.status = status;
  }

    // Create a client instance
    //MQTT on 1883, Sockets 1884
  self.client = new Paho.MQTT.Client(self.server(), Number(self.port()), self.clientID());

// set callback handlers
  self.client.onConnectionLost = function (responseObject) {
      // called when the client loses its connection
      if (responseObject.errorCode !== 0) {
          self.log.push("onConnectionLost:" + responseObject.errorMessage);
          self.setstatus(responseObject.errorMessage);
      }
  };

  function arrayObjectIndexOf(myArray, searchTerm, property) {
      for (var i = 0, len = myArray.length; i < len; i++) {
          if (myArray[i][property] === searchTerm) return i;
      }
      return -1;
  }

    //This bit would be hosted in the Node.JS service on the board.
  self.client.onMessageArrived = function (message) {
      self.log.push(message.destinationName);
      self.log.push(message.payloadString);
  };

// called when the client connects
  self.onConnect = function() {
      // Once a connection has been made, make a subscription and send a message.
      self.setstatus("Connected");
    self.log.push("Connected");
    // Note that a machine would just connect to run and a monitor would just connect to status.
    self.client.subscribe(self.Qcommands);
  }

}

ko.applyBindings(new musicController());
