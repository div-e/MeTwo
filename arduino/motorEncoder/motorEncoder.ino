#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>
#include <Wire.h>

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

#define leftMotorOutputA 4
#define leftMotorOutputB 5
#define rightMotorOutputA 6
#define rightMotorOutputB 7

#define leftMotorSpeed 7
#define rightMotorSpeed 8

#define leftEncoderInputA 3
#define leftEncoderInputB 2 // 2 for interrupt
#define rightEncoderInputA 3 // TODO CHANGE THIS
#define rightEncoderInputB 2 // TODO CHANGE THIS

#define leftArmMotorOutputA 8
#define leftArmMotorOutputB 9
#define rightArmMotorOutputA 11
#define rightArmMotorOutputB 12

#define leftArmSpeed 12
#define rightArmSpeed 13
#define armSpeed 100 // 50-255
#define armThreshold 800 // TODO find magic threshold for deployment

#define motorSpeedOffset 4 // serial input for motor speed is offset by 4
// ranges 1-7 with 4 corresponding to no movemento

#define motorSpeedLevel0 0
#define motorSpeedLevel1 80
#define motorSpeedLevel2 160
#define motorSpeedLevel3 240

//#define xServoPin 9
//#define yServoPin 11

#define defaultServoPosition 90

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int dx, dy, xServoPosition, yServoPosition;

// keeps track of whether or not arm is deployed
/* bool leftArmDeployed = false; */
/* bool rightArmDeployed = false; */
bool armDeployed = false;

volatile long leftEncoderValue; // keeps track of left arm encoder value
volatile long rightEncoderValue; // keeps track of right arm encoder value

Servo xServo, yServo; // Servo vars
int inputIntArr[5];

void setup() {

    pinMode(rightMotorSpeed, OUTPUT);
    pinMode(leftMotorSpeed, OUTPUT);

    pinMode(leftMotorOutputA, OUTPUT);
    pinMode(leftMotorOutputB, OUTPUT);
    pinMode(rightMotorOutputA, OUTPUT);
    pinMode(rightMotorOutputB, OUTPUT);

    pinMode(leftArmMotorOutputA, OUTPUT);
    pinMode(leftArmMotorOutputB, OUTPUT);
    pinMode(rightArmMotorOutputA, OUTPUT);
    pinMode(rightArmMotorOutputB, OUTPUT);

    pinMode(leftEncoderInputA, INPUT);
    pinMode(rightEncoderInputA, INPUT);

    pinMode(leftArmSpeed, OUTPUT);
    pinMode(rightArmSpeed, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(leftEncoderInputB), leftEncoderEvent, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rightEncoderInputB), rightEncoderEvent, CHANGE);

    /* xServo.attach(xServoPin); */
    /* yServo.attach(yServoPin); */

    /* xServo.write(defaultServoPosition); */
    /* yServo.write(defaultServoPosition); */

    pwm.begin();
    pwm.setPWMFreq(60);
    yield();

    // Initialize camera to default position
    setServoPWM(defaultServoPosition, defaultServoPosition);

    Serial.begin (9600);
    Serial.setTimeout(50);
    while(!Serial.available());
}

void loop() {
    // infinite loop that pauses loop until new serial msg is available

    Serial.print("\nxServoPosition: ");
    Serial.println(xServoPosition);
    Serial.print("\nyServoPosition: ");
    Serial.println(yServoPosition);

    for (int i = 0 ; i < 5 ; i++){
      inputIntArr[i] = 0;
    }
    convertSerialStringToIntArray();

    setLeftMotor(inputIntArr[0]);
    setRightMotor(inputIntArr[1]);

    setXServo(inputIntArr[2]);
    setYServo(inputIntArr[3]);
    setServoPWM(xServoPosition, yServoPosition);

    setArmsState(inputIntArr[4]);
    stopArmThresholdCheck();
}

