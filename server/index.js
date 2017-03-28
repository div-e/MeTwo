const WebSocket = require('ws')
const net = require('net')

let robot = null

const tcpServer = net.createServer(c => {
    console.log('client connected')
    robot = c
    c.on('close', err => {
        robot = null
        console.log('client disconnected')
    })
})
tcpServer.listen(6000)

const wss = new WebSocket.Server({
    perMessageDeflate: false,
    port: 6001
})

wss.on('connection', ws => {
    ws.on('message', message => {
        if (robot != null) {
            robot.write(message)
        }
    })
})