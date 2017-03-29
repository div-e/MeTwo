"use strict";

(() => {
    var ws;
    const FORWARD = new Uint8Array([1]).buffer;
    const BACKWARD = new Uint8Array([2]).buffer;
    const STOP = new Uint8Array([3]).buffer;
    const LEFT = new Uint8Array([4]).buffer;
    const RIGHT = new Uint8Array([5]).buffer;
    const STOP_T = new Uint8Array([6]).buffer;
    const UP = new Uint8Array([7]).buffer
    const DOWN = new Uint8Array([8]).buffer
    const STOP_U = new Uint8Array([9]).buffer
    const STOP_D = new Uint8Array([10]).buffer

    connect();

    function connect() {
        ws = new WebSocket("ws://" + window.location.host + "/control");
        ws.binaryType = "arraybuffer";

        window.onkeydown = keyDownHandler;
        window.onkeyup = keyUpHandler;
    }

    var w = false;
    var a = false;
    var s = false;
    var d = false;

    function keyDownHandler(e) {

        if (e.repeat) {
            return;
        }
        if (e.key === "i") {
            ws.send(UP)
        }
        else if (e.key === "k") {
            ws.send(DOWN)
        }
        if (e.key === "w" && !w && !s) {
            w = true;
            ws.send(FORWARD);
        }
        else if (e.key === "s" && !w && !s) {
            s = true;
            ws.send(BACKWARD);
        }
        else if (e.key === "a" && !a && !d) {
            a = true;
            ws.send(LEFT);
        }
        else if (e.key === "d" && !a && !d) {
            d = true;
            ws.send(RIGHT);
        }
    }

    function keyUpHandler(e) {
        if (e.repeat) {
            return;
        }
        if (e.key === "w" && !s) {
            ws.send(STOP);
            w = false;
        }
        else if (e.key === "s" && !w) {
            ws.send(STOP);
            s = false;
        }
        else if (e.key === "a" && !d) {
            ws.send(STOP_T);
            a = false;
        }
        else if (e.key === "d" && !a) {
            ws.send(STOP_T);
            d = false;
        }
        else if (e.key === "i") {
            ws.send(STOP_U)
        }
        else if (e.key === "k") {
            ws.send(STOP_D)
        }
    }
})()