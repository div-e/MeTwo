/*
 * This code will test the serial receiver. If data is recieved, 
 * it will blink the LED twice. 
 * 
 */

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Wait for data to be sent through serial.
  Serial.println("receiving data: ");
  while(!Serial.available());
  String str =Serial.readString(); 

  Serial.print("got: ");
  Serial.println(str);

  // Blink LED to show that data was received.
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for 1/2 a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for 1/2 a second
  
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);                       // wait for 1/2 a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);                       // wait for 1/2 a second

}
