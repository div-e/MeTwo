"use strict"

const WebSocket = require('ws');
const myPort = 8080;
const wsServer = new WebSocket.Server({port: myPort});
var ws;
var ws2; 

const myPort2 = 8081;
const wsServer2 = new WebSocket.Server({port: myPort2}); 

var express = require('express');
var path = require('path');
var app = express();

app.use(express.static(__dirname));

app.get('/1', function(request, response) {
  response.sendFile('index.html', {root: path.join(__dirname, '.')});
});

app.get('/2', function(request, response) {
  response.sendFile('index2.html', {root: path.join(__dirname, '.')});
});

app.listen(3000, function() {
  console.log("listening on port 3000");
});

wsServer.on('connection', connectionHandler);
wsServer2.on('connection', connectionHandler2); 




// connectionHandler() sets up the event handlers for the websocket.
function connectionHandler(websocket) {
  ws = websocket; 
  websocket.on('message', messageHandler);
  websocket.on('close', printClosingStatement);
}

// messageHandler() prints out the given message to console.log.
function messageHandler(message) {
  console.log("message from client: " + message);
  ws2.send("message from client: " + message); 
}

// printClosingStatement() prints the closing statement. Call when closing ws.
function printClosingStatement() {
  console.log("closing socket!");
}




// connectionHandler2() to connect with the "robot" 
function connectionHandler2(websocket) {
  ws2 = websocket; 
  websocket.on('message', messageHandler2); 
  websocket.on('close', printClosingStatement2); 
}


// messageHandler2() prints out the given message to console.log.
function messageHandler2(message) {
  console.log("message from robot: " + message);
  ws.send("message from client: " + message); 
}

// printClosingStatement2() prints the closing statement. Call when closing ws.
function printClosingStatement2() {
  console.log("closing robot socket!");
}


