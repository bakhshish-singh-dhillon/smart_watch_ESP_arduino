#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

ESP8266WebServer web;

char* ssid ="belkin.3d74";
char* pass ="Yellow@123"; 

void setup() {
  // put your setup code here, to run once:

  pinMode(16,OUTPUT);
  WiFi.begin(ssid,pass);
  Serial.begin(115200);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("IP Address:");
  Serial.println(WiFi.localIP());

  web.on("/",[](){web.send(200,"text/plain","Hello World");});
  web.on("/toggle",toggleLED);
  web.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
 web.handleClient();
}

void toggleLED()
{
  digitalWrite(16,!digitalRead(16));
  web.send(204,"");
}

