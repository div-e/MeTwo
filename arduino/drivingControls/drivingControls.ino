#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SERVO_MIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVO_MAX  575 // this is the 'maximum' pulse length count (out of 4096)

// Motor output pins for movement.
#define LEFT_MOTOR_A_PIN 3
#define LEFT_MOTOR_B_PIN 2
#define RIGHT_MOTOR_A_PIN 4
#define RIGHT_MOTOR_B_PIN 7

// PWM pins for motor speed.
#define LEFT_MOTOR_SPEED_PIN 5
#define RIGHT_MOTOR_SPEED_PIN 6

// Interrupt pins for encoder input to track amount of motor rotation.
#define LEFT_ENCODER_A_PIN 22
#define LEFT_ENCODER_B_PIN 18
#define RIGHT_ENCODER_A_PIN 23
#define RIGHT_ENCODER_B_PIN 19

// Motor output pins for arm movement.
#define LEFT_ARM_MOTOR_A_PIN 8
#define LEFT_ARM_MOTOR_B_PIN 11
#define RIGHT_ARM_MOTOR_A_PIN 13
#define RIGHT_ARM_MOTOR_B_PIN 12

// PWM pins for arm speed.
#define LEFT_ARM_SPEED_PIN 9
#define RIGHT_ARM_SPEED_PIN 10

// PWM value for arm speed.
#define ARM_SPEED 100 // 50-255 is range of usable values

// Amount that arms should rotate for deployment (unknown units).
#define ARM_DEPLOYMENT_THRESHOLD_VAL 1000 // TODO find magic threshold for deployment

// Offset for motorSpeed serial input.
#define INPUT_OFFSET 4 // serial input for motor speed is offset by 4
// motorSpeed ranges 1-7 with 4 corresponding to no movement.

// PWM Values for motor speed levels.
#define MOTOR_SPEED_LEVEL_0 0
#define MOTOR_SPEED_LEVEL_1 80
#define MOTOR_SPEED_LEVEL_2 160
#define MOTOR_SPEED_LEVEL_3 240

// Default servo position.
#define DEFAULT_SERVO_POSITION 90

// Constants for serial input array.
#define NUM_CONTROLS 5
#define LEFT_DRIVE_STATE_INDEX 0
#define RIGHT_DRIVE_STATE_INDEX 1
#define PAN_SERVO_STATE_INDEX 2
#define TILT_SERVO_STATE_INDEX 3
#define DEPLOY_STATE_INDEX 4

// Servo driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// How many ticks to change per iteration.
int dx, dy;
// Actual servo position to write to servo.
int xServoPosition, yServoPosition;

// Keeps track of status of arm deployment.
bool armDeployed = false;

// Keep track of whether or not to keep track of encoder value.
bool trackEncoder = false;

// Keep track of arm deployment status.
bool armDeploymentInProgress = false;

// Keeps track of encoder values of left and right arms.
volatile long leftArmEncoderValue;
volatile long rightArmEncoderValue;

// Array that stores serial input as integers.
int inputIntArr[5];

void setup() {

    // Initialize pin I/O.
    pinMode(RIGHT_MOTOR_SPEED_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_SPEED_PIN, OUTPUT);

    pinMode(LEFT_MOTOR_A_PIN, OUTPUT);
    pinMode(LEFT_MOTOR_B_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_A_PIN, OUTPUT);
    pinMode(RIGHT_MOTOR_B_PIN, OUTPUT);

    pinMode(LEFT_ARM_MOTOR_A_PIN, OUTPUT);
    pinMode(LEFT_ARM_MOTOR_B_PIN, OUTPUT);
    pinMode(RIGHT_ARM_MOTOR_A_PIN, OUTPUT);
    pinMode(RIGHT_ARM_MOTOR_B_PIN, OUTPUT);

    pinMode(LEFT_ENCODER_A_PIN, INPUT);
    pinMode(RIGHT_ENCODER_A_PIN, INPUT);

    pinMode(LEFT_ARM_SPEED_PIN, OUTPUT);
    pinMode(RIGHT_ARM_SPEED_PIN, OUTPUT);

    // Attach interrupt to interrupt pins.
    attachInterrupt(digitalPinToInterrupt(LEFT_ENCODER_B_PIN), leftEncoderEvent, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RIGHT_ENCODER_B_PIN), rightEncoderEvent, CHANGE);

    pwm.begin();
    pwm.setPWMFreq(60);
    yield();

    // Initialize camera to default position
    xServoPosition = DEFAULT_SERVO_POSITION;
    yServoPosition = DEFAULT_SERVO_POSITION;
    setServoPWM(DEFAULT_SERVO_POSITION, DEFAULT_SERVO_POSITION);

    Serial.begin (9600);
    Serial.setTimeout(50);
    resetInputIntArr(inputIntArr);
    while(!Serial.available());
}

