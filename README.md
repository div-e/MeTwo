## Description
This repository contains the WIP code for the second iteration of MeTwo. 

MeTwo is a remote, wifi-controlled robot, that sends back webcam data to its user. The user can connect to the robot by asking for the homepage of the robot's server. The homepage will automatically connect the webcam data from the robot to its background image so the user can see what the robot is seeing. Additionally, the user can control the robot by joystick controls on the screen, WASD keys if on desktop, and a deploy button to deploy the robot's arms. 

The project requires an Arduino Mega and Dragonboard 410c. The Arduino controls the robot's actuators through serial inputs from the Dragonboard 410c. The Dragonboard connects to the internet (and eventually receives the data from the client), serves the webpage, and feeds webcam data. 

## Structure
- server/ contains the server side code written in javascript.
- arduino/ contains the drivingControls that listens for instruction on serial connection written in Arduino. 

## Hardware Set up 
### 1. Connect Router to Ethernet Port
### 2. Connect Arduino to Port 0 of Dragonboard
### 3. Connect Webcam to Port 1 of Dragonboard
### 4. Connect 11.1V Battery to Robot
### 5. SSH into Dragonboard to begin server (see Code Usage)
- Make sure Arduino Code is uploaded to Arduino Mega. 
- Make sure wires are connected as specified in code. 
- Test with robot elevated. 


## Code Usage (Attempt after robot has been built and connected to boards)
### 1. Clone this repo to the Dragonboard 410c
 
### 2. Load the Arduino Code
- Install Arduino IDE on Draonboard
- Upload drivingControls.ino to the Mega

### 3. Start the websocket server
- in server/ 'npm install' to obtain 'serialport', 'ws', and 'express' node packages. 
- node server.js
- Make sure Mega is connected to USB port 0 of Dragonboard for serial output from the server. Webcam connected to port 1. 

### 4. On a client on the same intranet (TODO: make it so client can be on a different router)
- Our code assumes that Dragonboard has local ip 192.168.0.201, so client will need to hit 192.168.0.201:3000 in their browser. 

### 5. Drive using WASD (if on desktop) or (right or left?) joystick (if on mobile)

### 6. Look around using on-screen joystick (drag using mouse click+drag).

### 7. Deploy and retract arms using on-screen button or spacebar? 


## TODO
- Authenticate connections
- Enable public access to obtain controls (vs intranet)
- Auto wifi probing
- Distance sensors to provide user feedback of falling dangers (i.e. when to deploy arms)



