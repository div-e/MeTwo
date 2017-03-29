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



const forward = 1
const backward = 2
const stop = 3
const left = 4
const right = 5
const stop_turning = 6
const up = 7
const down = 8
const map = new Uint8Array(11)
map[exports.FORWARD] = forward
map[exports.BACKWARD] = backward
map[exports.STOP_F] = stop
map[exports.STOP_B] = stop
map[exports.LEFT] = left
map[exports.RIGHT] = right
map[exports.STOP_L] = stop_turning
map[exports.STOP_R] = stop_turning
map[exports.UP] = up
map[exports.DOWN] = down



exports.map = (buffer) => {
    buffer[0] = map[buffer[0]]
}