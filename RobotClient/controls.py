"""
The robot controlling functions
Only left(), right() and stop() will be called after a call to forward() or backward()
Only forward(), backward() and stop() will be called after a call to left() or right()
"""

# External module imports
import RPi.GPIO as GPIO
import time

#Motor Driver Pin declarations
motorA_pwm = 18 #GPIO18 for Motor A PWM speed control
motorA_01 = 5 #GPIO05 for Motor A 01 input
motorA_02 = 6 #GPIO06 for Motor A 02 input
motorB_01 = 13 #GPIO13 for Motor B 01 input
motorB_02 = 19 #GPIO19 for Motor B 02 input
motorB_pwm = 26 #GPIO26 for Motor B PWM speed control
stdby = 21 #GPIO21 for Standby pin on Motor Driver

#PWM Constants
dc_high = 95 # duty cycle (0-100) for PWM 
dc_low = 20 # duty cycle (0-100) for PWM
dc_off = 0 # duty cycle (0-100) for PWM
pwm_freq = 50 #pwm frequency for setup

# Pin Setup:
GPIO.setmode(GPIO.BCM) # Broadcom pin-numbering scheme

GPIO.setup(motorA_pwm, GPIO.OUT) # LED pin set as output
GPIO.setup(motorA_01, GPIO.OUT) # PWM pin set as output
GPIO.setup(motorA_02, GPIO.OUT) # PWM pin set as output
GPIO.setup(motorB_01, GPIO.OUT) # PWM pin set as output
GPIO.setup(motorB_02, GPIO.OUT) # PWM pin set as output
GPIO.setup(motorB_pwm, GPIO.OUT) # PWM pin set as output
GPIO.setup(stdby, GPIO.OUT) # Standby pin on motor driver needs to be pulled high 

pwmA = GPIO.PWM(motorA_pwm, pwm_freq) #Initialize PWM on motorA_pwm 100Hz frequency
pwmB = GPIO.PWM(motorB_pwm, pwm_freq) #Initialize PWM on motorA_pwm 100Hz frequency

#Initial states
GPIO.output(motorA_01, GPIO.LOW)
GPIO.output(motorA_02, GPIO.LOW)
GPIO.output(motorB_01, GPIO.LOW)
GPIO.output(motorB_02, GPIO.LOW)
GPIO.output(stdby, GPIO.HIGH)
pwmA.start(dc_low)
pwmB.start(dc_low)

def forward():
    """Make the robot keep moving forward"""
    print("forward")

    pwmA.ChangeDutyCycle(dc_off)
    pwmB.ChangeDutyCycle(dc_off)

    GPIO.output(motorA_01, GPIO.LOW)
    GPIO.output(motorA_02, GPIO.HIGH)
    GPIO.output(motorB_01, GPIO.LOW)
    GPIO.output(motorB_02, GPIO.HIGH)

    pwmA.ChangeDutyCycle(dc_high)
    pwmB.ChangeDutyCycle(dc_high)


def backward():
    """Make the robot keep moving backward"""
    print("backward")

    pwmA.ChangeDutyCycle(dc_off)
    pwmB.ChangeDutyCycle(dc_off)

    GPIO.output(motorA_01, GPIO.HIGH)
    GPIO.output(motorA_02, GPIO.LOW)
    GPIO.output(motorB_01, GPIO.HIGH)
    GPIO.output(motorB_02, GPIO.LOW)

    pwmA.ChangeDutyCycle(dc_high)
    pwmB.ChangeDutyCycle(dc_high)


def left():
    """Make the robot keep turning left"""
    print("left")

    pwmA.ChangeDutyCycle(dc_off)
    pwmB.ChangeDutyCycle(dc_off)

    GPIO.output(motorA_01, GPIO.LOW)
    GPIO.output(motorA_02, GPIO.HIGH)
    GPIO.output(motorB_01, GPIO.LOW)
    GPIO.output(motorB_02, GPIO.HIGH)

    pwmA.ChangeDutyCycle(dc_low)
    pwmB.ChangeDutyCycle(dc_high)


def right():
    """Make the robot keep turning right"""
    print("right")

    pwmA.ChangeDutyCycle(dc_off)
    pwmB.ChangeDutyCycle(dc_off)

    GPIO.output(motorA_01, GPIO.LOW)
    GPIO.output(motorA_02, GPIO.HIGH)
    GPIO.output(motorB_01, GPIO.LOW)
    GPIO.output(motorB_02, GPIO.HIGH)

    pwmA.ChangeDutyCycle(dc_high)
    pwmB.ChangeDutyCycle(dc_low)


def stop():
    """Stop the robot"""
    print("stop")

    pwmA.ChangeDutyCycle(dc_off)
    pwmB.ChangeDutyCycle(dc_off)

    GPIO.output(motorA_01, GPIO.LOW)
    GPIO.output(motorA_02, GPIO.LOW)
    GPIO.output(motorB_01, GPIO.LOW)
    GPIO.output(motorB_02, GPIO.LOW)
    GPIO.output(stdby, GPIO.LOW)

    pwmA.stop() # stop PWM
    pwmB.stop() # stop PWM
    GPIO.cleanup() # cleanup all GPIO


def stop_turning():
    """Stop turning"""
    print("stop turning")
    
    pwmA.ChangeDutyCycle(dc_high)
    pwmB.ChangeDutyCycle(dc_high)

