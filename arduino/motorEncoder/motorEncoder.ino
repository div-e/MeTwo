#define leftMotorOutputA 4
#define leftMotorOutputB 5
#define rightMotorOutputA 6
#define rightMotorOutputB 7

#define leftMotorSpeed 7
#define rightMotorSpeed 8

#define leftEncoderInputA 3
#define leftEncoderInputB 2 // 2 for interrupt
#define rightEncoderInputA 3 // TODO CHANGE THIS
#define rightEncoderInputA 2 // TODO CHANGE THIS

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

volatile long leftEncoderValue; // keeps track of left arm encoder value
volatile long rightEncoderValue; // keeps track of right arm encoder value

int prevLeftMotor = -1;
int prevRightMotor = -1;
int prevDeploy = -1;

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

    Serial.begin (9600);
}

void loop() {
    // infinite loop that pauses loop until new serial msg is available
    while(!Serial.available());

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

    if (leftEncodeValue < 0 || leftEncodeValue > armThreshold){
        digitalWrite(leftArmMotorOutputA, LOW);
        digitalWrite(leftArmMotorOutputB, LOW);
    }

    if (rightEncodeValue < 0 || rightEncodeValue > armThreshold){
        digitalWrite(rightArmMotorOutputA, LOW);
        digitalWrite(rightArmMotorOutputB, LOW);
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
