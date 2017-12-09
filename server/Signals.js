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

import Script from './pages/script.js'

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
map[Script.browserSignals.W_KEY_DOWN] = robotFunctions.FORWARD;
map[Script.browserSignals.W_KEY_UP] = robotFunctions.STOP;
map[Script.browserSignals.S_KEY_DOWN] = robotFunctions.BACKWARD;
map[Script.browserSignals.S_KEY_UP] = robotFunctions.STOP;
map[Script.browserSignals.A_KEY_DOWN] = robotFunctions.LEFT;
map[Script.browserSignals.A_KEY_UP] = robotFunctions.STOP_TURNING;
map[Script.browserSignals.D_KEY_DOWN] = robotFunctions.RIGHT;
map[Script.browserSignals.D_KEY_UP] = robotFunctions.STOP_TURNING;

map[Script.browserSignals.I_KEY_DOWN] = robotFunctions.TILT_UP;
map[Script.browserSignals.I_KEY_UP] = robotFunctions.TILT_UP;
map[Script.browserSignals.K_KEY_DOWN] = robotFunctions.TILT_DOWN;
map[Script.browserSignals.K_KEY_UP] = robotFunctions.TILT_DOWN;
map[Script.browserSignals.J_KEY_DOWN] = robotFunctions.PAN_LEFT;
map[Script.browserSignals.J_KEY_UP] = robotFunctions.PAN_LEFT;
map[Script.browserSignals.L_KEY_DOWN] = robotFunctions.PAN_RIGHT;
map[Script.browserSignals.L_KEY_UP] = robotFunctions.PAN_RIGHT;

/*
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
*/
//this array maps to the keyUpSignals + keyDownSignals array in script.js

// map is an array of size 13



exports.map = (buffer) => {
    buffer[0] = map[buffer[0]]
}
