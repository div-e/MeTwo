(function(exports){

  exports.robotFunctions = {
     FORWARD: 0,
     BACKWARD: 1,
     STOP: 2,
     LEFT: 3,
     RIGHT: 4,
     STOP_TURNING: 5,
     TILT_UP: 6,
     TILT_DOWN: 7,
     STOP_TILT: 8,
     PAN_RIGHT: 9,
     PAN_LEFT: 10,
     STOP_PAN: 11,
  }

  exports.browserSignals = {
        W_KEY_UP: 0,
        W_KEY_DOWN: 1,
        A_KEY_UP: 2,
        A_KEY_DOWN: 3,
        S_KEY_UP: 4,
        S_KEY_DOWN: 5,
        D_KEY_UP: 6,
        D_KEY_DOWN: 7,
        I_KEY_UP: 8,
        I_KEY_DOWN: 9,
        K_KEY_UP: 10,
        K_KEY_DOWN: 11,
        J_KEY_UP: 12,
        J_KEY_DOWN: 13,
        L_KEY_UP: 14,
        L_KEY_DOWN: 15
  }

}(typeof exports === 'undefined' ? this.share = {} : exports))
