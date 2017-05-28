#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial rfSerial(2, 3); // RX, TX
LiquidCrystal_PCF8574 lcd(0x27); //Set lcd address

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rfSerial.begin(1200);
  lcdSetup();
  lcdShowStartGame();
}


bool state[3];
bool team[3];
int screen =0;


void loop() {
  // put your main code here, to run repeatedly:
  int addr; //addr to send to ( 0-4 ) 
  bool type_flag; //Type flag, false for Request true for command
  //For Testing...
  Serial.println("Polling Each Address for Current Score...");
  type_flag=false; //Request
  for(addr=1;addr<3;addr++)
  {
    String Packet = createPacket(addr,type_flag,byte(1));
    Serial.println("Sending: "+Packet);
    rfSerial.println(Packet);
    //Wait upto 5 Seconds for response.
    int i = 0;
    bool rec = false;
    for(i=0;i<3;i++)
    {
      if(rfSerial.available())
      {
        rec=true;
        break;
      }
      updateLCD();
      delay(1000);
    }
    if(!rec)
    {
      Serial.println("No Packet recieved in 5 Seconds from Addr #" + addr);
      state[addr]=false;
      continue;
    }
    else
    {
      
      delay(50);
      String RecP = String(rfSerial.read());
      state[addr]=true;
      int Res[3];
      parsePacket(RecP,0,Res);
      if(char(Res[1])=='B')
      {
        //Give points to Blue here
      }
      else
      {
        //give points to Red.
      }
    }
  }

  updateLCD();
  //TODO: Check win condition.
  //TODO: Do Horn Blasts.
  //TODO: Reset Condition
  //Reset all Slaves
  addr=0;
  type_flag=true; //Command
  byte cmd = byte('R'); // R for Reset
  for(addr=1;addr<4;addr++)
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
        state[addr]=true;
        break;
      }
      updateLCD();
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
      state[addr]=false;
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
  packet.concat(String(char(response)));
  return packet;
}




void lcdSetup()
{
  lcd.begin(20,4);
  lcd.setBacklight(255);
}

void lcdShowStartGame()
{
  lcd.clear();
  lcd.home();
  lcd.print("  AIRSOFT CONQUEST");
  lcd.setCursor(0,1);
  lcd.print("  BEGIN IN:");
  int i =10;
  for(i=10;i>0;i--)
  {
    lcd.setCursor(12,1);
    lcd.print(" "+String(i)+"  ");
    delay(1000);
  }
  lcd.clear();
  lcd.home();
  lcd.print("    GAME STARTED");
  for(i=0;i<20;i++)
  {
    lcd.setBacklight(0);
    delay(200);
    lcd.setBacklight(255);
    delay(200);
  }
  lcd.clear();
}

void lcdHeartBeat()
{
  lcd.clear();
  lcd.home();
  lcd.setBacklight(255);
  lcd.print("Heart Beat");
  int i=0;
  for(i=0;i<3;i++)
  {
    lcd.setCursor(0,i+1);
    lcd.print("B"+String(i)+" ");
    lcd.print(state[i]?"  PULSE ACTIVE  ":" PULSE INACTIVE ");
    
  }
}




void updateLCD()
{
  switch(screen)
  {
    case(0):
    {
      //main score page
    }
    case(1):
    {
      //heartbeat page
      lcdHeartBeat();
    }
  }
  screen=(screen+1)%2;
}
















