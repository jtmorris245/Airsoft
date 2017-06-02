#include <Event.h>
#include <Timer.h>

//HC-12 messenger send/receive
//autor Tom Heylen tomtomheylen.com

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); //RX, TX
  String sendpacket; //to send 
  String Address; 
  String Myaddress;
  String ReqorCom; //requst R = arduino sends points Command C = arduino reset
  String points; //How many points each team has
  String BaseCapturedBy; //stores who has captured the base "B" or "R" (red/ blue)
  int ticktock; //incremented every iteration of full loop
  int ticktocktime=10; //number of iterations before dominace increase/decreas.
  int dominance; //who has the base (-14 = red 14 = blue)
  Timer t; //using a millis library which instead of using hard ware timer uses the time that the arduino has been running
  int bledEvent; //oscillate blue LEDS
  int rledEvent; //oscilate red LEDS
  bool x = true;
  int i = 1;//used for toggle buzzer
  
void setup() {
  points = ("0R0");
  Myaddress = "1"; //unique to this base
  Serial.begin(1200);
  mySerial.begin(1200);
  pinMode(4, INPUT_PULLUP);//toggle switch
  pinMode(5, INPUT_PULLUP);  //Red Button
  pinMode(13, OUTPUT);//LED red
  pinMode(10, OUTPUT);//LED blue
  pinMode(9, OUTPUT);//Buzzer  
  pinMode(12, OUTPUT);//ground
  pinMode(11, OUTPUT);//ground
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
}

void togglebuzzer()
{

   if (i == 1)
   {
    tone(9, 900); //buzzer on
    i = 0;
   }
   else if (i == 0)
   {
    noTone(9); //buzzer off
    i = 1;
   }
  
}
String basecaptured()
{
  String dominantteam;
  if (dominance > 13){
    dominantteam = "B";
    //Serial.println("Blue captured");
      digitalWrite(10,HIGH);
    }
  else if (dominance < -13){
    dominantteam = "R";
    digitalWrite(13,HIGH);
     //Serial.println("Red captured");
    }
  else 
  {
     dominantteam = "N";
  }    
  
  return dominantteam;
}
void resettick() //when ever tick gets high enough for dominace to increment (idealy 1 second)
{
    if (ticktock >ticktocktime) // RED CAPTURE for 1 second
    {
    ticktock = 0;
    togglebuzzer();
    }
    else if (ticktock <(0-ticktocktime)) // BLUE CAPTURE for 1 second
    {
    ticktock = 0;
    togglebuzzer();
    }    
}

void capture(){
  if ((digitalRead(4) == 1) && (dominance < 14)) // RED CAPTURING
  {
  ticktock ++;
  bledEvent = t.oscillate(13, 100, LOW);
  rledEvent = t.oscillate(10, 100,HIGH);
  t.update(); //required for timer library
 // Serial.println(ticktock);
  }
  if ((digitalRead(4) == 0) && (dominance > -14)) // BLUE CAPTURING
  {
  ticktock --;
  bledEvent = t.oscillate(13, 100, LOW);
  rledEvent = t.oscillate(10, 100,HIGH);
   t.update(); //required for timer library
   // Serial.println(ticktock);
  }
  if ((ticktock == (ticktocktime)) && (dominance < 14))//BLUE has captured for 1 ticktock and can increase in dominace
  {
    dominance ++;
    
    //Serial.println(dominance);
  }
    if ((ticktock < (0-ticktocktime)) && (dominance >-14) )//RED has captured for 1 ticktock and can increase in dominace
  {
    dominance --;
    //Serial.println(dominance);
  }
 resettick();

}

void loop() {
  
  BaseCapturedBy = basecaptured();
  //Serial.println(dominance);
    //Serial.println(digitalRead(5)); 
  if (digitalRead(5) == 0) //red button pushed
  {
    capture();
     x = true; //used to gauge if button has been depressed
  }

  if (digitalRead(5) == 1) //red button not pushed
  {
      if (x == true) //repeat only once after button release
      {
      digitalWrite(13,LOW);  //make sure LED returns LOW after oscillation
      digitalWrite(10,LOW);
      noTone(9); //buzzer off
      x = false;
      }
    
  }
  if(mySerial.available() > 1){//Read from HC-12 and send to serial monitor
    String input = mySerial.readString();
    Serial.println(input);
    Address = input.substring(0,1);
    ReqorCom = input.substring(1,2);
    
    if (Address == Myaddress) {
    
      if (ReqorCom == "R") {
        sendpacket = (Myaddress + "R" + BaseCapturedBy); 
        mySerial.println(sendpacket); 
       
      }
      else if (ReqorCom == "C") {
         Serial.println("reset");
         dominance = 0;      
      }
    }

    }

  
  delay(20);
}
