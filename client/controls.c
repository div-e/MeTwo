#include <stdio.h>
#include "controls.h"

#include <wiringPi.h> 
#include <softPwm.h> 
#include <errno.h>

//////////////// VARIABLE DEFINITIONS ////////////////

// Motor Driver Pin declarations (Broadcom pinouts)
#define motorA_pwm 12 //GPIO12 (Pin 32) for Motor A PWM speed control
#define motorA_01 5 //GPIO05 (Pin 29) for Motor A 01 input
#define motorA_02 6 //GPIO06 (Pin 31) for Motor A 02 input
#define motorB_01 13 //GPIO13 (Pin 33) for Motor B 01 input
#define motorB_02 19 //GPIO19 (Pin 35) for Motor B 02 input
#define motorB_pwm 26 //GPIO26 (Pin 37) for Motor B PWM speed control
#define stdby 21 //GPIO21 (Pin 40) for Standby pin on Motor Driver
#define servo 18 //GPIO18 (Pin 12) for camera tilt

// Last call states 
#define FORWARD 1
#define BACKWARD 2
#define RIGHT 3
#define LEFT 4
#define STOP 0

// PWM Constants
#define PWM_MIN 0 
#define PWM_MAX 100 
#define PWM_SPINC 60 //how high the motors go when FORWARD or BACKWARD
#define PWM_INC 20 //for turning
#define PWM_SERVO_MAX 300
#define PWM_SINC 5

// Default pin modes
int pwmA_val = PWM_MIN;
int pwmB_val = PWM_MIN;
int last_pwmA_val = PWM_MIN;
int last_pwmB_val = PWM_MIN;
int motorA_01_state = LOW;
int motorA_02_state = LOW;
int motorB_01_state = LOW;
int motorB_02_state = LOW;
int pwmServo_val = PWM_MIN;

// Default lastCall to STOP
int lastCall = STOP;

void updateMotors(void);

/////////////////// INITIALIZER ///////////////////
// initializes wiringPi and pins
int init()
{
    printf("%s\n", "Initializing...");

    if(getuid()!=0) //wiringPi requires root privileges  
    {  
        printf("Error: wiringPi must be run as root.\n");  
        return 1;  
    } 

    wiringPiSetupGpio(); // Initializes wiringPi using the Broadcom GPIO pin numbers
    
    // not sure if we need soft pwm, but we'll try it first
    if (softPwmCreate(motorA_pwm, PWM_MIN, PWM_MAX) != 0) {
        printf("Motor A PWM failed to create %s\n", strerror(errno));
        return 1; 
    } 
    if (softPwmCreate(motorB_pwm, PWM_MIN, PWM_MAX) != 0) {
        printf("Motor B PWM failed to create %s\n", strerror(errno));
        return 1; 
    } 

    pinMode(motorA_01, OUTPUT); 
    pinMode(motorA_02, OUTPUT);
    pinMode(motorB_01, OUTPUT);
    pinMode(motorB_02, OUTPUT);
    pinMode(servo, PWM_OUTPUT);
    
    pwmSetMode(PWM_MODE_MS); 
    pwmSetClock(384); //clock at 50kHz (20us tick)
    pwmSetRange(1000); //range at 1000 ticks (20ms)

    pinMode(stdby, OUTPUT); 
    digitalWrite(stdby, HIGH); //Pull Standby high 

    stop();
    
    return 0; 
}

//////////////////// UP and DOWN Tilting methods //////////////

// increments the camera tilt servo by SINC val
void up() {
    if(pwmServo_val + PWM_SINC <= PWM_MAX) {
        pwmServo_val += PWM_SINC
    } else {
        pwmServo_val = PWM_MAX; 
    }
    printf("Up was called. Servo val: %d\n", pwmServo_val);
    pwmWrite(servo, pwmServo_val);
}

