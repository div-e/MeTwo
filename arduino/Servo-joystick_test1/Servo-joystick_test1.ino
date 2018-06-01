#include <Servo.h>
Servo xservo, yservo;
int dx,dy,xpos=90,ypos=90;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  xservo.attach(9);
  yservo.attach(11);
}

void loop() {
String STR="";
char x[45];
  Serial.println("dx=");
  while(!Serial.available());
  STR=Serial.readString(); //read the sting in the buffer and store it into DX

  STR.toCharArray(x, 45);  //convert string to char array
  
  Serial.println(x[13]);  //print out the integers
  Serial.println(x[28]);
  Serial.println(x[35]);
  Serial.println(x[43]);
  Serial.println("Values");
  int RM=x[13]-48; //Convert ASCII to Integers
  int LF=x[28]-48;
  int dx=x[35]-48;
  int dy=x[42]-48;
  Serial.println(RM);     //print the values of the integers after they are stored
  Serial.println(LF);
  Serial.println(dx);
  Serial.println(dy);
  //Control the Camera
  dx=dx-5;    //take away the offset
  dy=dy-5;
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
      
  
}
 /* Char=Serial.read();
      if (isDigit(Char)) {
      // convert the incoming byte to a char and add it to the string:
      DX += (char)Char;
      dx=DX.toInt();
  Serial.println(dx);
  Serial.println("dy=");
  while(!Serial.available());
  Char=Serial.read();
      if (isDigit(Char)) {
      // convert the incoming byte to a char and add it to the string:
      DY += (char)Char;
  dy=DY.toInt();
  Serial.println(dy);
  if(dx>0)
  {
    pos=min(180,pos+dx); //move to the right
  }
  if(dx<0)
  {
    pos=max(15,pos+dx);  //move to the left
  }
  if(dy>0)
  {
    pos2=max(15,pos2-dy); //move down
  }
  if(dy<0)
  {
    pos2=min(180,pos2-dy); //move up
  }
  Serial.print("pos=");
  Serial.println(pos);
  Serial.print("pos2=");
  Serial.println(pos2);
      }}}
*/
