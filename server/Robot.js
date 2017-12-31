"use strict" 

// Controletate for handling and mapping buffer to the robot
const ControlState = require('./ControlState');

module.exports = class Robot {
    /* Constructor connects handlers to appropriate functions */ 
    constructor(socket, broadcast, cleanUp) {
        this.disconnectHandler = this.disconnectHandler.bind(this);
        this.messageHandler = this.messageHandler.bind(this);
        this.concatBuffer = this.concatBuffer.bind(this);

        this.frame = Buffer.allocUnsafe(40000);
        this.frameLength = 0;
        this.copied = 0;
        this.restBuffer = null;

        this.conState = new ControlState();
        this.socket = socket;
        this.broadcast = broadcast;
        this.cleanUp = cleanUp;

        socket.on('close', this.disconnectHandler);
        socket.on('data', this.messageHandler);
        console.log('Robot connected');
    }

    /* takeThis() handles buffers sent from browser to robot */
    takeThis(buffer) {
        this.conState.handle(buffer, this.socket);
    }

    disconnectHandler() {
        this.cleanUp();
        console.log('Robot disconnected');
    }

    /* WIP For Video streaming */
    messageHandler(buffer) {
        if (this.restBuffer != null) {
            concatBuffer(this.restBuffer);
            this.restBuffer = null;
        }
        this.concatBuffer(buffer);

        if (this.frameLength == this.copied) {
            this.broadcast(this.frame.slice(0, this.frameLength));
        }
    }

    /* WIP For Video streaming */
    concatBuffer(buffer) {
        if (this.frameLength == this.copied) {
            this.frameLength = buffer.readUInt16BE(0);
            this.copied = 0;
            buffer = buffer.slice(2);
        }

        let slice = this.frame.slice(this.copied, this.frameLength);
        if (buffer.length <= this.frameLength - this.copied) {
            let len = buffer.length;
            buffer.copy(slice);
            this.copied += len;
            this.restBuffer = null;
        }
        else {
            let len = this.frameLength - this.copied;
            buffer.copy(slice, 0, 0, len);
            this.copied += len;
            this.restBuffer = buffer.slice(len);
        }
    }
}
