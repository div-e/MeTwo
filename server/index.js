const WebSocket = require('ws')
const net = require('net')
const CameraState = require('./CameraState')
const ControlState = require('./ControlState')
const Signals = require('./Signals')



let robot = null
let camState = null
let conState = null



const tcpServer = net.createServer(tcpConnectionHandler)
tcpServer.listen(6000)
const wsServer = new WebSocket.Server({
    perMessageDeflate: false,
    port: 6001
})
wsServer.on('connection', wsConnectionHandler)



function tcpConnectionHandler(sock) {
    console.log('Robot connected')
    robot = sock
    camState = new CameraState()
    conState = new ControlState()
    sock.on('close', robotDisconnectHandler)
}



function wsConnectionHandler(ws) {
    console.log('Browser connected')
    ws.on('message', wsMessageHandler)
    ws.on('close', () => {
        console.log('Browser disconnected')
    })
}



function wsMessageHandler(msg) {
    if (robot != null) {
        if (msg[0] < Signals.UP) {
            conState.handle(msg, robot)
        }
        else {
            camState.handle(msg, robot)
        }
    }
    else {
        console.log('Robot is not connected')
    }
}



function robotDisconnectHandler() {
    camState.close()
    console.log('Robot disconnected')
    robot = null
    camState = null
    conState = null
}