void loop() {
    // Infinite loop that pauses loop until new serial message is available.

    // Reset inputIntArr. (for debug purposes)
    //resetInputIntArr(inputIntArr);

    convertSerialStringToIntArray();

    // Set motor and servo values based on serial input.
    setLeftMotor(inputIntArr[LEFT_DRIVE_STATE_INDEX]);
    setRightMotor(inputIntArr[RIGHT_DRIVE_STATE_INDEX]);

    setXServo(inputIntArr[PAN_SERVO_STATE_INDEX]);
    setYServo(inputIntArr[TILT_SERVO_STATE_INDEX]);
    setServoPWM(xServoPosition, yServoPosition);

    bool inputDeployState = (inputIntArr[DEPLOY_STATE_INDEX] == 1);
    setArmsState(inputDeployState);

    // Check if arms have reached deployment threshold.
    //stopARM_DEPLOYMENT_THRESHOLD_VALCheck();
}

void resetInputIntArr(int * inputIntArr){
    inputIntArr[0] = 4;
    inputIntArr[1] = 4;
    inputIntArr[2] = 4;
    inputIntArr[3] = 4;
    inputIntArr[4] = 0;
}

void convertSerialStringToIntArray(){
    if(Serial.available()){
      // Array to store input as characters. Extra spot for null character.
      char tempCharArr[NUM_CONTROLS + 1];

      // Extra  store null termination character.
      Serial.readString().toCharArray(tempCharArr, NUM_CONTROLS + 1);

      // Convert ASCII to ints and store in inputIntArr
      for (int i = 0 ; i < NUM_CONTROLS ; i++){
          inputIntArr[i] = tempCharArr[i] - 48;
      }
    }
}

// Takes in int motorSpeed and maps it to a PWM value.
int mapMotorSpeedToPWM(int motorSpeed){
    switch (abs(motorSpeed)){
        case 0: return MOTOR_SPEED_LEVEL_0;
        case 1: return MOTOR_SPEED_LEVEL_1;
        case 2: return MOTOR_SPEED_LEVEL_2;
        case 3: return MOTOR_SPEED_LEVEL_3;
    }
}

// Set speed of left motor.
void setLeftMotor(int motorSpeed){
    motorSpeed = motorSpeed - 4;
    analogWrite(LEFT_MOTOR_SPEED_PIN, mapMotorSpeedToPWM(motorSpeed));

    if (motorSpeed == 0){
        digitalWrite(LEFT_MOTOR_A_PIN, LOW);
        digitalWrite(LEFT_MOTOR_B_PIN, LOW);
    } else if (motorSpeed > 0){
        digitalWrite(LEFT_MOTOR_A_PIN, HIGH);
        digitalWrite(LEFT_MOTOR_B_PIN, LOW);
    } else {
        digitalWrite(LEFT_MOTOR_A_PIN, LOW);
        digitalWrite(LEFT_MOTOR_B_PIN, HIGH);
    }
}

// Set speed of right motor.
void setRightMotor(int motorSpeed){
    motorSpeed = motorSpeed - 4;
    analogWrite(RIGHT_MOTOR_SPEED_PIN, mapMotorSpeedToPWM(motorSpeed));

    if (motorSpeed == 0){
        digitalWrite(RIGHT_MOTOR_A_PIN, LOW);
        digitalWrite(RIGHT_MOTOR_B_PIN, LOW);
    } else if (motorSpeed > 0){
        digitalWrite(RIGHT_MOTOR_A_PIN, HIGH);
        digitalWrite(RIGHT_MOTOR_B_PIN, LOW);
    } else {
        digitalWrite(RIGHT_MOTOR_A_PIN, LOW);
        digitalWrite(RIGHT_MOTOR_B_PIN, HIGH);
    }
}

// Set state of arms by delegated to retractArms and deployArms
void setArmsState(bool armState){
    if (!armDeploymentInProgress){
      return;
    } else {
      armDeploymentInProgress = true;
    }
    if(armState != armDeployed){
        if(armDeployed){
            retractArms();
        } else {
            deployArms();
        }
    }
}

void deployArms(){
    trackEncoder = true;
    Serial.println("deployArms");
    analogWrite(LEFT_ARM_SPEED_PIN, ARM_SPEED);
    analogWrite(RIGHT_ARM_SPEED_PIN, ARM_SPEED);

    // reset encoder values
    leftArmEncoderValue = 0;
    rightArmEncoderValue = 0;

    digitalWrite(LEFT_ARM_MOTOR_A_PIN, LOW);
    digitalWrite(LEFT_ARM_MOTOR_B_PIN, HIGH);
    //digitalWrite(RIGHT_ARM_MOTOR_A_PIN, LOW);
    //digitalWrite(RIGHT_ARM_MOTOR_B_PIN, HIGH);
}

