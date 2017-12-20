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

//import browserSignals from './pages/script'

//const Script = require('./pages/script');

var root = null; 
if(typeof module !== 'undefined' && typeof module.exports !== 'undefined') 
  root = module;
else 
  root = window; 


//root = exports ? window
// Function Enum
// These must be mapped the same as robot.cpp controls[] fuction
root.robotFunctions = {
        FORWARD: 0,
        BACKWARD: 1,
        STOP: 2,
        LEFT: 3,
        RIGHT: 4,
        STOP_TURNING: 5,
        TILT_UP: 6,
        TILT_DOWN: 7,
        STOP_TILT: 8, 
        PAN_RIGHT: 9,
        PAN_LEFT: 10, 
        STOP_PAN: 11, 
}

// Browser Signal Enum
root.browserSignals = {
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
};

root.map = new Uint8Array(17)
/*exports.map[Script.browserSignals.W_KEY_DOWN] = exports.robotFunctions.FORWARD;
exports.map[Script.browserSignals.W_KEY_UP] = exports.robotFunctions.STOP;
exports.map[Script.browserSignals.S_KEY_DOWN] = exports.robotFunctions.BACKWARD;
exports.map[Script.browserSignals.S_KEY_UP] = exports.robotFunctions.STOP;
exports.map[Script.browserSignals.A_KEY_DOWN] = exports.robotFunctions.LEFT;
exports.map[Script.browserSignals.A_KEY_UP] = exports.robotFunctions.STOP_TURNING;
exports.map[Script.browserSignals.D_KEY_DOWN] = exports.robotFunctions.RIGHT;
exports.map[Script.browserSignals.D_KEY_UP] = exports.robotFunctions.STOP_TURNING;

exports.map[Script.browserSignals.I_KEY_DOWN] = exports.robotFunctions.TILT_UP;
exports.map[Script.browserSignals.I_KEY_UP] = exports.robotFunctions.STOP_TILT;
exports.map[Script.browserSignals.K_KEY_DOWN] = exports.robotFunctions.TILT_DOWN;
exports.map[Script.browserSignals.K_KEY_UP] = exports.robotFunctions.STOP_TILT;
exports.map[Script.browserSignals.J_KEY_DOWN] = exports.robotFunctions.PAN_LEFT;
exports.map[Script.browserSignals.J_KEY_UP] = exports.robotFunctions.STOP_PAN;
exports.map[Script.browserSignals.L_KEY_DOWN] = exports.robotFunctions.PAN_RIGHT;
exports.map[Script.browserSignals.L_KEY_UP] = exports.robotFunctions.STOP_PAN;
*/

root.map[root.browserSignals.W_KEY_DOWN] = root.robotFunctions.FORWARD;
root.map[root.browserSignals.W_KEY_UP] = root.robotFunctions.STOP;
root.map[root.browserSignals.S_KEY_DOWN] = root.robotFunctions.BACKWARD;
root.map[root.browserSignals.S_KEY_UP] = root.robotFunctions.STOP;
root.map[root.browserSignals.A_KEY_DOWN] = root.robotFunctions.LEFT;
root.map[root.browserSignals.A_KEY_UP] = root.robotFunctions.STOP_TURNING;
root.map[root.browserSignals.D_KEY_DOWN] = root.robotFunctions.RIGHT;
root.map[root.browserSignals.D_KEY_UP] = root.robotFunctions.STOP_TURNING;

root.map[root.browserSignals.I_KEY_DOWN] = root.robotFunctions.TILT_UP;
root.map[root.browserSignals.I_KEY_UP] = root.robotFunctions.STOP_TILT;
root.map[root.browserSignals.K_KEY_DOWN] = root.robotFunctions.TILT_DOWN;
root.map[root.browserSignals.K_KEY_UP] = root.robotFunctions.STOP_TILT;
root.map[root.browserSignals.J_KEY_DOWN] = root.robotFunctions.PAN_LEFT;
root.map[root.browserSignals.J_KEY_UP] = root.robotFunctions.STOP_PAN;
root.map[root.browserSignals.L_KEY_DOWN] = root.robotFunctions.PAN_RIGHT;
root.map[root.browserSignals.L_KEY_UP] = root.robotFunctions.STOP_PAN;


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


/*
exports.map = (buffer) => {
    buffer[0] = map[buffer[0]]
}*/
