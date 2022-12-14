
 // Basic serial communication with ESP8266
// Uses serial monitor for communication with ESP8266
//
//  Pins
//  Arduino pin 2 (RX) to ESP8266 TX
//  Arduino pin 3 to voltage divider then to ESP8266 RX
//  Connect GND from the Arduiono to GND on the ESP8266
//  Pull ESP8266 CH_PD HIGH
//
// When a command is entered in to the serial monitor on the computer 
// the Arduino will relay it to the ESP8266

#include <SoftwareSerial.h>
SoftwareSerial ESPserial(2, 3); // RX | TX

const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
unsigned int num;
unsigned int count;
unsigned int temp;
String input;

void setup() {
  num=0;
  count=0;
  setColourRgb(0,0,0);

  Serial.begin(9600);     // communication with the host computer
    //while (!Serial)   { ; }
 
    // Start the software serial for communication with the ESP8266
    ESPserial.begin(9600);  
 
    Serial.println("");
    Serial.println("Remember to to set Both NL & CR in the serial monitor.");
    Serial.println("Ready");
    Serial.println("");
}

void loop() {
  num=0;
  count=0;
  temp=0;
  unsigned int rgbColour[3];

  // listen for communication from the ESP8266 and then write it to the serial monitor
    if ( ESPserial.available() )   
      {  
        input=ESPserial.readString();
        unsigned int strSize=input.length();
        while(strSize)
        {
          temp=0;
          if(input.charAt(input.length()-strSize)!=' ')
          {
              temp=input.charAt(input.length()-strSize)-48;
              num=num*10+temp;              
          }
          if(input.charAt(input.length()-strSize)==' ')
          {
            rgbColour[count]=255-num;
            count++;
            num=0;
          }
          strSize--;
        }
        setColourRgb(rgbColour[0],rgbColour[1],rgbColour[2]);
      }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  Serial.println("Red =");
  Serial.println(red);
  Serial.println("Green =");
  Serial.println(green);
  Serial.println("Blue =");
  Serial.println(blue);
 }

