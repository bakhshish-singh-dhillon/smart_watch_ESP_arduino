/*
 
 PUBSUB MQTT library
 
 
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
  Code from Andreas Spiess
*/

#define LEDBLUE 16

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define ssid          "WR3005N3-33FA"
#define password      "waheguru"
#define SERVER          "io.adafruit.com"
#define SERVERPORT      1883
#define MQTT_USERNAME   "bakhshish"
#define MQTT_KEY        "3cc5be7739be479a8477308f49b2a1a4"
#define USERNAME          "bakhshish/"
#define PREAMBLE          "feeds/"




WiFiClient WiFiClient;
// create MQTT object
PubSubClient client(WiFiClient);

int ledState=1;
int prevLedState=1;
char postData[4];
//
void setup() {

  pinMode(LEDBLUE, OUTPUT);
  pinMode(D6, INPUT);
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);
}

//
void loop() {
  //Serial.println("loop");
  yield();
  if (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("", MQTT_USERNAME, MQTT_KEY)) {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(USERNAME PREAMBLE "Bulb", 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  if (digitalRead(D6)==LOW) 
  {
    while(digitalRead(D6)==LOW);
    ledState=!ledState;
  }

  if (client.connected() && ledState!=prevLedState) 
  {
    Serial.println("Publish Led State");
    String state="";
    if(ledState==0)
    {
      state="OFF";
    }
    if(ledState==1)
    {
      state="ON";
    }
    state.toCharArray(postData, 4);
    client.publish(USERNAME PREAMBLE "Bulb", postData);
    prevLedState=ledState;
    delay(500);
  }
//
//  if (client.connected()&& prevClientStatus != clientStatus ) {
//    Serial.println("Publish Status");
//
//    String hi = (String)clientStatus;
//    hi.toCharArray(valueStr, 2);
//    client.publish(USERNAME PREAMBLE T_CLIENTSTATUS, valueStr);
//    prevClientStatus = clientStatus;
//  }
  client.loop();
}


//----------------------------------------------------------------------


void callback(char* topic, byte * data, unsigned int length) {
  // handle message arrived {

  Serial.print(topic);
  Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();
  if (data[1] == 'F')  {
    digitalWrite(LEDBLUE, LOW);
    ledState=0;
  } else {
    digitalWrite(LEDBLUE, HIGH);
    ledState=1;
  }
}
