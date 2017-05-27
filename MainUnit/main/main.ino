#include <SoftwareSerial.h>
SoftwareSerial rfSerial(2, 3); // RX, TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rfSerial.begin(1200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int addr; //addr to send to ( 0-4 ) 
  bool type_flag; //Type flag, false for Request true for command

  //For Testing...
  Serial.println("Loop");
  addr=0;
  for(addr=0;addr<4;addr++)
  {
    String Packet = createPacket(addr,type_flag,NULL);
    Serial.println(Packet);
  }



}


String createPacket(int addr, bool type, byte response) // Other flags ignored for now, can be used for expansion.
{
  String packet = "";
  packet.concat(String(addr));
  packet.concat( type == true ? "C":"R");
  packet.concat(String(int(response)));
}