void retractArms(){
    Serial.println("retractArms");
    trackEncoder = true;
    analogWrite(LEFT_ARM_SPEED_PIN, ARM_SPEED);
    analogWrite(RIGHT_ARM_SPEED_PIN, ARM_SPEED);

    // reset encoder values
    leftArmEncoderValue = ARM_DEPLOYMENT_THRESHOLD_VAL;
    rightArmEncoderValue = ARM_DEPLOYMENT_THRESHOLD_VAL;

    digitalWrite(LEFT_ARM_MOTOR_A_PIN, HIGH);
    digitalWrite(LEFT_ARM_MOTOR_B_PIN, LOW);
    //digitalWrite(RIGHT_ARM_MOTOR_A_PIN, HIGH);
    //digitalWrite(RIGHT_ARM_MOTOR_B_PIN, LOW);
}

// Check if arm movement has reached threshold yet.
void stopARM_DEPLOYMENT_THRESHOLD_VALCheck(){
    static int numArmsDeployed = 0;
    /*
    if (encoderValue > 800){
        PORTB &= B11111100; // lookup bit manipulation. PORTB maps to 13-8 so this
        // sets 8 and 9 to LOW which turns off motor 1
    }
    */

    // above code snippet is faster version of digitalWrite.

    if (leftArmEncoderValue < 0 || leftArmEncoderValue > ARM_DEPLOYMENT_THRESHOLD_VAL){
        Serial.println("leftArmOff");
        digitalWrite(LEFT_ARM_MOTOR_A_PIN, LOW);
        digitalWrite(LEFT_ARM_MOTOR_B_PIN, LOW);
        if (leftArmEncoderValue < 0){
            numArmsDeployed++;
            armDeployed = false;
        } else {
            numArmsDeployed++;
            armDeployed = true;
        }
    }

    if (rightArmEncoderValue < 0 || rightArmEncoderValue > ARM_DEPLOYMENT_THRESHOLD_VAL){
        Serial.println("rightArmOff");
        digitalWrite(RIGHT_ARM_MOTOR_A_PIN, LOW);
        digitalWrite(RIGHT_ARM_MOTOR_B_PIN, LOW);
        if (rightArmEncoderValue < 0){
            numArmsDeployed++;
            armDeployed = false;
        } else {
            numArmsDeployed++;
            armDeployed = true;
        }
    }

    if (numArmsDeployed == 2){
      numArmsDeployed = 0;
      armDeploymentInProgress = false;
    }
}

void leftEncoderEvent(){
    if (!trackEncoder) return;
    Serial.print("leftEncoderValue: ");
    Serial.println(leftArmEncoderValue);
    int encodeA = digitalRead(LEFT_ENCODER_A_PIN);
    int encodeB = digitalRead(LEFT_ENCODER_B_PIN);
    
    // Magic code that keeps track of how much the motor has spinned
    if (encodeA == HIGH){
        if (encodeB == LOW){
            leftArmEncoderValue++;
        } else {
            leftArmEncoderValue--;
        }
    } else {
        if (encodeB == LOW) {
            leftArmEncoderValue--;
        } else {
            leftArmEncoderValue++;
        }
    }
    stopARM_DEPLOYMENT_THRESHOLD_VALCheck();
}

void rightEncoderEvent(){
    if (!trackEncoder) return;
    Serial.print("rightEncoderValue: ");
    Serial.println(rightArmEncoderValue);
    int encodeA = digitalRead(RIGHT_ENCODER_A_PIN);
    int encodeB = digitalRead(RIGHT_ENCODER_B_PIN);

    // Magic code that keeps track of how much the motor has spinned
    if (encodeA != 0){
        if (encodeB == 0){
            rightArmEncoderValue++;
        } else {
            rightArmEncoderValue--;
        }
    } else {
        if (encodeB == 0) {
            rightArmEncoderValue--;
        } else {
            rightArmEncoderValue++;
        }
    }
    stopARM_DEPLOYMENT_THRESHOLD_VALCheck();
}

int mapServoStateToPosition(int state){
    return 10*(state-4);
}

void setXServo(int dx){
    dx = mapServoStateToPosition(dx);
    if (dx > 0){
        // move to right
        xServoPosition = min(180, xServoPosition+dx);
    } else if (dx < 0){
        // move to left
        xServoPosition = max(15, xServoPosition+dx);
    }
}

void setYServo(int dy){
    dy = mapServoStateToPosition(dy);
    if (dy > 0){
        // move down
        yServoPosition = min(180, yServoPosition+dy);
    } else if (dy < 0){
        // move up
        yServoPosition = max(15, yServoPosition+dy);
    }
}


void setServoPWM(int xServoPosition, int yServoPosition){
    pwm.setPWM(0, 0, angleToPulse(xServoPosition));
    pwm.setPWM(1, 0, angleToPulse(yServoPosition));
}

int angleToPulse(int angle){
    int pulse = map(angle, 0, 180, SERVO_MIN, SERVO_MAX); // map angle of 0 to 180
                                                        // to Servo min and
                                                        // Servo max

    return pulse;
}
