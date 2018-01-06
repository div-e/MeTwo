"use strict" 

// State class is extended by ControlState
module.exports = class State {
    redirect(buffer, socket) {
        try {
            socket.write(buffer)
        }
        catch (e) {
            console.log('Failed to communicate with robot')
            console.log(e.stack);
        }
    }
}
