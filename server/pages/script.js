'use strict';

// Browser Signal Enum
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
};

if (typeof(window) !== 'undefined') {

  window.addEventListener('load', browserFunction, false);
  function browserFunction() {
      let ws = null
      const keyDownSignals = new Map()
      const keyUpSignals = new Map()
      const img = document.getElementById('img')
      keyDownSignals.set('w', new Uint8Array([browserSignals.W_KEY_DOWN]).buffer)
      keyDownSignals.set('s', new Uint8Array([browserSignals.S_KEY_DOWN]).buffer)
      keyUpSignals.set('w', new Uint8Array([browserSignals.W_KEY_UP]).buffer)
      keyUpSignals.set('s', new Uint8Array([browserSignals.S_KEY_UP]).buffer)
      keyDownSignals.set('a', new Uint8Array([browserSignals.A_KEY_DOWN]).buffer)
      keyDownSignals.set('d', new Uint8Array([browserSignals.D_KEY_DOWN]).buffer)
      keyUpSignals.set('a', new Uint8Array([browserSignals.A_KEY_UP]).buffer)
      keyUpSignals.set('d', new Uint8Array([browserSignals.D_KEY_UP]).buffer)
      keyDownSignals.set('i', new Uint8Array([browserSignals.I_KEY_DOWN]).buffer)
      keyDownSignals.set('k', new Uint8Array([browserSignals.K_KEY_DOWN]).buffer)
      keyUpSignals.set('i', new Uint8Array([browserSignals.I_KEY_UP]).buffer)
      keyUpSignals.set('k', new Uint8Array([browserSignals.K_KEY_UP]).buffer)
      keyDownSignals.set('j', new Uint8Array([browserSignals.J_KEY_DOWN]).buffer)
      keyDownSignals.set('l', new Uint8Array([browserSignals.L_KEY_DOWN]).buffer)
      keyUpSignals.set('j', new Uint8Array([browserSignals.J_KEY_UP]).buffer)
      keyUpSignals.set('l', new Uint8Array([browserSignals.L_KEY_UP]).buffer)


      connect()

      function connect() {
          ws = new WebSocket("ws://" + window.location.hostname + ":6001/control")
          ws.binaryType = "arraybuffer"
          ws.onmessage = imgHandler

          window.onkeydown = keyDownHandler
          window.onkeyup = keyUpHandler
      }

      function keyDownHandler(e) {
          if (e.repeat || !keyDownSignals.has(e.key)) {
              return;
          }
          ws.send(keyDownSignals.get(e.key))
      }

      function keyUpHandler(e) {
          if (e.repeat || !keyUpSignals.has(e.key)) {
              return;
          }
          ws.send(keyUpSignals.get(e.key))
      }

      function imgHandler(e) {
          console.log(e.data)
          let b = new Blob([e.data], {type: 'image/jpeg'})
          console.log(b.type)
          img.src = URL.createObjectURL(b)
      }
  }
}
