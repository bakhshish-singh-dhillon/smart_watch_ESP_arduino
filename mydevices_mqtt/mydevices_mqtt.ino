#include <CayenneMQTTESP8266.h>
#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial

char ssid[] = "belkin.3d74";
char password[] = "Yellow@123";

char username[] = "182165d0-518d-11e7-99a9-9908f7dc6bae";
char mqtt_password[] = "24e634bbd44be068a451d4d9f0788d56162f6073";
char client_id[] = "ba8e4ca0-519e-11e7-8ab6-097b71ab053c";

void setup() {
  // put your setup code here, to run once:
  Cayenne.begin(username, mqtt_password, client_id, ssid, password);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Cayenne.loop();
}

CAYENNE_IN(0)
{
  digitalWrite(2, !getValue.asInt());
}

