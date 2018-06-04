"use strict"

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
}

// printClosingStatement() prints the closing statement. Call when closing ws.
function printClosingStatement() {
  console.log("closing socket!");
}
