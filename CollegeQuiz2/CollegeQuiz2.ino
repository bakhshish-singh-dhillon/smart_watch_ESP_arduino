/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/


// the setup function runs once when you press reset or power the board
int flag;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  flag=0;
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(16,INPUT);

  pinMode(14,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(28,OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(0)==HIGH)
  {
    digitalWrite(14,LOW);
    flag=1;  
  }
  if(digitalRead(1)==HIGH)
  {
    digitalWrite(15,LOW);  
    flag=1;
  }
  if(digitalRead(2)==HIGH)
  {
    digitalWrite(23,LOW);  
    flag=1;
  }
  if(digitalRead(3)==HIGH)
  {
    digitalWrite(24,LOW);  
    flag=1;
  }
  if(digitalRead(4)==HIGH)
  {
    digitalWrite(25,LOW);  
    flag=1;
  }
  if(digitalRead(5)==HIGH)
  {
    digitalWrite(26,LOW);  
    flag=1;
  }
  if(digitalRead(6)==HIGH)
  {
    digitalWrite(27,LOW);
    flag=1;  
  }
  if(digitalRead(7)==HIGH)
  {
    digitalWrite(28,LOW);
    flag=1;  
  }
  while(flag==1)
  {
    if(digitalRead(16)==HIGH)
    {
      flag=0;
    }
  }
}

