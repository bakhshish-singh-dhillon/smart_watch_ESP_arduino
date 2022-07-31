#include<SoftwareSerial.h>

// LED connected to pin 4 on Arduino
int LEDpin = 4;

// Create softserial object GSM, on pins 5/RX & 6/TX
SoftwareSerial GSM(5, 6);

void setup()
{
  pinMode(LEDpin, OUTPUT);
  GSM.begin(9600);               // the GSM baud rate
  Serial.begin(9600);             // the Serial port of Arduino baud rate.
  delay(1000);                  
  GSM.print("AT+CLIP=1\r\n");    
  delay(1000);
}

void loop() {
  checkSMS();
  delay(100);
}

void checkSMS() {
  char smsData[10];        // Buffer for SMS text
  while (GSM.available()) {  
    Serial.println("Got data over GSM serial");
    // sequentially check the characters for +CMT, which is received when a new SMS arrives
    // This only works when the GSM modem is set to directly send the SMS to serial port. 
    // AT command to set this mode is AT+CNMI=3,2,0,0
    Serial.println(GSM.read());
  }
}
