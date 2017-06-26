## Description
This repository contains the remote control system of the robot.

## Structure
- server/ contains the server side code written in javascript.
- wwwroot/ contains the web frontend code.
- dragon/ contains the driver program for dragon board.
- pi/ contains the driver program for pi

## Usage
### 1. Clone this repo

### 2. Set up nginx
- Make sure nginx is running
  	- Install nginx with --with-stream. Ubuntu's quick install doesn't 
	  include this, so I had to recompile with 
	  http://www.geoffstratton.com/recompile-nginx-19-installed-apt-ubuntu-1604
	  More info on Stream https://www.nginx.com/resources/admin-guide/tcp-load-balancing/
- sudo ./dist.sh
- This script will copy the front end code to the correct directory and reload nginx

### 3. Start the websocket server
- Please make sure you have node.js installed
- cd to server/
- node index.js
  	- Need to install ws package
	  http://websockets.github.io/ws/
	- "use strict" for all the .js files in server/
	- Need to sudo

### 4. Start the robot client
- cd to dragon/ OR pi/
- ./build_db.sh OR ./build_pi.sh
- ./a.out ip
  - Need to sudo
- ip is the server ip address

### 5. Open a browser and navigate to the server address

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
