"use strict"

var SerialPort = require('serialport');
var port = new SerialPort('/dev/ttyACM0', {
  baudRate: 9600
});

const WebSocket = require('ws');
const myPort = 8085;
const wsServer = new WebSocket.Server({port: myPort});
var express = require('express');
var path = require('path');
var app = express();

app.use(express.static(__dirname));

app.get('/', function(request, response) {
  response.sendFile('index.html', {root: path.join(__dirname, '.')});
});

app.listen(3000, function() {
  console.log("listening on port 3000");
});

wsServer.on('connection', connectionHandler);


// connectionHandler() sets up the event handlers for the websocket.
function connectionHandler(websocket) {
  websocket.on('message', messageHandler);
  websocket.on('close', printClosingStatement);
}

// messageHandler() prints out the given message to console.log.
function messageHandler(message) {
  console.log("message from client: " + message);
  port.write(message, function(err) {
    if(err) {
      console.log(err);
    } else {
      console.log('message written to arduino');
    }
  })
}

// printClosingStatement() prints the closing statement. Call when closing ws.
function printClosingStatement() {
  console.log("closing socket!");
}
