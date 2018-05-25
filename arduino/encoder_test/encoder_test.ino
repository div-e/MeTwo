//#define encoder0PinA  2
//#define encoder0PinB  3
int pwm= 9;
int in1= 8;
int in2= 10;

//volatile unsigned int encoder0Pos = 0;

void setup() {
  //pinMode(encoder0PinA, INPUT_PULLUP);

  pinMode(pwm, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  // encoder pin on interrupt 0 (pin 2)
  //attachInterrupt(0, doEncoderA, CHANGE);

  Serial.begin (9600);
}

void loop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  //analogWrite(pwm, 256);
  
  for (int i = 0 ; i < 256 ; i++){
    analogWrite(pwm, i);
    delay(20);
  }
  for (int i = 255 ; i >=0 ; i--){
    analogWrite(pwm, i);
    delay(20);
  }

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  for (int i = 0 ; i < 256 ; i++){
    analogWrite(pwm, i);
    delay(20);
  }
  for (int i = 255 ; i >=0 ; i--){
    analogWrite(pwm, i);
    delay(20);
  }
  
}
/*
void doEncoderA() {
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) {

    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  else   // must be a high-to-low edge on channel A
  {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == HIGH) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
  Serial.println (encoder0Pos, DEC);
  // use for debugging - remember to comment out
}

void doEncoderB() {
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else {
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {
      encoder0Pos = encoder0Pos + 1;          // CW
    }
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }
}
*/

