"""
The robot controlling functions
Only left(), right() and stop() will be called after a call to forward() or backward()
Only forward(), backward() and stop() will be called after a call to left() or right()
"""


def forward():
    """Make the robot keep moving forward"""
    print("forward")


def backward():
    """Make the robot keep moving backward"""
    print("backward")


def left():
    """Make the robot keep turning left"""
    print("left")


def right():
    """Make the robot keep turning right"""
    print("right")


def stop():
    """Stop the robot"""
    print("stop")


def stop_turning():
    """Stop turning"""
    print("stop turning")

