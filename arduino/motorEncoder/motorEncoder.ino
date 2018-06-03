#define encoderInputA 6 
#define encoderInputB 2 // 2 for interrupt
#define motorOutputA 8 
#define motorOutputB 9 
#define motorSpeed 10 // range 50-255 (<50 doesnt power motor)

volatile long encoderValue = 0; // keeps track of encoder value

void setup() {

  pinMode(motorSpeed, OUTPUT);
  
  pinMode(motorOutputA, OUTPUT);
  pinMode(motorOutputB, OUTPUT);

  pinMode(encoderInputA, INPUT);
  pinMode(encoderInputB, INPUT);

  digitalWrite(motorOutputA, LOW);
  digitalWrite(motorOutputB, HIGH);
  analogWrite(motorSpeed, 255); // TODO change magic number

  encoderValue = 0;

  attachInterrupt(0, encoderEvent, CHANGE); // 0 maps to pin 2
  
  Serial.begin (9600);
}

void loop() {
  Serial.println(encoderValue);
  if (encoderValue > 800){
    PORTB &= B11111100; // lookup bit manipulation. PORTB maps to 13-8 so this sets 8 and 9 to LOW
  }
}

// Execute upon change in encoderInputB
void encoderEvent(){
  int encodeA = digitalRead(encoderInputA);
  int encodeB = digitalRead(encoderInputB);

  // Magic code that keeps track of how much the motor has spinned
  if (encodeA != 0){
    if (encodeB == 0){
      encoderValue++;
    } else {
      encoderValue--;
    }
  } else {
    if (encodeB == 0) {
      encoderValue--;
    } else {
      encoderValue++;
    }
  }
}
