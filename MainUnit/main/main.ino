#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial rfSerial(2, 3); // RX, TX
LiquidCrystal_PCF8574 lcd(0x27); //Set lcd address
int score_init = 750; //inital scores
int scoreR=0;
int scoreB=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rfSerial.begin(1200);
  lcdSetup();
  resetBases();
  lcdShowStartGame();
  pinMode(10,OUTPUT);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  scoreR=score_init;
  scoreB=score_init;
  
}

int screentimer=0;
//<<<<<<< HEAD

int Reddominance; //how many bases red have captured
int Bluedominance;
bool state[3]; //
int team[3]; //1 -B, 2-R 0-N
int screen =0;
//>>>>>>> origin/master

int ticketbleed3 = 15; //Score per tick if a team has full dominance
int ticketbleed2 = 10; //Score per tick if a team has dominance
int ticketbleed1 = 5;



void loop() {
  checkInputs();
  screentimer++;
  int addr; //addr to send to ( 0-4 ) 
  bool type_flag; //Type flag, false for Request true for command
  //For Testing...
  Serial.println("Polling Each Address for Current Score...");
  type_flag=false; //Request
  for(addr=1;addr!=4;addr++)
  {
    String Packet = createPacket(addr,type_flag,byte(0));
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
      updateLCD();
      delay(1000);
    }
    if(!rec)
    {
      Serial.println("FAIL");
      state[addr-1]=false;
      continue;
    }
    else
    {
      
      delay(50);
      String RecP =rfSerial.readString();
      Serial.println(RecP);
      Serial.println("SUCCESS");
      state[addr-1]=true;
      String Res=parsePacket(RecP,0);
      //Serial.println(Res);
      if(Res=="R")
      {
        team[addr-1]=2;
      }
      else if(Res == "B")
      {
        team[addr-1]=1;
      }
      else if(Res =="N")
      {
        team[addr-1]=0;
      }
      //Serial.println(team[addr-1]);
    }
  }
  updateScores();
  checkInputs();
  updateLCD();
  if(scoreR<0||scoreB<0) 
  {
    endgame();
    digitalWrite(10,HIGH);
    delay(250);
    digitalWrite(10,LOW);
    delay(250);
    digitalWrite(10,HIGH);
    delay(250);
    digitalWrite(10,LOW);
    digitalWrite(10,HIGH);
    delay(2000);
    digitalWrite(10,LOW);
    delay(250);
    digitalWrite(10,HIGH);
    delay(250);
    digitalWrite(10,LOW);
    while(!digitalRead(5))
    {
      delay(250);
      resetBases();
    }
    while(true)
    {
      endgame();
    }
    
  }
  
  
  
  
  
  
  
  
  
  
  
  //Reset all Slaves
 
}


String parsePacket(String packet,int addr_cur)
{
  int addr = packet.substring(0,1).toInt();
  /*if(addr_cur != addr)
  { //Packet isn't for us, return null
    return "";
  }
  */
  bool type = packet.substring(1,2)=="R" ? false : true;
  byte Resp =0;
  if(packet.length() > 3)
  {
    byte Resp = NULL;
    int len = packet.length() - 2;
    packet.substring(2,3).getBytes(Resp,len);
  }
  return packet.substring(2,3);
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
  lcd.print("     Heart Beat     ");
  int i=0;
  for(i=0;i<3;i++)
  {
    lcd.setCursor(0,i+1);
    lcd.print("B"+String(i+1)+" ");
    lcd.print(state[i]?"  PULSE ACTIVE  ":" PULSE INACTIVE ");
    
  }
}

void lcdPointScreen()
{
  lcd.clear();
  lcd.home();
  lcd.print("SCORES AS THEY STAND");
  lcd.setCursor(0,1);
  lcd.print("    RED      BLUE   ");
  String sr = String(scoreR);
  String sb = String(scoreB);
  lcd.setCursor(0,2);
  String insideSpaces ="";
  int i=0;
  i = 9 - sr.length();
  for(i=i;i!=0;i--)
  {
    insideSpaces += " ";
  }
  lcd.print("    "+sr+insideSpaces+sb);
  if(sr==sb)
  {
    lcd.setCursor(0,3);
    lcd.print("  Scores are Tied");
    return;
  }
  lcd.setCursor(0,3);
  lcd.print(sr>sb?"   Red is Winning":"  Blue is Winning");
}

