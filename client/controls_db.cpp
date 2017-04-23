#include <stdio.h>
#include <stdexcept>
#include "controls.hpp"

#include <mraa/pwm.h>
#include <mraa/gpio.h>
#include <errno.h>

//////////////// VARIABLE DEFINITIONS ////////////////

// Motor Driver Pin declarations (Broadcom pinouts)
#define motorA_pwm 23 //GPIO12 (Pin 32) for Motor A PWM speed control
#define motorB_pwm 29 //GPIO26 (Pin 37) for Motor B PWM speed control
#define motorA_A 25
#define motorA_B 27 //GPIO06 (Pin 31) for Motor A 02 input
#define motorB_A 31 //GPIO13 (Pin 33) for Motor B 01 input
#define motorB_B 33 //GPIO19 (Pin 35) for Motor B 02 input
#define stdby 24 //GPIO21 (Pin 40) for Standby pin on Motor Driver
#define servo 26 //GPIO18 (Pin 12) for camera tilt

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
#define PWM_SERVO_MAX 100
#define PWM_SERVO_MIN 25
#define PWM_SINC 1

//Define LOW and HIGH
#define LOW  0;
#define HIGH  1;

    // Default pin modes
    float pwmA_val = PWM_MIN;
    float pwmB_val = PWM_MIN;
    float last_pwmA_val = PWM_MIN;
    float last_pwmB_val = PWM_MIN;
    int motorA_A_state = LOW;
    int motorA_B_state = LOW;
    int motorB_A_state = LOW;
    int motorB_B_state = LOW;
    float pwmServo_val = PWM_MIN;
    
    // Map all pins to GPIO context
    mraa_gpio_context motorA_A_io;
    mraa_gpio_context motorA_B_io;
    mraa_gpio_context motorB_A_io;
    mraa_gpio_context motorB_B_io;
    mraa_gpio_context stdby_io;
    mraa_pwm_context servo_io;
    mraa_pwm_context motorA_pwm_io;
    mraa_pwm_context motorB_pwm_io;

    // Default lastCall to STOP
    int lastCall = STOP;

    void updateMotors(void);

/////////////////// INITIALIZER ///////////////////
// initializes wiringPi and pins
void metwo::init()
{
    printf("%s\n", "Initializing...");

    if(getuid()!=0) //wiringPi requires root privileges  
    {  
        throw std::runtime_error("Error: wiringPi must be run as root.");
    } 

    // Map all pins to GPIO context
    motorA_A_io = mraa_gpio_init(motorA_A);
    motorA_B_io = mraa_gpio_init(motorA_B);
    motorB_A_io = mraa_gpio_init(motorB_A);
    motorB_B_io = mraa_gpio_init(motorB_B);
    stdby_io = mraa_gpio_init(stdby);
    servo_io = mraa_pwm_init(servo);
    motorA_pwm_io = mraa_pwm_init(motorA_pwm);
    motorB_pwm_io = mraa_pwm_init(motorB_pwm);


    // Set mode/direction for all GPIO context
    printf("%d",MRAA_SUCCESS==mraa_gpio_dir(motorA_A_io, MRAA_GPIO_OUT_LOW));
    mraa_gpio_dir(motorA_B_io, MRAA_GPIO_OUT_LOW);
    mraa_gpio_dir(motorB_A_io, MRAA_GPIO_OUT_LOW);
    mraa_gpio_dir(motorB_B_io, MRAA_GPIO_OUT_LOW);
    mraa_gpio_dir(stdby_io, MRAA_GPIO_OUT_HIGH);
    mraa_pwm_period_us(servo_io, 20);
    mraa_pwm_pulsewidth_ms(servo_io, 20);
    mraa_pwm_period_us(motorA_pwm_io, 20);
    mraa_pwm_pulsewidth_ms(motorA_pwm_io, 20);
    mraa_pwm_period_us(motorB_pwm_io, 20);
    mraa_pwm_pulsewidth_ms(motorB_pwm_io, 20);

// Default lastCall to STOP
int lastCall = STOP;

void updateMotors(void);

    stop();
}

//////////////////// UP and DOWN Tilting methods //////////////

// increments the camera tilt servo by SINC val
void metwo::up() {
    if(pwmServo_val + PWM_SINC <= PWM_SERVO_MAX) {
        pwmServo_val += PWM_SINC;
    } else {
        pwmServo_val = PWM_SERVO_MAX; 
    }
    printf("Up was called. Servo val: %d\n", pwmServo_val);
    mraa_pwm_write(servo_io, pwmServo_val);
}

// decrements the camera tilt servo by SINC val
void metwo::down() {
    if(pwmServo_val - PWM_SINC >= PWM_SERVO_MIN) {
        pwmServo_val -= PWM_SINC;
    } else {
        pwmServo_val = PWM_SERVO_MIN; 
    }
    printf("Down was called. Servo val: %d\n", pwmServo_val);
    mraa_pwm_write(servo_io, pwmServo_val);
}

//////////////////// FORWARD and BACKWARD METHODS /////////////

// makes the robot go forward 
void metwo::forward()
{
    printf("%s\n", "forward");

    pwmA_val = PWM_SPINC; 
    pwmB_val = PWM_SPINC; 

    motorA_A_state = LOW;
    motorA_B_state = HIGH;
    motorB_A_state = LOW;
    motorB_B_state = HIGH;

    updateMotors();

    last_pwmA_val = pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = FORWARD;
}

// makes the robot go backward 
void metwo::backward()
{
    printf("%s\n", "backward");

    pwmA_val = PWM_SPINC; 
    pwmB_val = PWM_SPINC; 

    motorA_A_state = HIGH;
    motorA_B_state = LOW;
    motorB_A_state = HIGH;
    motorB_B_state = LOW;

    updateMotors();

    last_pwmA_val= pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = BACKWARD;
    
}

// turning LEFT means moving motor A faster than motor B 
void metwo::left()
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
        motorA_A_state = LOW;
        motorA_B_state = HIGH;
        motorB_A_state = LOW;
        motorB_B_state = HIGH;
    }

    updateMotors();
}

// turning RIGHT means moving motor B faster than motor A 
void metwo::right()
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
        motorA_A_state = LOW;
        motorA_B_state = HIGH;
        motorB_A_state = LOW;
        motorB_B_state = HIGH;
    }

    updateMotors();
}

///////////////////// STOPPING METHODS //////////////////

// Stops all motors and sets PWM to 0
void metwo::stop()
{
    printf("%s\n", "stop");

    pwmA_val = PWM_MIN;
    pwmB_val = PWM_MIN;

    motorA_A_state = LOW;
    motorA_B_state = LOW;
    motorB_A_state = LOW;
    motorB_B_state = LOW;

    updateMotors(); 

    last_pwmA_val= pwmA_val;
    last_pwmB_val = pwmB_val;
    lastCall = STOP;
}

// Puts robot back into original FORWARD, BACKWARD, or STOP state
void metwo::stop_turning()
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
    mraa_pwm_write(motorA_pwm_io, pwmA_val);
    mraa_pwm_write(motorB_pwm_io, pwmB_val);

    mraa_gpio_write(motorA_A_io, motorA_A_state);
    mraa_gpio_write(motorA_B_io, motorA_B_state);
    mraa_gpio_write(motorB_A_io, motorB_A_state);
    mraa_gpio_write(motorB_B_io, motorB_B_state);
}

