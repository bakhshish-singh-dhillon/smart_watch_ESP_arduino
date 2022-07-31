/*
 * Hello world web server
 * circuits4you.com
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


//SSID and Password to your ESP Access Point
const char* ssid = "esp1";
const char* password = "12345678";
 
//setting the addresses
IPAddress ip(192,168,8,1);
IPAddress gateway(192, 168, 8, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80); //Server on port 80
 

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(2,!(digitalRead(2)));
}
 
//===============================================================
//                  SETUP
//===============================================================
void setup(void){
  Serial.begin(9600);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  WiFi.config(ip,gateway,subnet);
  WiFi.begin();
  
pinMode(2,OUTPUT);
 
  Serial.print("HotSpt IP:");
  Serial.println(ip);
 
  server.on("/", handleRoot);      //Which routine to handle at root location
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//===============================================================
//                     LOOP
//===============================================================
void loop(void){
  server.handleClient();          //Handle client requests
}
