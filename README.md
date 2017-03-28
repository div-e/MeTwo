## Description
This repository contains the remote control system of the robot.

## Structure
- server/ contains the server side code written in javascript.
- wwwroot contains the web frontend code.
- RobotClient contains the python client running on the robot.

## Usage
### 1. Clone this repo

### 2. Set up nginx
- Make sure nginx is running
- sudo ./dist.sh
- This script will copy the front end code to the correct directory and reload nginx

### 3. Start the websocket server
- Please make sure you have node.js installed
- cd to server/
- node index.js

### 4. Start the robot client
- cd to client/
- ./build.sh
- ./a.out ip
- ip is the server ip address

### 5. Open a browser and navigate to the server address

## TODO
- Authenticate connections
- Auto-recover from exceptions
- Better UI
