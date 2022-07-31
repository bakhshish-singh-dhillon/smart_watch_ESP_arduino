#include<SoftwareSerial.h>
char data = 0;                //Variable for storing received data
SoftwareSerial portOne(10, 11);
void setup() 
{
  Serial.begin(9600);         //Sets the data rate in bits per second (baud) for serial data transmission
  pinMode(13, OUTPUT);        //Sets digital pin 13 as output pin
  portOne.begin(9600);
}
void loop()
{
  if(portOne.available() > 0)  // Send data only when you receive data:
  {
    data = portOne.read();      //Read the incoming data and store it into variable data
    Serial.print(data);        //Print Value inside data in Serial monitor
    Serial.print("\n");        //New line 
    if(data == 'A')            //Checks whether value of data is equal to 1 
      digitalWrite(13, HIGH);  //If value is 1 then LED turns ON
    else if(data == 'E')       //Checks whether value of data is equal to 0
      digitalWrite(13, LOW);   //If value is 0 then LED turns OFF
  }                            
 
}
