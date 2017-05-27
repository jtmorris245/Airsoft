//FOR TESTING THE FET
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Waiting for Serial");
  while(!Serial.available())
  {delay(100);}
  Serial.println("A for on, S for off");
  pinMode(13,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available())
  {
    Serial.println(Serial.peek());
    int res = Serial.read();
    if(res == 97)
    {
        digitalWrite(13,HIGH);
        Serial.println("On");
      }
     else if(res == 115)
      {
        digitalWrite(13,LOW);
        Serial.println("Off"); 
      }
  }
}
