"use strict"

const WebSocket = require('ws');
const myPort = 8080;
const wsServer = new WebSocket.Server({port: myPort});

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