void updateScores()
{
  int possesion;
  Reddominance = 0;
  Bluedominance = 0;  
  for (int x = 0; x != 4; x++)
  {
    if ((team[x]) == 2)
    {
      Reddominance++;
    }
    else if((team[x]) == 1)
    {
      Bluedominance++;
    }
  }
  //Serial.println(Bluedominance);
  //Serial.println(Reddominance);
  possesion = Bluedominance - Reddominance; //find who has more bases. 1 = blue has more -1 for red has more 2 or -2 means one has total dominance
  //Serial.println(possesion);
  switch (possesion) {
    case -3:
      scoreB -= ticketbleed3;
      break;
    case -2:
      scoreB -= ticketbleed2;
      break;
    case -1:  
      scoreB -= ticketbleed1;
      break;
    case 1:
      scoreR -= ticketbleed1;
      break;
    case 2:
      scoreR -= ticketbleed2;
      break;  
    case 3:
      scoreR -= ticketbleed3;
    break;
  }
}

//================================================================
void endgame()
{
  if (scoreB < 0)
  {
    lcd.clear();
    lcd.home();
    lcd.print("   GAME OVER   ");
    lcd.setCursor( 0,1);
    lcd.print("   Red Team wins   ");
  }
  if (scoreR < 0)
  {
    lcd.clear();
    lcd.home();
    lcd.print("   GAME OVER   ");
    lcd.setCursor(0,1);
    lcd.print("   Blue Team wins   ");
  }
}

void lcdDominanceScreen()
{
  lcd.clear();
  lcd.home();
  lcd.print("     DOMINANCE     ");
  String s ="";
  int i =0;
  for(i=0;i<3;i++)
  {
    lcd.setCursor(0,i+1);
    if(state[i])
    {
      switch(team[i])
      {
        case(0):
        {
          s="NEUTRAL";
          break;
        }
        case(1):
        {
          s="  BLUE ";
          break;
        }
        case(2):
        {
          s="  RED  ";
          break;
        }
        default:
        {
          s=" ERROR ";
          break;
        }
      }
    }
    else
    {
      s="DISCONN";
    }
    lcd.print("B"+String(i+1)+" : "+s);
  }
}

void updateLCD()
{
  //Serial.println(screentimer);
  if(screentimer > 1)
  {
    screen++;
    screentimer=0;
  }
  if(screen>6){screen=0;}
  //screen =1;
  switch(screen)
  {
    case(0):
    {
      //main score page
      lcdPointScreen();
      break;
    }
    case(1):
    {
      //heartbeat page
      //lcdHeartBeat();
      break;
    }
    case(2):
    {
      lcdDominanceScreen();
      break;
    }
    case(3):
    {
      lcdPointScreen();
      break;
    }
    case(4):
    {
      //main score page
      lcdPointScreen();
      break;
    }
     case(5):
    {
      //main score page
      lcdPointScreen();
      break;
    }
     case(6):
    {
      //main score page
      lcdPointScreen();
      break;
    }
    default:
    {
      lcdPointScreen();
      break;
    }
  }
}

void checkInputs()
{
  if(!digitalRead(4))
  {
    screen=1;
    updateLCD();
  }
  else
  {
    //screen=0;
    updateLCD();
  }
  if(!digitalRead(5))
  {
    //do reset
    resetBases();
    lcd.clear();
    lcd.home();
    lcd.print("TURN OFF ");
    lcd.setCursor(0,1);
    lcd.print("AND ON TO RESTART");
  }
  //Serial.println("inputs");
  //Serial.println(digitalRead(4));
  //Serial.println(digitalRead(5));
}



void resetBases()
{
  int addr=0;
  bool type_flag=true; //Command
  byte cmd = byte('R'); // R for Reset
  for(addr=1;addr<4;addr++)
  {
    String Packet = createPacket(addr,type_flag,cmd);
    Serial.println("Sending Reset Command Packet:"+String(Packet));
    rfSerial.println(Packet);
    int i =0;    
    delay(1000);
  }
  
  
  
}









