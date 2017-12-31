"use strict"

// Signals for mapping and understanding the signals we get. 
const Signals = require('./pages/script');

// Extending State for redirect function to robot.
const State = require('./State')


module.exports = class ControlState extends State {
    /* Constructor initializes all the variables */
    constructor() {
        super()
        this.forward = false
        this.backward = false
        this.left = false
        this.right = false
        
        this.tiltUp = false
        this.tiltDown = false
	      this.panRight = false
	      this.panLeft = false
    }

    /* 
     * validate()
     * checks if any functions need to be sent to robot. Returns true if yes, false if not. 
     * Also, updates the states of the robot set in constructor. 
     */
    validate(signal) {

      switch(signal) {
        case Signals.browserSignals.W_KEY_DOWN: 
          if (this.forward || this.backward) {
            return false
          } else {
            // Go forward
            this.forward = true
            return true
          }
        case Signals.browserSignals.S_KEY_DOWN: 
          if (this.backward || this.forward) {
            return false
          } else {
            // Go backward
            this.backward = true
            return true
          }
        case Signals.browserSignals.W_KEY_UP: 
          if(this.forward) { 
            // Stop going forward
            this.forward = false; 
            return true; 
          } else { 
            return false; 
          }
        case Signals.browserSignals.S_KEY_UP: 
          if(this.backward) { 
            // Stop going backward
            this.backward = false; 
            return true; 
          } else { 
            return false; 
          }
        case Signals.browserSignals.A_KEY_DOWN: 
          if(this.left || this.right) {
            return false; 
          } else {
            // Go left
            this.left = true;
            return true; 
          } 
        case Signals.browserSignals.D_KEY_DOWN: 
          if (this.right || this.left) {
            return false
          } else {
            // Go right
            this.right = true
            return true
          }
        case Signals.browserSignals.A_KEY_UP:
          if (this.left) {
            // Stop turning left
            this.left = false
            return true
          } else { 
            return false 
          }
        case Signals.browserSignals.D_KEY_UP:
          if (this.right) {
            // Stop turning right
            this.right = false
            return true
          } else { 
            return false 
          }
        case Signals.browserSignals.I_KEY_DOWN: 
          if (this.tiltDown || this.tiltUp) {
            return false
          } else { 
            // Tilt camera up
            this.tiltUp = true
            return true
          } 
        case Signals.browserSignals.K_KEY_DOWN: 
          if (this.tiltDown || this.tiltUp) {
            return false
          } else { 
            // Tilt camera down
            this.tiltDown = true
            return true
          } 
        case Signals.browserSignals.I_KEY_UP: 
          if (this.tiltUp) {
            // Stop tilting up
            this.tiltUp = false
            return true
          } else {
            return false
          }
        case Signals.browserSignals.K_KEY_UP: 
          if (this.tiltDown) {
            // Stop tilting down
            this.tiltDown = false
            return true
          } else {
            return false
          }
        case Signals.browserSignals.J_KEY_DOWN: 
          if(this.panRight || this.panLeft) {
            return false
          } else {
            // Pan left
            this.panLeft = true
            return true
          }
        case Signals.browserSignals.L_KEY_DOWN: 
          if(this.panRight || this.panLeft) {
            return false; 
          } else {
            // Pan right
            this.panRight = true; 
            return true; 
          } 
        case Signals.browserSignals.J_KEY_UP: 
          if (this.panLeft) {
            // Stop pan left
            this.panLeft = false
            return true
          } else {
            return false
          }
        case Signals.browserSignals.L_KEY_UP: 
          if (this.panRight) {
            // Stop pan right
            this.panRight = false
            return true
          } else {
            return false
          }
        default: return false; 

      }
    }


    /* 
     * handle() 
     * called when a buffer to the robot needs to be handled.
     */
    handle(buffer, socket) {
      var browserSignal = buffer[0]; 
      // Check if valid browser signal
      if(Signals.map.has(browserSignal)) {
        // Check if any function changes need to be sent to robot
        if(this.validate(browserSignal)) {
          var robotFunction = Signals.map.get(browserSignal); 
          // Send mapped robotFunction to robot
          super.redirect(robotFunction, socket); 
        } 
      }
    }
}
