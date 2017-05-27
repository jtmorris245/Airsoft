//FOR TESTING THE FET

void setup() {
  // put your setup code here, to run once:
  Serial scon= new Serial(9600);
  scon.writeLine("Waiting for A");
  while(scon.read() != "A")
  {delay(100);}
  scon.writeLine("A for on, S for off");
  pinMode(13,OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(scon.availible();)
  {
    if(scon.read() ="A")
      {
        digitalWrite(13,HIGH);
      }
     else if(scon.read() = "S")
      {
        digitalWrite(13,LOW); 
      }
  }
}
