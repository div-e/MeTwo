## Description
This repository contains the remote control system of the robot.

## Structure
- server/ contains the server side code written in javascript.
- dragon/ contains the driver program for dragon board.
- pi/ contains the driver program for pi

## Usage
### 1. Clone this repo

### 2. Start the websocket server
- make sure you have node.js, express, ws installed
- cd to server/
- node index.js

### 3. Open browser
- localhost:3000
 
### 4. Build the robot code
- cd to dragon/
- mkdir build
- cd build
- cmake .. 
- make
- cd ..


### 5. Run robot code
- in dragon/, sudo ./dragon ip

### 6. Navigate to browser and use WASD, IKJL to control robot motors. 



## TODO
- Authenticate connections
- Auto-recover from exceptions
- Better UI
- Fix paging bug in the streaming server
- Use libjpeg-turbo to compress image
- Enable more threads on the robot
- Auto wifi probing


## compile dependency
cmake, swig, pkgconfig, pthreads

## prepare environment
'''
./bootstrap [with cmake configure option here]
'''

## compile 
'''
cd dragon
mkdir build
cd build
cmake ..
'''

## run
sudo LD_LIBRARY_PATH=/usr/local/lib [command]
