"""
The robot controlling functions
Only left(), right() and stop() will be called after a call to forward() or backward()
Only forward(), backward() and stop() will be called after a call to left() or right()
"""

# External module imports
import RPi.GPIO as GPIO
import time

################## VARIABLE DEFINITIONS ##################

# Motor Driver Pin declarations
motorA_pwm = 12 #GPIO12 (Pin 32) for Motor A PWM speed control
motorA_01 = 5 #GPIO05 (Pin 29) for Motor A 01 input
motorA_02 = 6 #GPIO06 (Pin 31) for Motor A 02 input
motorB_01 = 13 #GPIO13 (Pin 33) for Motor B 01 input
motorB_02 = 19 #GPIO19 (Pin 35) for Motor B 02 input
motorB_pwm = 26 #GPIO26 (Pin 37) for Motor B PWM speed control
stdby = 21 #GPIO21 (Pin 40) for Standby pin on Motor Driver

# PWM Constants
dc_max = 100
dc_min = 0
dc_speed_inc = 60
dc_inc = 20

# pwm frequency for setup
pwm_freq = 50 

# Default pin modes
dc_pwmA = dc_min
dc_pwmB = dc_min
last_dc_pwmA = dc_pwmA
last_dc_pwmB = dc_pwmB
motorA_01_state = GPIO.LOW
motorA_02_state = GPIO.LOW
motorB_01_state = GPIO.LOW
motorB_02_state = GPIO.LOW

# Last call states 
FORWARD = 1
BACKWARD = 2
RIGHT = 3
LEFT = 4
STOP = 0

# Default lastCall to STOP
lastCall = STOP

######################### INITIALIZER ######################

# initializes pins (be sure to call before using other methods)
def init(): 
    # Pin Setup:
    GPIO.setmode(GPIO.BCM) # Broadcom pin-numbering scheme

    GPIO.setup(motorA_pwm, GPIO.OUT) # LED pin set as output
    GPIO.setup(motorA_01, GPIO.OUT) # PWM pin set as output
    GPIO.setup(motorA_02, GPIO.OUT) # PWM pin set as output
    GPIO.setup(motorB_01, GPIO.OUT) # PWM pin set as output
    GPIO.setup(motorB_02, GPIO.OUT) # PWM pin set as output
    GPIO.setup(motorB_pwm, GPIO.OUT) # PWM pin set as output
    GPIO.setup(stdby, GPIO.OUT) # Standby pin on motor driver needs to be pulled high 

    global pwmA
    pwmA = GPIO.PWM(motorA_pwm, pwm_freq) #Initialize PWM on motorA_pwm 100Hz frequency
    global pwmB
    pwmB  = GPIO.PWM(motorB_pwm, pwm_freq) #Initialize PWM on motorA_pwm 100Hz frequency

    GPIO.output(stdby, GPIO.HIGH)

    pwmA.start(dc_pwmA)
    pwmB.start(dc_pwmB)
    stop()

################ FORWARD and BACKWARD METHODS #################

# makes the robot go forward 
def forward():
    """Make the robot keep moving forward"""
    print("forward")
    
    if lastCall == FORWARD: 
        #Speed incrementing
        if dc_pwmA + dc_speed_inc <= dc_max: 
            dc_pwmA += dc_speed_inc
        else:
            dc_pwmA = dc_max
        
        if dc_pwmB + dc_speed_inc <= dc_max: 
            dc_pwmB += dc_speed_inc
        else:
            dc_pwmB = dc_max
    else:
        dc_pwmA = dc_speed_inc
        dc_pwmB = dc_speed_inc

    motorA_01_state = GPIO.LOW
    motorA_02_state = GPIO.HIGH
    motorB_01_state = GPIO.LOW
    motorB_02_state = GPIO.HIGH

    updateMotors()

    last_dc_pwmA = dc_pwmA
    last_dc_pwmB = dc_pwmB
    lastCall = FORWARD

# makes the robot go backward 
def backward():
    """Make the robot keep moving backward"""
    print("backward")

    if lastCall == BACKWARD: 
        #Speed incrementing
        if dc_pwmA + dc_speed_inc <= dc_max: 
            dc_pwmA += dc_speed_inc
        else:
            dc_pwmA = dc_max
        
        if dc_pwmB + dc_speed_inc <= dc_max: 
            dc_pwmB += dc_speed_inc
        else:
            dc_pwmB = dc_max
    else:
        dc_pwmA = dc_speed_inc
        dc_pwmB = dc_speed_inc

    motorA_01_state = GPIO.HIGH
    motorA_02_state = GPIO.LOW
    motorB_01_state = GPIO.HIGH
    motorB_02_state = GPIO.LOW

    updateMotors()

    last_dc_pwmA = dc_pwmA
    last_dc_pwmB = dc_pwmB
    lastCall = BACKWARD

##################### TURNING METHODS ####################

# turning LEFT means moving motor A faster than motor B 
def left():
    """Make the robot keep turning left"""
    print("left")

    if dc_pwmA + dc_inc <= dc_max: 
        dc_pwmA += dc_inc
    else
        dc_pwmA = dc_max
        dc_pwmB = dc_max - dc_inc

    updateMotors()

# turning RIGHT means moving motor B faster than motor A 
def right():
    """Make the robot keep turning right"""
    print("right")

    if dc_pwmB + dc_inc <= dc_max: 
        dc_pwmB += dc_inc
    else
        dc_pwmB = dc_max
        dc_pwmA = dc_max - dc_inc

    updateMotors()

##################### STOPPING METHODS ####################

# Stops all motors and sets PWM to 0
def stop():
    """Stop the robot"""
    print("stop")

    dc_pwmA = dc_min
    dc_pwmB = dc_min

    motorA_01_state = GPIO.LOW
    motorA_02_state = GPIO.LOW
    motorB_01_state = GPIO.LOW
    motorB_02_state = GPIO.LOW

    updateMotors() 

    last_dc_pwmA = dc_pwmA
    last_dc_pwmB = dc_pwmB
    lastCall = STOP

# Puts robot back into original FORWARD, BACKWARD, or STOP state
def stop_turning():
    """Stop turning"""
    print("stop turning")

    if lastCall == FORWARD: 
        motorA_01_state = GPIO.LOW
        motorA_02_state = GPIO.HIGH
        motorB_01_state = GPIO.LOW
        motorB_02_state = GPIO.HIGH
    elif lastCall == BACKWARD:
        motorA_01_state = GPIO.HIGH
        motorA_02_state = GPIO.LOW
        motorB_01_state = GPIO.HIGH
        motorB_02_state = GPIO.LOW
        
    dc_pwmA = last_dc_pwmA
    dc_pwmB = last_dc_pwmB

    updateMotors()

# Cleans up pins and pinmodes. Call init() again to start again. 
def terminate(): 
    stop()
    GPIO.output(stdby, GPIO.LOW)
    pwmA.stop() # stop PWM
    pwmB.stop() # stop PWM
    GPIO.cleanup() # cleanup all GPIO

# Helper method to update the pins to the set global variables
def updateMotors(): 
    pwmA.ChangeDutyCycle(dc_pwmA)
    pwmB.ChangeDutyCycle(dc_pwmB)

    GPIO.output(motorA_01, motorA_01_state)
    GPIO.output(motorA_02, motorA_02_state)
    GPIO.output(motorB_01, motorB_01_state)
    GPIO.output(motorB_02, motorB_02_state)

    
    