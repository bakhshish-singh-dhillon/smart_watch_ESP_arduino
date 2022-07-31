#include "SSD1306.h"
#include <Wire.h> 
SSD1306  display(0x3c, D3, D5);
void setup() {
  Serial.begin(115200);
  display.init();

  display.setContrast(255);

  display.clear();
  display.flipScreenVertically();

}
unsigned int val;
void loop()
{
val = analogRead(A0);
float mv = ( val/1024.0)*5000; 
float cel = (mv/10)-15;
float farh = (cel*9)/5 + 32;

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Temperature Sensor");
  String str="Farhenhiet=";
  str+=farh;
  display.drawString(0, 20, str);
  str="Celcius=";
  str+=cel;
  display.drawString(0, 30,str);
  delay(1000);
  display.display();

  display.clear();
}
