  #include <Servo.h>
  #include <Adafruit_PWMServoDriver.h>
  #include <Wire.h>
  #define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
  #define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  
  Servo xservo, yservo;
  int dx,dy,xpos=90,ypos=90;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  xservo.attach(9);
  yservo.attach(11);
  
  xservo.write(xpos);   //Initialize camera to default position
  yservo.write(ypos);
  
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  yield();  
  
  pwm.setPWM(0, 0, angleToPulse(xpos) );    //Initialize Camera to default position
  pwm.setPWM(1, 0, angleToPulse(ypos) );
  
}

void loop() {
  String STR="";
  char x[45];
  Serial.println("dx=");
  while(!Serial.available());
  STR=Serial.readString(); //read the sting in the buffer and store it into DX

  STR.toCharArray(x, 6);  //convert string to char array
  
  int RM=x[0]-48; //Convert ASCII to Integers
  int LF=x[1]-48;
  int dx=x[2]-48;
  int dy=x[3]-48;
  Serial.println(RM);     //print the values of the integers after they are stored
  Serial.println(LF);
  Serial.println(dx);
  Serial.println(dy);
  //Control the Camera
  dx=10*(dx-5);    //take away the offset and multiply
  dy=10*(dy-5);
  
  if(dx>0)
  {
    xpos=min(180,xpos+dx); //move to the right
  }
  if(dx<0)
  {
    xpos=max(15,xpos+dx);  //move to the left
  }
  if(dy>0)
  {
    ypos=max(15,ypos-dy); //move down
  }
  if(dy<0)
  {
    ypos=min(180,ypos-dy); //move up
  }
  Serial.print("Xpos=");
  Serial.println(xpos);
  Serial.print("Ypos=");
  Serial.println(ypos);
  xservo.write(xpos);
  yservo.write(ypos);
  pwm.setPWM(0, 0, angleToPulse(xpos) );
  pwm.setPWM(1, 0, angleToPulse(ypos) );
  
}

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  
   return pulse;
}
