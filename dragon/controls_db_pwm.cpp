


#include "MRAA_PWMDriver.hpp"
#include "mraa.hpp"
#include "controls.hpp"

using namespace mraa;
using namespace metwo;

//////////////// VARIABLE DEFINITIONS ////////////////

// Motor Driver Pin declarations (Dragonboard GPIO Pinouts)
#define MOTOR_A_1_PIN 23 
#define MOTOR_A_2_PIN 25
#define MOTOR_B_1_PIN 24
#define MOTOR_B_2_PIN 26

#define STANDBY_PIN   27

// Servo Driver Pin declarations (Adafruit PWM I2c Pinouts) 
#define MOTOR_A_PWM_PIN 0
#define MOTOR_B_PWM_PIN 1
#define SERVO_TILT_PWM_PIN 2
#define SERVO_PAN_PWM_PIN 3


// Last call states 
#define FORWARD 1
#define BACKWARD 2
#define RIGHT 3
#define LEFT 4
#define STOP 0

//Define LOW and HIGH
#define LOW  0
#define HIGH  1

// PWM Constants
#define FREQ     60 // Servos run at 60 Hz. DC motor driver doesn't really care.

#define PWM_MIN  0
#define PWM_MAX  4000 // real max is 4095, but to make math nice here... 
#define SPEED_MIDPOINT 2500
#define SPEED_INCREMENT 500

#define SERVO_PWM_MIN 100
#define SERVO_PWM_MAX 200 //BASH (where is max rotation?) may write helper method to wrap.
#define SERVO_DEFAULT 150 
#define SERVO_INCREMENT 10

// Global pwm and i2c objects
MRAA_PWMDriver* pwm;
mraa::I2c* i2c; 

// Global gpio objects
mraa::Gpio* motor_A_1_gpio; 
mraa::Gpio* motor_A_2_gpio;
mraa::Gpio* motor_B_1_gpio; 
mraa::Gpio* motor_B_2_gpio; 

// Initial global variables
int lastCall = STOP; 

int motor_A_pwm_val = PWM_MIN;
int motor_B_pwm_val = PWM_MAX; 
int last_motor_A_pwm_val = PWM_MIN;
int last_motor_B_pwm_val = PWM_MIN; 

int motor_A_1_state = LOW; 
int motor_A_2_state = LOW; 
int motor_B_1_state = LOW; 
int motor_B_2_state = LOW; 

int servo_tilt_pwm_val = SERVO_DEFAULT; 
int servo_pan_pwm_val = SERVO_DEFAULT; 

// Helper Function Prototypes
void updateMotors(void);
int incrementSpeed(int originalSpeed); 
int decrementSpeed(int originalSpeed); 
int incrementServoPos(int originalServoPos); 
int decrementServoPos(int originalServoPos);


/////////////////// INITIALIZER ///////////////////
void metwo::init()
{
  printf("%s\n", "Initializing...");
  
  printf("h\n");
  motor_A_1_gpio = new mraa::Gpio(MOTOR_A_1_PIN);
  printf("hi\n");
  motor_A_2_gpio = new mraa::Gpio(MOTOR_A_2_PIN); 
  printf("hii\n");
  motor_B_1_gpio = new mraa::Gpio(MOTOR_B_1_PIN); 
  printf("hiii\n");
  motor_B_2_gpio = new mraa::Gpio(MOTOR_B_2_PIN); 
  printf("hiiii\n");

  motor_A_1_gpio->dir(DIR_OUT_LOW); 
  motor_A_2_gpio->dir(DIR_OUT_LOW); 
  motor_B_1_gpio->dir(DIR_OUT_LOW); 
  motor_B_2_gpio->dir(DIR_OUT_LOW); 

  i2c = new mraa::I2c(0);

  pwm = new MRAA_PWMDriver(DEFAULT_ADDRESS); 

  pwm->begin(); 

  pwm->setPWMFreq(FREQ); 

  updateMotors(); 
  printf("done initializing\n");
}






//////////////////// WEBCAM Tilting Panning methods //////////////

// increments the camera tilt servo by SERVO_INCREMENT
void metwo::up() {
  printf("Up was called. Servo val: %d\n", servo_tilt_pwm_val); 
  
  servo_tilt_pwm_val = incrementServoPos(servo_tilt_pwm_val); 

  updateMotors(); 
}

// decrements the camera tilt servo by SERVO_INCREMENT
void metwo::down() {
  printf("Down was called. Servo val: %d\n", servo_tilt_pwm_val);
  
  servo_tilt_pwm_val = decrementServoPos(servo_tilt_pwm_val); 

  updateMotors();
}

// pans right (increments the pan servo value) 
void metwo::panRight() {   
  printf("Pan Right was called. Servo val: %d\n", servo_pan_pwm_val);

  servo_pan_pwm_val = incrementServoPos(servo_pan_pwm_val); 

  updateMotors(); 
}

// pans left (decrements the pan servo value) 
void metwo::panLeft() {
  printf("Pan Left was called. Servo val: %d\n", servo_pan_pwm_val);

  servo_pan_pwm_val = decrementServoPos(servo_pan_pwm_val); 

  updateMotors(); 
}





//////////////////// DRIVING  METHODS /////////////

// makes the robot go forward 
void metwo::forward()
{
  printf("%s\n", "forward");

  motor_A_1_state = HIGH; 
  motor_A_2_state = LOW; 
  motor_B_1_state = HIGH; 
  motor_B_2_state = LOW; 

  motor_A_pwm_val = SPEED_MIDPOINT; 
  motor_B_pwm_val = SPEED_MIDPOINT; 

  updateMotors();

  last_motor_A_pwm_val = motor_A_pwm_val; 
  last_motor_B_pwm_val = motor_B_pwm_val; 
  lastCall = FORWARD;
}

