#include <SoftwareSerial.h>
SoftwareSerial rfSerial(2, 3); // RX, TX
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rfSerial.begin(1200);
}
int state[4];
void loop() {
  // put your main code here, to run repeatedly:
  int addr; //addr to send to ( 0-4 ) 
  bool type_flag; //Type flag, false for Request true for command
  //For Testing...
  Serial.println("Polling Each Address for Current Score...");
  type_flag=false; //Request
  int ScoreR[4];
  int scoreB[4];
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
      delay(1000);
    }
    if(!rec)
    {
      Serial.println("No Packet recieved in 5 Seconds from Addr #" + addr);
      state[addr]=0;
      continue;
    }
    else
    {
      
      delay(50);
      String RecP = String(rfSerial.read());
      state[addr]=0;
      int Res[3];
      parsePacket(RecP,0,Res);
      //scoreR[addr]=rec;
      //scoreB[addr]=rec;
    }
  }
  //TODO: Display Update.
  //TODO: Check win condition.
  //TODO: Do Horn Blasts.
  //TODO: Reset Condition
  //Reset all Slaves
  addr=0;
  type_flag=true; //Command
  byte cmd = byte('R'); // R for Reset
  for(addr=0;addr<4;addr++)
  {
    String Packet = createPacket(addr,type_flag,cmd);
    Serial.println("Sending Reset Command Packet:"+Packet);
    rfSerial.println(Packet);
    int i =0;
    bool rec = false;
    for(i=0;i<5;i++)
    {
      if(rfSerial.available())
      {
        rec=true;
        break;
      }
      delay(1000);
    }
    if(!rec)
    {
      Serial.println("No Packet recieved in 5 Seconds from Addr #" + addr);
      continue;
    }
    else
    {
      delay(50);
      String RecP = String(rfSerial.read());
      state[addr]=0;
      int Res[3];
      parsePacket(RecP,0,Res);
      

    }
    
  }
  
  
}


void parsePacket(String packet,int addr_cur,int *Res)
{
  int addr = packet.substring(0,1).toInt();
  if(addr_cur != addr)
  { //Packet isn't for us, return null
    return NULL;
  }
  bool type = packet.substring(1,2)=="R" ? false : true;
  byte Resp =0;
  if(packet.length() > 3)
  {
    byte Resp = NULL;
    int len = packet.length() - 2;
    packet.substring(2,3).getBytes(Resp,len);
  }
  Res[0]=addr;
  Res[1]=(int)type;
  Res[2]=(int)Resp;
  
}


String createPacket(int addr, bool type, byte response) // Other flags ignored for now, can be used for expansion.
{
  String packet = "";
  packet.concat(String(addr));
  packet.concat( type == true ? "C":"R");
  packet.concat(String(int(response)));
  return packet;
}


























