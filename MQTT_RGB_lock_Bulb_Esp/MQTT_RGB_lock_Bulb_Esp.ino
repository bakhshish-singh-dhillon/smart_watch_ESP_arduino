#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>

MDNSResponder mdns;

// Replace with your network credentials
#define ssid "WR3005N3-33FA"
#define password "waheguru"
#define SERVER          "io.adafruit.com"
#define SERVERPORT      1883
#define MQTT_USERNAME   "bakhshish"
#define MQTT_KEY        "3cc5be7739be479a8477308f49b2a1a4"
#define USERNAME          "bakhshish/"
#define PREAMBLE          "feeds/"

WiFiClient WiFiClient;
// create MQTT object
PubSubClient client(WiFiClient);

void setup(void){
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

  WiFi.printDiag(Serial);

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);
  
}
 
void loop(void)
{

  //Serial.println("loop");
  yield();
  if (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("", MQTT_USERNAME, MQTT_KEY)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(USERNAME PREAMBLE "Bulb", 1);
      client.subscribe(USERNAME PREAMBLE "Led", 1);
      client.subscribe(USERNAME PREAMBLE "Lock", 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  client.loop();
}


//----------------------------------------------------------------------


void callback(char* topic, byte * data, unsigned int length) {
  // handle message arrived {

  Serial.println(topic);
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)data[i]);
  }
  Serial.println();
} 

