/*exports.FORWARD = 1
exports.BACKWARD = 2
exports.STOP_F = 3
exports.STOP_B = 4
exports.LEFT = 5
exports.RIGHT = 6
exports.STOP_L = 7
exports.STOP_R = 8

exports.UP = 9
exports.DOWN = 10
exports.STOP_U = 11
exports.STOP_D = 12
exports.PAN_RIGHT = 13
exports.PAN_LEFT = 14
exports.STOP_PAN_RIGHT = 15
exports.STOP_PAN_LEFT = 16*/


//Signals Enum
var exports.browserSignals = {
        W_KEY_UP: 0, 
        W_KEY_DOWN: 1,
        A_KEY_UP: 2,
        A_KEY_DOWN: 3,
        S_KEY_UP: 4, 
        S_KEY_DOWN: 5, 
        D_KEY_UP: 6,
        D_KEY_DOWN: 7, 
        I_KEY_UP: 8,
        I_KEY_DOWN: 9, 
        K_KEY_UP: 10, 
        K_KEY_DOWN: 11,
        J_KEY_UP: 12,
        J_KEY_DOWN: 13,
        L_KEY_UP: 14,
        L_KEY_DOWN: 15
}

// Function Enum
var robotFunctions = {
        FORWARD: 0,
        BACKWARD: 1, 
        STOP: 2, 
        LEFT: 3, 
        RIGHT: 4, 
        STOP_TURNING: 5,
        TILT_UP: 6, 
        TILT_DOWN: 7, 
        PAN_RIGHT: 8, 
        PAN_LEFT: 9
} 

const map = new Uint8Array(17)
map[browserSignals.W_KEY_DOWN] = robotFunctions.FORWARD;
map[browserSignals.W_KEY_UP] = robotFunctions.STOP;
map[browserSignals.S_KEY_DOWN] = robotFunctions.BACKWARD;
map[browserSignals.S_KEY_UP] = robotFunctions.STOP;
map[browserSignals.A_KEY_DOWN] = robotFunctions.LEFT;
map[browserSignals.A_KEY_UP] = robotFunctions.STOP_TURNING;
map[browserSignals.D_KEY_DOWN] = robotFunctions.RIGHT;
map[browserSignals.D_KEY_UP] = robotFunctions.STOP_TURNING;
map[exports.BACKWARD] = backward
map[exports.STOP_F] = stop
map[exports.STOP_B] = stop
map[exports.LEFT] = left
map[exports.RIGHT] = right
map[exports.STOP_L] = stop_turning
map[exports.STOP_R] = stop_turning
map[exports.UP] = tiltUp
map[exports.DOWN] = tiltDown
map[exports.STOP_U] = stop
map[exports.STOP_D] = stop
map[exports.PAN_RIGHT] = panRight
map[exports.PAN_LEFT] = panLeft
map[exports.STOP_PAN_RIGHT] = stop
map[exports.STOP_PAN_LEFT] = stop
//this array maps to the keyUpSignals + keyDownSignals array in script.js

// map is an array of size 13



exports.map = (buffer) => {
    buffer[0] = map[buffer[0]]
}
