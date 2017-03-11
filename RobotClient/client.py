"""This module listens to the server and calls the controlling functions"""

import sys
import socket
import controls

FORWARD = 1
BACKWARD = 2
LEFT = 3
RIGHT = 4
STOP = 5

CONTROLS = [controls.forward, controls.backward, controls.stop,
            controls.left, controls.right, controls.stop_turning]


def listen():
    """Create a TCP socket and listen to instructions from the server"""

    if len(sys.argv) < 3:
        print("Usage: python client.py ip port")
        return

    tcp_socket = socket.socket()
    try:
        print("Connecting...")
        tcp_socket.connect((sys.argv[1], int(sys.argv[2])))
        print("Connected")
    except (socket.error, ValueError) as err:
        print(err)
        return

    while True:
        CONTROLS[tcp_socket.recv(1)[0] - 1]()


listen()
