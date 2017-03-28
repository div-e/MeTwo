const WebSocket = require('ws')
const net = require('net')

let robot = null

const tcpServer = net.createServer(c => {
    robot = c
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

wsServer.on('connection', ws => {
    console.log('browser connected')
    ws.on('message', message => {
        if (robot != null) {
            try {
                robot.write(message)
            }
            catch (e) {
                robot = null
                console.log('falied to communicate with the robot')
                console.log(e)
            }
        }
    })
    ws.on('close', () => {
        console.log('browser disconnected')
    })
})