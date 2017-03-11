## Description
This repository contains the remote control system of the robot.

## Structure
- MeTwo.Server contains the server side code written in C#.
- wwwroot contains the web frontend code.
- RobotClient contains the python client running on the robot.

## Usage
### 1. Clone this repo

### 2. Start the server
- Please make sure you have .NET Core SDK installed.
- cd to MeTwo.Server
- dotnet run ip port
- ip is the server ip address, port is the port for TCP connection with the robot.

### 3. Start the robot client
- Please use python 3.6
- cd to RobotClient
- python3.6 client.py ip port
- ip is the server ip address, port is the port for TCP connection with the robot.

## TODO
- Implement the robot client functions
- Authenticate connections
- Real-time video streaming
- Auto-recover from exceptions
- Better UI
