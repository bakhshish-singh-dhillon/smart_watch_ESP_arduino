#include<Servo.h>

Servo serv;

int loc=0;

void setup() {
  // put your setup code here, to run once:
serv.attach(9);
pinMode(12,INPUT);
pinMode(13,INPUT);
digitalWrite(12,HIGH);
digitalWrite(13,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(12) == LOW)
  {
    loc=180;
    serv.write(loc);
  }
  if(digitalRead(13) == LOW)
  {
      loc=0;
      serv.write(loc);
  }
}
