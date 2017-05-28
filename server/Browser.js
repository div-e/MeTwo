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