// decrements the camera tilt servo by SINC val
void down() {
    if(pwmServo_val - PWM_SINC >= PWM_MIN) {
        pwmServo_val -= PWM_SINC
    } else {
        pwmServo_val = PWM_MIN; 
    }
    printf("Down was called. Servo val: %d\n", pwmServo_val);
    pwmWrite(servo, pwmServo_val);
}

//////////////////// FORWARD and BACKWARD METHODS /////////////

// makes the robot go forward 
void forward()
{
    printf("%s\n", "forward");

    pwmA_val = PWM_SPINC; 
    pwmB_val = PWM_SPINC; 

    motorA_01_state = LOW;
    motorA_02_state = HIGH;
    motorB_01_state = LOW;
    motorB_02_state = HIGH;

    updateMotors();

    last_pwmA_val = pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = FORWARD;
}

// makes the robot go backward 
void backward()
{
    printf("%s\n", "backward");

    pwmA_val = PWM_SPINC; 
    pwmB_val = PWM_SPINC; 

    motorA_01_state = HIGH;
    motorA_02_state = LOW;
    motorB_01_state = HIGH;
    motorB_02_state = LOW;

    updateMotors();

    last_pwmA_val= pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = BACKWARD;
    
}

// turning LEFT means moving motor A faster than motor B 
void left()
{
    printf("%s\n", "left");

    if (lastCall == FORWARD) { 
        forward();
        pwmA_val = PWM_SPINC + PWM_INC;
        pwmB_val = PWM_SPINC - PWM_INC;
    }
    else if (lastCall == BACKWARD) {
        backward();
        pwmA_val = PWM_SPINC + PWM_INC;
        pwmB_val = PWM_SPINC - PWM_INC;
    }
    else {
        pwmA_val = PWM_INC * 2;
        pwmB_val = PWM_MIN;

        //go forward
        motorA_01_state = LOW;
        motorA_02_state = HIGH;
        motorB_01_state = LOW;
        motorB_02_state = HIGH;
    }

    updateMotors();
}

// turning RIGHT means moving motor B faster than motor A 
void right()
{
    printf("%s\n", "right");

    if (lastCall == FORWARD) { 
        forward();
        pwmA_val = PWM_SPINC - PWM_INC;
        pwmB_val = PWM_SPINC + PWM_INC;
    }
    else if (lastCall == BACKWARD) {
        backward();
        pwmA_val = PWM_SPINC - PWM_INC;
        pwmB_val = PWM_SPINC + PWM_INC;
    }
    else {
        pwmA_val = PWM_MIN;
        pwmB_val = PWM_INC * 2;

        //go forward
        motorA_01_state = LOW;
        motorA_02_state = HIGH;
        motorB_01_state = LOW;
        motorB_02_state = HIGH;
    }

    updateMotors();
}

///////////////////// STOPPING METHODS //////////////////

// Stops all motors and sets PWM to 0
void stop()
{
    printf("%s\n", "stop");

    pwmA_val = PWM_MIN;
    pwmB_val = PWM_MIN;

    motorA_01_state = LOW;
    motorA_02_state = LOW;
    motorB_01_state = LOW;
    motorB_02_state = LOW;

    updateMotors(); 

    last_pwmA_val= pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = STOP;
}

// Puts robot back into original FORWARD, BACKWARD, or STOP state
void stop_turning()
{
    printf("%s\n", "stop_turning");

    if (lastCall == FORWARD) { 
        forward();
    }
    else if (lastCall == BACKWARD) {
        backward();
    }
    else {
        stop();
    }
}

// Helper method to update the pins to the set global variables
void updateMotors() 
{
    softPwmWrite(motorA_pwm, pwmA_val); 
    softPwmWrite(motorB_pwm, pwmB_val); 

    digitalWrite(motorA_01, motorA_01_state);
    digitalWrite(motorA_02, motorA_02_state);
    digitalWrite(motorB_01, motorB_01_state);
    digitalWrite(motorB_02, motorB_02_state);

}
