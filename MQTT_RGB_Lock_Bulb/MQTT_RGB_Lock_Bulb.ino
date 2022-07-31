
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

#define bulbPin A1
#define lockPin1 A0
#define lockPin2 A2

void setup() {
  setColourRgb(0, 0, 0);
  pinMode(bulbPin, OUTPUT);
  pinMode(lockPin1, OUTPUT);
  pinMode(lockPin2, OUTPUT);
  digitalWrite(lockPin1, LOW);
  digitalWrite(lockPin2, LOW);
  digitalWrite(bulbPin, HIGH);

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
  unsigned int rgbColour[3] = {0, 0, 0};

  // listen for communication from the ESP8266 and then write it to the serial monitor
  if ( ESPserial.available() )
  {
    String topic = ESPserial.readString();
    switch (topic.charAt(17))
    {
      case 'u':
        {
          if (topic.charAt(23) == 'N')
          {
            digitalWrite(bulbPin, LOW);
            Serial.println("Bulb ON");
            Serial.println(digitalRead(A1));
          }
          if (topic.charAt(23) == 'F')
          {
            digitalWrite(bulbPin, HIGH);
            Serial.println("Bulb OFF");
            Serial.println(digitalRead(A1));
          }
          break;
        }
      case 'o':
        {
          if (topic.charAt(23) == 'N')
          {
            digitalWrite(lockPin1, LOW);
            digitalWrite(lockPin2, HIGH);
            Serial.println("Lock Open");
            delay(125);
            digitalWrite(lockPin1, LOW);
            digitalWrite(lockPin2, LOW);
          }
          if (topic.charAt(23) == 'F')
          {
            digitalWrite(lockPin1, HIGH);
            digitalWrite(lockPin2, LOW);
            Serial.println("Lock Close");
            delay(125);
            digitalWrite(lockPin1, LOW);
            digitalWrite(lockPin2, LOW);
          }
          break;
        }
      case 'e':
        {
          int loc = 0, num1, num2;
          for (int i = 1; i <= 6; i = i + 2)
          {
            num1 = (int)topic.charAt(21 + i);
            num2 = (int)topic.charAt(22 + i);
            if (num1 > 96)
            {
              num1 = num1 - 87;
            }
            else
            {
              num1 = num1 - 48;
            }
            if (num2 > 96)
            {
              num2 = num2 - 87;
            }
            else
            {
              num2 = num2 - 48;
            }
            rgbColour[loc] = (num1 * 16) + num2;
            loc++;
          }

          setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
          break;
        }
    }
  }
}

void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
  Serial.print("Red =");
  Serial.println(red);
  Serial.print("Green =");
  Serial.println(green);
  Serial.print("Blue =");
  Serial.println(blue);
}

