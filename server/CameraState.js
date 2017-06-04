"use strict" 

const State = require('./State')
const Signals = require('./Signals')



module.exports = class CameraState extends State {
    constructor() {
        super()
        this.up = false
        this.down = false
    }



    validate(flag) {
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

        return false
    }



    handle(buffer, socket) {
        let flag = buffer[0]
        if (!this.validate(flag)) {
            return
        }

        if (flag <= Signals.DOWN) {
            Signals.map(buffer)
            super.redirect(buffer, socket)
            this.interval = setInterval(() => {
                super.redirect(buffer, socket)
            }, 100)
        }
        else {
            clearInterval(this.interval)
        }
    }



    close() {
        clearInterval(this.interval)
    }
}
