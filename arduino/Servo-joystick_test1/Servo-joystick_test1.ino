int dx,dy,pos=90,pos2=90, Char;

void setup() {
  Serial.begin(9600);
}

void loop() {
String DX="", DY="";
  Serial.println("dx=");
  while(!Serial.available());
  Char=Serial.read();
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

