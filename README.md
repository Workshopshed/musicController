# Controller

## Overview

This demo project remote controls some servos from a web page

![System Diagram](SystemDiagram.png)

## Client Components

* Bootstrap - For styling
* Knockout - To bind the data to the controls on the page
* Socket.IO - To communicate with the sockets
* Paho MQ Client - To communicate with the MQ

## Server Components
* Mosca - An MQTT broker
* MQTT - The MQTT client for node

## Install
Install Mosca
https://github.com/mcollina/mosca/wiki/Mosca-as-a-standalone-service.

Install MQTT
https://www.npmjs.com/package/mqtt#install

Copy the files to the board

Run Mosca with
 mosca -v --http-port 1884 --only-http

Run webserver with
./webServer.js

Run musicController with
./musicListener.js

Navigate to web page (don't use Edge)
http://beaglebone.lan:9090/controller.html

## Finished Glockenspiel ##

![Finished Glockenspiel](FinishedGlockenspiel.jpg)

