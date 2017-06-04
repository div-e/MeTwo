"use strict"

const WebSocket = require('ws')
const net = require('net')
const Robot = require('./Robot');
const Browser = require('./Browser');


//let robot = null
//let browser = null;
//let camState = null
//let conState = null
// let frame = Buffer.allocUnsafe(40000)
// let frameLength = 0
// let copied = 0
// let restBuffer = null
let robot = null;
let browser = null;



const tcpServer = net.createServer(socket => {
    robot = new Robot(socket, buffer => {
        if (browser != null) {
            browser.takeThis(buffer);
        }
    }, () => {
        robot = null;
    });
});
tcpServer.listen(6000);



const wsServer = new WebSocket.Server({
    perMessageDeflate: false,
    port: 6001
})
wsServer.on('connection', (ws) => {
    browser = new Browser(ws, buffer => {
        if (robot != null) {
            robot.takeThis(buffer);
        }
    }, () => {
        browser = null;
    });
});



//function tcpConnectionHandler(socket) {
    //console.log('Robot connected')
    //robot = sock
    //camState = new CameraState()
    //conState = new ControlState()
    //sock.on('close', robotDisconnectHandler)
    //sock.on('data', robotMessageHandler)
//}



// function wsConnectionHandler(ws) {
//     console.log('Browser connected')
//     browser = ws;
//     ws.on('message', wsMessageHandler)
//     ws.on('close', () => {
//         ws = null;
//         console.log('Browser disconnected')
//     })
// }



// function wsMessageHandler(msg) {
//     if (robot != null) {
//         if (msg[0] < Signals.UP) {
//             conState.handle(msg, robot)
//         }
//         else {
//             camState.handle(msg, robot)
//         }
//     }
//     else {
//         console.log('Robot is not connected')
//     }
// }



// function robotDisconnectHandler() {
//     camState.close()
//     console.log('Robot disconnected')
//     robot = null
//     camState = null
//     conState = null
// }



// function robotMessageHandler(buffer) {
//     if (restBuffer != null) {
//         concatBuffer(restBuffer)
//         restBuffer = null
//     }
//     concatBuffer(buffer)

//     if (browser != null && frameLength == copied) {
//         browser.send(frame.slice(0, frameLength))
//     }
// }


// function concatBuffer(buffer) {
//     if (frameLength == copied) {
//         frameLength = buffer.readUInt16BE(0)
//         copied = 0
//         buffer = buffer.slice(2);
//     }

//     let slice = frame.slice(copied, frameLength)
//     if (buffer.length <= frameLength - copied) {
//         let len = buffer.length
//         buffer.copy(slice)
//         copied += len
//         restBuffer = null
//     }
//     else {
//         let len = frameLength - copied
//         buffer.copy(slice, 0, 0, len)
//         copied += len
//         restBuffer = buffer.slice(len)
//     }
// }