// makes the robot go backward 
void metwo::backward()
{
  printf("%s\n", "backward");

  motor_A_1_state = LOW; 
  motor_A_2_state = HIGH; 
  motor_B_1_state = LOW; 
  motor_B_2_state = HIGH; 

  motor_A_pwm_val = SPEED_MIDPOINT; 
  motor_B_pwm_val = SPEED_MIDPOINT; 

  updateMotors();

  last_motor_A_pwm_val = motor_A_pwm_val; 
  last_motor_B_pwm_val = motor_B_pwm_val; 
  lastCall = BACKWARD;
    
}

// turning LEFT means moving motor A faster than motor B 
void metwo::left()
{
  printf("%s\n", "left");

  if (lastCall == FORWARD) { 
    forward();
    motor_A_pwm_val = incrementSpeed(SPEED_MIDPOINT); 
    motor_B_pwm_val = decrementSpeed(SPEED_MIDPOINT); 
  }
  else if (lastCall == BACKWARD) {
    backward();
    motor_A_pwm_val = incrementSpeed(SPEED_MIDPOINT); 
    motor_B_pwm_val = decrementSpeed(SPEED_MIDPOINT); 
  }
  else {
    // Turn on right wheel pivot
    motor_A_pwm_val = incrementSpeed(SPEED_INCREMENT); 
    motor_B_pwm_val = decrementSpeed(SPEED_INCREMENT); 

    // go forward
    motor_A_1_state = HIGH;
    motor_A_2_state = LOW; 
    motor_B_1_state = HIGH; 
    motor_B_2_state = LOW; 
  }

  updateMotors();

}

// turning RIGHT means moving motor B faster than motor A 
void metwo::right()
{
  printf("%s\n", "right");

  if (lastCall == FORWARD) { 
    forward();
    motor_A_pwm_val = decrementSpeed(SPEED_MIDPOINT); 
    motor_B_pwm_val = incrementSpeed(SPEED_MIDPOINT); 
  }
  else if (lastCall == BACKWARD) {
    backward();
    motor_A_pwm_val = decrementSpeed(SPEED_MIDPOINT); 
    motor_B_pwm_val = incrementSpeed(SPEED_MIDPOINT); 
  }
  else {
    // Turn on right wheel pivot
    motor_A_pwm_val = decrementSpeed(SPEED_INCREMENT); 
    motor_B_pwm_val = incrementSpeed(SPEED_INCREMENT); 

    // go forward
    motor_A_1_state = HIGH;
    motor_A_2_state = LOW; 
    motor_B_1_state = HIGH; 
    motor_B_2_state = LOW; 
  }

  updateMotors();
}





///////////////////// STOPPING METHODS //////////////////

// Stops all motors and sets PWM to 0
void metwo::stop()
{
  printf("%s\n", "stop");

  motor_A_pwm_val = PWM_MIN;
  motor_B_pwm_val = PWM_MIN;

  motor_A_1_state = LOW;
  motor_A_2_state = LOW;
  motor_B_1_state = LOW;
  motor_B_2_state = LOW;

  updateMotors(); 

  last_motor_A_pwm_val = motor_A_pwm_val;
  last_motor_B_pwm_val = motor_B_pwm_val;
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


// Close nicely. 
void terminate(void) {
  stop(); 
  delete(pwm);
  delete(i2c); 
  delete(motor_A_1_gpio); 
  delete(motor_A_2_gpio); 
  delete(motor_B_1_gpio); 
  delete(motor_B_2_gpio); 
}

// Helper method to update the pins to the set global variables
void updateMotors(void) 
{
  motor_A_1_gpio->write(motor_A_1_state);
  motor_A_2_gpio->write(motor_A_2_state);
  motor_B_1_gpio->write(motor_B_1_state);
  motor_B_2_gpio->write(motor_B_2_state);

  pwm->setPWM(MOTOR_A_PWM_PIN, 0, motor_A_pwm_val); 
  pwm->setPWM(MOTOR_B_PWM_PIN, 0, motor_B_pwm_val); 
  pwm->setPWM(SERVO_TILT_PWM_PIN, 0, servo_tilt_pwm_val);
  pwm->setPWM(SERVO_PAN_PWM_PIN, 0, servo_pan_pwm_val); 
}

int incrementSpeed(int originalSpeed) {
  if(originalSpeed + SPEED_INCREMENT <= PWM_MAX) {
    originalSpeed += SPEED_INCREMENT; 
  } else {
    originalSpeed = PWM_MAX; 
  }

  return originalSpeed; 
}

int decrementSpeed(int originalSpeed) {
  if(originalSpeed - SPEED_INCREMENT >= PWM_MIN) {
    originalSpeed -= SPEED_INCREMENT; 
  } else {
    originalSpeed = PWM_MIN; 
  }

  return originalSpeed; 
}


int incrementServoPos(int originalServoPos) {
  if(originalServoPos + SERVO_INCREMENT <= SERVO_PWM_MAX) {
    originalServoPos += SERVO_INCREMENT; 
  } else {
    originalServoPos = SERVO_PWM_MAX; 
  } 

  return originalServoPos; 
}


int decrementServoPos(int originalServoPos) {
  if(originalServoPos - SERVO_INCREMENT >= SERVO_PWM_MIN) {
    originalServoPos -= SERVO_INCREMENT; 
  } else {
    originalServoPos = SERVO_PWM_MIN; 
  } 

  return originalServoPos; 
}