void convertSerialStringToIntArray(){
    char tempCharArr[5];

    if(Serial.available()){
      Serial.readString().toCharArray(tempCharArr, 6);
    }

    // Convert ASCII to ints
    for (int i = 0 ; i < 5 ; i++){
        inputIntArr[i] = tempCharArr[i] - 48;
        
        Serial.print("\ninputArrInt[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(inputIntArr[i]);

        Serial.print("\ntempCharArr[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(tempCharArr[i]);
        
    }
}

int mapMotorSpeedToPWM(int motorSpeed){
    switch (abs(motorSpeed)){
        case 0: return motorSpeedLevel0;
        case 1: return motorSpeedLevel1;
        case 2: return motorSpeedLevel2;
        case 3: return motorSpeedLevel3;
    }
}

void setLeftMotor(int motorSpeed){
    motorSpeed = motorSpeed - 4;
    analogWrite(leftMotorSpeed, mapMotorSpeedToPWM(motorSpeed));

    if (motorSpeed == 0){
        analogWrite(leftMotorOutputA, LOW);
        analogWrite(leftMotorOutputB, LOW);
    } else if (motorSpeed > 0){
        analogWrite(leftMotorOutputA, HIGH);
        analogWrite(leftMotorOutputB, LOW);
    } else {
        analogWrite(leftMotorOutputA, LOW);
        analogWrite(leftMotorOutputB, HIGH);
    }
}
void setRightMotor(int motorSpeed){
    motorSpeed = motorSpeed - 4;
    analogWrite(rightMotorSpeed, mapMotorSpeedToPWM(motorSpeed));

    if (motorSpeed == 0){
        analogWrite(rightMotorOutputA, LOW);
        analogWrite(rightMotorOutputB, LOW);
    } else if (motorSpeed > 0){
        analogWrite(rightMotorOutputA, HIGH);
        analogWrite(rightMotorOutputB, LOW);
    } else {
        analogWrite(rightMotorOutputA, LOW);
        analogWrite(rightMotorOutputB, HIGH);
    }
}

void setArmsState(int armState){
    bool boolArmState = (armState == 1);

    if(boolArmState != armDeployed){
        if(boolArmState){
            retractArms();
        } else {
            deployArms();
        }
    }
}

void deployArms(){
    analogWrite(leftArmSpeed, armSpeed);
    analogWrite(rightArmSpeed, armSpeed);

    // reset encoder values
    leftEncoderValue = 0;
    rightEncoderValue = 0;

    digitalWrite(leftArmMotorOutputA, LOW);
    digitalWrite(leftArmMotorOutputB, HIGH);
    digitalWrite(rightArmMotorOutputA, LOW);
    digitalWrite(rightArmMotorOutputB, HIGH);
}

void retractArms(){
    analogWrite(leftArmSpeed, armSpeed);
    analogWrite(rightArmSpeed, armSpeed);

    // reset encoder values
    leftEncoderValue = armThreshold;
    rightEncoderValue = armThreshold;

    digitalWrite(leftArmMotorOutputA, HIGH);
    digitalWrite(leftArmMotorOutputB, LOW);
    digitalWrite(rightArmMotorOutputA, HIGH);
    digitalWrite(rightArmMotorOutputB, LOW);
}

void stopArmThresholdCheck(){
    /*
    if (encoderValue > 800){
        PORTB &= B11111100; // lookup bit manipulation. PORTB maps to 13-8 so this
        // sets 8 and 9 to LOW which turns off motor 1
    }
    */

    // above code snippet is faster version of digitalWrite.

    if (leftEncoderValue < 0 || leftEncoderValue > armThreshold){
        digitalWrite(leftArmMotorOutputA, LOW);
        digitalWrite(leftArmMotorOutputB, LOW);
        if (leftEncoderValue < 0){
            armDeployed = false;
        } else {
            armDeployed = true;
        }
    }

    if (rightEncoderValue < 0 || rightEncoderValue > armThreshold){
        digitalWrite(rightArmMotorOutputA, LOW);
        digitalWrite(rightArmMotorOutputB, LOW);
        if (rightEncoderValue < 0){
            armDeployed = false;
        } else {
            armDeployed = true;
        }
    }
}

void leftEncoderEvent(){
    int encodeA = digitalRead(leftEncoderInputA);
    int encodeB = digitalRead(leftEncoderInputB);

    // Magic code that keeps track of how much the motor has spinned
    if (encodeA != 0){
        if (encodeB == 0){
            leftEncoderValue++;
        } else {
            leftEncoderValue--;
        }
    } else {
        if (encodeB == 0) {
            leftEncoderValue--;
        } else {
            leftEncoderValue++;
        }
    }
}

void rightEncoderEvent(){
    int encodeA = digitalRead(rightEncoderInputA);
    int encodeB = digitalRead(rightEncoderInputB);

    // Magic code that keeps track of how much the motor has spinned
    if (encodeA != 0){
        if (encodeB == 0){
            rightEncoderValue++;
        } else {
            rightEncoderValue--;
        }
    } else {
        if (encodeB == 0) {
            rightEncoderValue--;
        } else {
            rightEncoderValue++;
        }
    }
}

int mapServoStateToPosition(int state){
    return 10*(state-5);
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
    } else if (dx < 0){
        // move up
        yServoPosition = max(15, yServoPosition+dy);
    }
}


void setServoPWM(int xServoPosition, int yServoPosition){
    pwm.setPWM(0, 0, angleToPulse(xServoPosition));
    pwm.setPWM(0, 0, angleToPulse(yServoPosition));
}

int angleToPulse(int angle){
    int pulse = map(angle, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180
                                                        // to Servo min and
                                                        // Servo max

    return pulse;
}
