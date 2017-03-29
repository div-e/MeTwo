const WebSocket = require('ws')
const net = require('net')
const CameraState = require('./CameraState')

let robot = null
let state = null

const tcpServer = net.createServer(c => {
    robot = c
    state = new CameraState()
    console.log('robot connected')
    c.on('close', had_error => {
        robot = null
        console.log(`socket is destroyed: ${c.destroyed}`)
        if (had_error) {
            console.log('robot disconnected due to an error')
        }
        else {
            console.log('robot disconnected')
        }
    })
})
tcpServer.listen(6000)

const wsServer = new WebSocket.Server({
    perMessageDeflate: false,
    port: 6001
})

let up = false
let down = false

wsServer.on('connection', ws => {
    console.log('browser connected')
    ws.on('message', message => {
        if (robot != null) {
            let flag = message[0]
            if (flag >= 7) {
                state.handle(message, robot)
                return
            }

            try {
                robot.write(message)
            }
            catch (e) {
                robot = null
                console.log('falied to communicate with the robot')
                console.log(e)
            }
        }
        else {
            console.log('robot is not connected')
        }
    })
    ws.on('close', () => {
        console.log('browser disconnected')
    })
})