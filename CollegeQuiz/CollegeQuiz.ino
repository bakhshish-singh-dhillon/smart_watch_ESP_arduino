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
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  DDRD = 0x00;
  DDRB = 0xfb;
  DDRC = 0xff;
}

// the loop function runs over and over again forever
void loop() {
  PORTD=0x00;
  PORTB=0x00;
  PORTC=0x00;
  while(PIND == 0x00);
  switch(PIND)
  {
    case 0x01:
    {
      PORTC=0x01;
      break;
    }
    
    case 0x02:
    {
      PORTC=0x02;
      break;
    }
    
    case 0x04:
    {
      PORTC=0x04;
      break;
    }
    
    case 0x08:
    {
      PORTC=0x08;
      break;
    }
    
    case 0x10:
    {
      PORTC=0x10;
      break;
    }
    
    case 0x20:
    {
      PORTC=0x20;
      break;
    }

    case 0x40:
    {
      PORTB=0x01;
      break;
    }
    
    case 0x80:
    {
      PORTB=0x02;
      break;
    }
  }
  while(PINB==0x00);
}

