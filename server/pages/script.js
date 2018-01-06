'use strict';

// Define root as module if using this file through nodejs for server, and window if used by client.
var root = null; 
if(typeof module !== 'undefined' && typeof module.exports !== 'undefined') 
  root = module;
else 
  root = window; 

/* 
 * Robot Functions Enum
 * These must be mapped the same as robot.cpp controls[] fuction.
 * The numbers represent the place of the function in the cpp function array.
 * These are what are uncovered by the robot after being sent the browser signal. 
 * The two are mapped together in the variable map. 
 */
root.robotFunctions = {
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

/* 
 * Browser Signal Enum
 * These are all the possible happenings from the browser. 
 */ 
root.browserSignals = {
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
};

/*
 * Map is a simple array mapping between the browser signal and the robotFunction.
 * The browser signals are the indices and the robot functions are the values.  
 * The size of map depends on how many browser signals we have. 
 */ 
root.map = new Uint8Array(17);

root.map[root.browserSignals.W_KEY_DOWN] = root.robotFunctions.FORWARD;
root.map[root.browserSignals.W_KEY_UP] = root.robotFunctions.STOP;
root.map[root.browserSignals.S_KEY_DOWN] = root.robotFunctions.BACKWARD;
root.map[root.browserSignals.S_KEY_UP] = root.robotFunctions.STOP;
root.map[root.browserSignals.A_KEY_DOWN] = root.robotFunctions.LEFT;
root.map[root.browserSignals.A_KEY_UP] = root.robotFunctions.STOP_TURNING;
root.map[root.browserSignals.D_KEY_DOWN] = root.robotFunctions.RIGHT;
root.map[root.browserSignals.D_KEY_UP] = root.robotFunctions.STOP_TURNING;

root.map[root.browserSignals.I_KEY_DOWN] = root.robotFunctions.TILT_UP;
root.map[root.browserSignals.I_KEY_UP] = root.robotFunctions.STOP_TILT;
root.map[root.browserSignals.K_KEY_DOWN] = root.robotFunctions.TILT_DOWN;
root.map[root.browserSignals.K_KEY_UP] = root.robotFunctions.STOP_TILT;
root.map[root.browserSignals.J_KEY_DOWN] = root.robotFunctions.PAN_LEFT;
root.map[root.browserSignals.J_KEY_UP] = root.robotFunctions.STOP_PAN;
root.map[root.browserSignals.L_KEY_DOWN] = root.robotFunctions.PAN_RIGHT;
root.map[root.browserSignals.L_KEY_UP] = root.robotFunctions.STOP_PAN;


/*
 * Code to be run only if this javascript file is run on a browser (i.e. client-side)
*/ 
if (typeof(window) !== 'undefined') {
  browserFunction();

  /*
   * browserFunction()
   * This function maps the keyup and keydown signals and connects the window keyup and keydown
   * events to their appropriate handlers. The keys get sent through a websocket initialized in connect(). 
   */
  function browserFunction() {
      let ws = null
      const keyDownSignals = new Map()
      const keyUpSignals = new Map()
      const img = document.getElementById('img')
      keyDownSignals.set('w', new Uint8Array([window.browserSignals.W_KEY_DOWN]).buffer)
      keyDownSignals.set('s', new Uint8Array([window.browserSignals.S_KEY_DOWN]).buffer)
      keyUpSignals.set('w', new Uint8Array([window.browserSignals.W_KEY_UP]).buffer)
      keyUpSignals.set('s', new Uint8Array([window.browserSignals.S_KEY_UP]).buffer)
      keyDownSignals.set('a', new Uint8Array([window.browserSignals.A_KEY_DOWN]).buffer)
      keyDownSignals.set('d', new Uint8Array([window.browserSignals.D_KEY_DOWN]).buffer)
      keyUpSignals.set('a', new Uint8Array([window.browserSignals.A_KEY_UP]).buffer)
      keyUpSignals.set('d', new Uint8Array([window.browserSignals.D_KEY_UP]).buffer)
      keyDownSignals.set('i', new Uint8Array([window.browserSignals.I_KEY_DOWN]).buffer)
      keyDownSignals.set('k', new Uint8Array([window.browserSignals.K_KEY_DOWN]).buffer)
      keyUpSignals.set('i', new Uint8Array([window.browserSignals.I_KEY_UP]).buffer)
      keyUpSignals.set('k', new Uint8Array([window.browserSignals.K_KEY_UP]).buffer)
      keyDownSignals.set('j', new Uint8Array([window.browserSignals.J_KEY_DOWN]).buffer)
      keyDownSignals.set('l', new Uint8Array([window.browserSignals.L_KEY_DOWN]).buffer)
      keyUpSignals.set('j', new Uint8Array([window.browserSignals.J_KEY_UP]).buffer)
      keyUpSignals.set('l', new Uint8Array([window.browserSignals.L_KEY_UP]).buffer)


      connect()

      /*
       * connect() 
       * Connects websocket to the a specific port on the server
       */
      function connect() {
          ws = new WebSocket("ws://" + window.location.hostname + ":6001/control")
          ws.binaryType = "arraybuffer"
          // WIP for live feed video processing
          ws.onmessage = imgHandler

          window.onkeydown = keyDownHandler
          window.onkeyup = keyUpHandler
      }

      // called when a key is pressed. 
      function keyDownHandler(e) {
          if (e.repeat || !keyDownSignals.has(e.key)) {
              return;
          }
          ws.send(keyDownSignals.get(e.key))
      }

      // called when a key is released. 
      function keyUpHandler(e) {
          if (e.repeat || !keyUpSignals.has(e.key)) {
              return;
          }
          ws.send(keyUpSignals.get(e.key))
      }

      // WIP for live feed video processing
      function imgHandler(e) {
          console.log(e.data)
          let b = new Blob([e.data], {type: 'image/jpeg'})
          console.log(b.type)
          img.src = URL.createObjectURL(b)
      }
  }
  

}
