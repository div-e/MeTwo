"use strict" 

// State class is extended by ControlState
module.exports = class State {
    redirect(buffer, socket) {
        try {
            socket.write(buffer)
        }
        catch (e) {
            console.log('Falied to communicate with the robot')
        }
    }
}
