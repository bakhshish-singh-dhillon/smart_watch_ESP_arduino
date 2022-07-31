#include<Servo.h>

Servo serv;

int loc;

void setup() {
  // put your setup code here, to run once:
serv.attach(9);
pinMode(10,INPUT);
pinMode(11,INPUT(;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(10) == LOW)
  {
    for(loc=0;loc<=180;loc+=20)
    {
    serv.write(loc);
    delay(400);
    }
  }
  if(digitalRead(11) == LOW)
  {
    for(loc=180;loc>=0;loc-=20)
    {
      serv.write(loc);
      delay(400);
    }
  }
}
