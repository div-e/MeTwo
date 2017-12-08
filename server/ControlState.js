"use strict"
const Signals = require('./Signals')
const State = require('./State')



module.exports = class ControlState extends State {
    constructor() {
        super()
        this.forward = false
        this.backward = false
        this.left = false
        this.right = false
    }



    validate(flag) {
        if (flag === Signals.FORWARD) {
            if (this.forward || this.backward) {
                return false
            }
            this.forward = true
            return true
        }
        else if (flag === Signals.BACKWARD) {
            if (this.backward || this.forward) {
                return false
            }
            this.backward = true
            return true
        }
        else if (flag === Signals.LEFT) {
            if (this.left || this.right) {
                return false
            }
            this.left = true
            return true
        }
        else if (flag == Signals.RIGHT) {
            if (this.right || this.left) {
                return false
            }
            this.right = true
            return true
        }
        else if (flag == Signals.STOP_F) {
            if (this.forward) {
                this.forward = false
                return true
            }
            return false
        }
        else if (flag == Signals.STOP_B) {
            if (this.backward) {
                this.backward = false
                return true
            }
            return false
        }
        else if (flag == Signals.STOP_L) {
            if (this.left) {
                this.left = false
                return true
            }
            return false
        }
        else if (flag == Signals.STOP_R) {
            if (this.right) {
                this.right = false
                return true
            }
            return false
        }
        // WTF IS WRONG WITH YOU
        //TODO ADD CASES FOR UP THROUGH STOP_PAN_LEFT IN SIGNALS.JS EXPORTS
        return false
    }



    handle(buffer, socket) {
      console.log(buffer[0])
        if (this.validate(buffer[0])) {
            Signals.map(buffer)
            super.redirect(buffer, socket)
        }
    }
}
