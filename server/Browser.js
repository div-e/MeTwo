"use strict"

/*
 * Browser class
 * created and used in index.js.
 * Connects functions to websocket events. 
 * takeThis() in the Browser class is intended for robot feedback to the browser (e.g. video feed)
 */
module.exports = class Browser {
    constructor(ws, broadcast, cleanUp) {
        ws.on('message', broadcast);
        ws.on('close', () => {
            cleanUp();
            console.log('Browser disconnected');
        })
        this.ws = ws;
        console.log('Browser connected');
    }

    takeThis(buffer) {
        this.ws.send(buffer);
    }
}
