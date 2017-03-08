# Controller

## Overview


This demo project is remote control the board

## Components

* Bootstrap - For styling

* Knockout - To bind the data to the controls on the page

* Socket.IO - To communicate with the sockets
* Paho MQ Client - To communicate with the MQ

## Install
Install Mosca
https://github.com/mcollina/mosca/wiki/Mosca-as-a-standalone-service.

Install MQTT
https://www.npmjs.com/package/mqtt#install

Copy the files to the board

Run Mosca with
 mosca -v --http-port 1884 --only-http

Run webserver with
./server.js

Run musicController with
./MusicListener.js

Navigate to web page (don't use Edge)
http://beaglebone.lan:9090/controller.html

