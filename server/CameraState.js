'use strict';

const Signals = require('./Signals')

module.exports = class CameraState {
    constructor() {
        this.up = false
        this.down = false
    }

    isValid(flag) {
        if (flag === Signals.UP) {
            if (this.down || this.up) {
                return false
            }
            this.up = true
            return true
        } else if (flag === Signals.DOWN) {
            if (this.up || this.down) {
                return false
            }
            this.down = true
            return true
        }
        else if (flag === Signals.STOP_U) {
            if (this.up) {
                this.up = false
                return true
            }
            return false
        }
        else if (flag === Signals.STOP_D) {
            if (this.down) {
                this.down = false
                return true
            }
            return false
        }
    }

    handle(buffer, socket) {
        let flag = buffer[0]
        if (!this.isValid(flag)) {
            return
        }
        if (flag <= Signals.DOWN) {
            try {
                socket.write(buffer)
            }
            catch (e) {
                return
            }

            this.handler = setInterval(() => {
                try {
                    socket.write(buffer)
                }
                catch (e) {
                    clearInterval(this.handler)
                }
            }, 100)
        }
        else {
            clearInterval(this.handler)
        }
    }
}
