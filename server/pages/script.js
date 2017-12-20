'use strict';

//import * as Signals from "../Signals";


if (typeof(window) !== 'undefined') {

  console.log(typeof(window));
  browserFunction();
  

  //window.addEventListener('load', browserFunction, false);

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

      function connect() {
          ws = new WebSocket("ws://" + window.location.hostname + ":6001/control")
          ws.binaryType = "arraybuffer"
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

      function imgHandler(e) {
          console.log(e.data)
          let b = new Blob([e.data], {type: 'image/jpeg'})
          console.log(b.type)
          img.src = URL.createObjectURL(b)
      }
  }
  

}
