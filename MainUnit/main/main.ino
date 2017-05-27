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
  Serial.println("Polling Each Address for Current Score...");
  type_flag=false; //Request
  int[4] scoreR;
  int[4] scoreB;
  for(addr=0;addr<4;addr++)
  {
    String Packet = createPacket(addr,type_flag,byte(1));
    Serial.println("Sending: "+Packet);
    rfSerial.println(Packet);
    //Wait upto 5 Seconds for response.
    int i = 0;
    bool rec = false;
    for(i=0;i<5;i++)
    {
      if(rfSerial.available())
      {
        rec=true;
        break;
      }
    }
    if(!rec)
    {
      Serial.println("No Packet recieved in 5 Seconds from Addr #" + addr);
      continue;
    }
    else
    {
      delay(250);
      String RecP = String(rfSerial.read());
      // Plug in the parser here/
      //scoreR[addr]=rec;
      //scoreB[addr]=rec;
    }
  }
  //TODO: Display Update.
  //TODO: Check win condition.
}


String createPacket(int addr, bool type, byte response) // Other flags ignored for now, can be used for expansion.
{
  String packet = "";
  packet.concat(String(addr));
  packet.concat( type == true ? "C":"R");
  packet.concat(String(int(response)));
  return packet;
}

