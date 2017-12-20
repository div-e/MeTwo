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
        
        this.up = false
        this.down = false
	      this.panRight = false
	      this.panLeft = false
    }

    validate(funct) {

      switch(funct) {
        case Signals.robotFunctions.FORWARD: 
          if (this.forward || this.backward) {
            return false
          } else {
            this.forward = true
            return true
          }
        case Signals.robotFunctions.BACKWARD: 
          if (this.backward || this.forward) {
            return false
          } else {
            this.backward = true
            return true
          }
        case Signals.robotFunctions.STOP: 
          if(this.forward || this.backward) { 
            this.forward = false; 
            this.backward = false; 
            return true; 
          } else { 
            return false; 
          }
        case Signals.robotFunctions.LEFT: 
          if(this.left || this.right) {
            return false; 
          } else {
            this.left = true;
            return true; 
          } 
        case Signals.robotFunctions.RIGHT: 
          if (this.right || this.left) {
            return false
          } else {
            this.right = true
            return true
          }
        case Signals.robotFunctions.STOP_TURNING:
          if (this.left || this.right) {
            this.left = false
            this.right = false;
            return true
          } else { 
            return false 
          }
        case Signals.robotFunctions.TILT_UP: 
          if (this.down || this.up) {
            return false
          } else { 
            this.up = true
            return true
          } 
        case Signals.robotFunctions.TILT_DOWN: 
          if (this.down || this.up) {
            return false
          } else { 
            this.down = true
            return true
          } 
        case Signals.robotFunctions.STOP_TILT: 
          if (this.up || this.down) {
            this.up = false
            this.down = false
            return true
          } else {
            return false
          }
        case Signals.robotFunctions.PAN_LEFT: 
          if(this.panRight || this.panLeft) {
            return false
          } else {
            this.panLeft = true
            return true
          }
        case Signals.robotFunctions.PAN_RIGHT: 
          if(this.panRight || this.panLeft) {
            return false; 
          } else {
            this.panLeft = true; 
            return true; 
          } 
        case Signals.robotFunctions.STOP_PAN: 
          if (this.panRight || this.panLeft) {
            this.panRight = false
            this.panLeft = false
            return true
          } else {
            return false
          }
        default: return false; 



      }
      /*
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


        // Camera tilting controls
        else if (flag === Signals.UP) {
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

        else if(flag === Signals.PAN_RIGHT) {
          if(this.panRight || this.panLeft) {
            return false
          } else {
            this.panRight = true
            return true
          }
        }
        else if(flag === Signals.PAN_LEFT) {
          if(this.panRight || this.panLeft) {
            return false
          } else {
            this.panLeft = true
            return true
          }
        }

        return false*/
    }



    handle(buffer, socket) {
      var browserSignal = buffer[0]; 
      if(Signals.map.has(browserSignal)) {
        var signal = Signals.map.get(browserSignal); 
        if(this.validate(signal)) {
          super.redirect(signal, socket); 
        } 
      }
    }
/*
      if (Signals.map.has(browserSignal) && 
        this.validate(Signals.map(browserSignal))) {
          //Signals.map(buffer)
          super.redirect(Signals.map(browserSignal), socket)
      }
    }*/
}
