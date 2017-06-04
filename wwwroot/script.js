'use strict';

(() => {
    let ws = null
    const keyDownSignals = new Map()
    const keyUpSignals = new Map()
    const img = document.getElementById('img')
    keyDownSignals.set('w', new Uint8Array([1]).buffer)
    keyDownSignals.set('s', new Uint8Array([2]).buffer)
    keyUpSignals.set('w', new Uint8Array([3]).buffer)
    keyUpSignals.set('s', new Uint8Array([4]).buffer)
    keyDownSignals.set('a', new Uint8Array([5]).buffer)
    keyDownSignals.set('d', new Uint8Array([6]).buffer)
    keyUpSignals.set('a', new Uint8Array([7]).buffer)
    keyUpSignals.set('d', new Uint8Array([8]).buffer)
    keyDownSignals.set('i', new Uint8Array([9]).buffer)
    keyDownSignals.set('k', new Uint8Array([10]).buffer)
    keyUpSignals.set('i', new Uint8Array([11]).buffer)
    keyUpSignals.set('k', new Uint8Array([12]).buffer)
    keyDownSignals.set('j', new Uint8Array([13]).buffer)
    keyDownSignals.set('l', new Uint8Array([14]).buffer)
    keyUpSignals.set('j', new Uint8Array([15]).buffer)
    keyUpSignals.set('l', new Uint8Array([16]).buffer)


    connect()

    function connect() {
        ws = new WebSocket("ws://" + window.location.hostname + "/control")
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
})()
