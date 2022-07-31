#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "belkin.3d74";
const char* password = "Yellow@123";

ESP8266WebServer server(80);

String webPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  webPage += "<form action='/RGB' method='get'>  Red <input type='text' name='red'><br>";
  webPage += "Green <input type='text' name='green'><br> Blue <input type='text' name='blue'><br>";
  webPage +="<button type='submit'>Submit</button></form>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/temp", temp);
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 

void temp()
{
  String str;
  if ( Serial.available() )       
  {  
    str=Serial.readString();
    String temp="";
    unsigned int i=0;
    while(str.charAt(i)!='*')
    {
      temp+=str.charAt(i);
      i++;
    }
    str=temp;
  }
  server.send(200, "text/html", str);
  Serial.println(str);
  str="";
}


