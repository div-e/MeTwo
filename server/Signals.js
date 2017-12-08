exports.FORWARD = 1
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
exports.PAN_RIGHT = 11
exports.PAN_LEFT = 12
exports.STOP_PAN_RIGHT = 15
exports.STOP_PAN_LEFT = 16



const forward = 1
const backward = 2
const stop = 3
const left = 4
const right = 5
const stop_turning = 6
const tiltUp = 7
const tiltDown = 8
const panRight = 9
const panLeft = 10

const map = new Uint8Array(17)
map[exports.FORWARD] = forward
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
