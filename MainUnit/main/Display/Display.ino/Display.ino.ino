#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int show;

void setup()
{
  int error;

  Serial.begin(9600);
  Serial.println("LCD...");

  while (! Serial);

  Serial.println("Dose: check for LCD");

  // See http://playground.arduino.cc/Main/I2cScanner
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 4); // initialize the lcd
  show = 0;
} // setup()

void loop()
{
  show=1;
  Serial.println(show);
  if (show == 0) {
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    lcd.print("  AIRSOFT CONQUEST");
    lcd.setCursor(0,1);
    lcd.print("BEGIN IN:");
    int i =10;
    while(i!=0)
    {
      lcd.setCursor(9,1);
      lcd.print(String(i)+" ");
      delay(500);
      i--;
    }
    lcd.clear();
    lcd.home();
    lcd.print("       BEGIN!");
    i=0;
    delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255); delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255); delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255); delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255); delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255); delay(400);
      lcd.setBacklight(0);
      delay(400);
      lcd.setBacklight(255);
  } else if (show == 1) {
    lcd.clear();
    lcd.print("Game is Running");
    bool states[4] = {true,false,true,false};
    bool teams[4] = {true,false,true,false};
    String strings[4];
    
    delay(100);
    
    int i=0;
    
    lcd.setCursor(0,1);
    strings[i]="B";
    strings[i]+=String(i);
    strings[i]+=states[i]?"  UP  ":" DOWN ";
    strings[i]+=teams[i]?" RED  ":" BLUE ";
    lcd.print(strings[i]);
    delay(500);
    

  } else if (show == 2) {
    lcd.clear();
    lcd.print("Cursor Blink");
    lcd.blink();

  } else if (show == 3) {
    lcd.clear();
    lcd.print("Cursor OFF");
    lcd.noBlink();
    lcd.noCursor();

  } else if (show == 4) {
    lcd.clear();
    lcd.print("Display Off");
    lcd.noDisplay();

  } else if (show == 5) {
    lcd.clear();
    lcd.print("Display On");
    lcd.display();

  } else if (show == 7) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*** first line.");
    lcd.setCursor(0, 1);
    lcd.print("*** second line.");

  } else if (show == 8) {
    lcd.scrollDisplayLeft();
  } else if (show == 9) {
    lcd.scrollDisplayLeft();
  } else if (show == 10) {
    lcd.scrollDisplayLeft();
  } else if (show == 11) {
    lcd.scrollDisplayRight();
  } // if

  delay(2000);
  show = (show + 1) % 12;
} // loop()
