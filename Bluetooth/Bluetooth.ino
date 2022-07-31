#include<SoftwareSerial.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd(13,12,4,5,6,7);
SoftwareSerial bt(0,1);

int Blue;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Bluetooth Data");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bt.available())
  {
    Blue=bt.read();
    Serial.print(Blue);

    if(Blue=='a')
    {
      lcd.setCursor(0,1);
      lcd.print("A");
    }
    if(Blue=='b')
    {
      lcd.setCursor(0,1);
      lcd.print("B");
    }
    if(Blue=='c')
    {
      lcd.setCursor(0,1);
      lcd.print("C");
    }
    if(Blue=='d')
    {
      lcd.setCursor(0,1);
      lcd.print("D");
    }
    }
}
