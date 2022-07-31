#include "SSD1306.h"
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

#include "RTClib.h"

RTC_DS1307 rtc;

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;

WiFiClient WiFiClient;
// create MQTT object
PubSubClient client(WiFiClient);
SSD1306  display(0x3c, D3, D5);

#define ssid          "WR3005N3-33FA"
#define password      "waheguru"
#define SERVER          "io.adafruit.com"
#define SERVERPORT      1883
#define MQTT_USERNAME   "bakhshish"
#define MQTT_KEY        "3cc5be7739be479a8477308f49b2a1a4"
#define USERNAME          "bakhshish/"
#define PREAMBLE          "feeds/"

//#define Pink "#ff0066"
//#define Green "#00ff00"
//#define Yellow "#ffff00"
//#define Orange "#ff3300"
//#define Red "#ff0009"
//#define Purple "#6600ff"
//#define Mint_Green "#00ff99"
//#define Dark_Blue "#0000bf"
//#define Neon_Blue "#0000ff"
//#define Light_Blue "#00ffff"

//months and da Arrays. Put in Language of your choice, omitt the 'da' part of the weekdays
char *months[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
char *days[] = {"Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri", "Satur" };

// date and time variables
byte m = 0;          // contains the minutes, refreshed each loop
byte h = 0;          // contains the hours, refreshed each loop
byte s = 0;         // contains the seconds, refreshed each loop
byte mo = 0;       // contains the months, refreshes each loop
int j = 0;           // contains the year, refreshed each loop
byte d = 0;         // contains the da (1-31)
byte da = 0;     // contains da of week (0-6)

boolean buttonClick;

String ledColor[] = {"#ff0066", "#00ff00", "#ffff00", "#ff3300", "#ff0009", "#6600ff", "#00ff99", "#0000bf", "#0000ff", "#00ffff"};
String ledColorName[] = {"", "Pink" , "Green", "Yellow", "Orange", "Red", "Purple", "Mint Green", "Dark Blue", "Neon Blue", "Light Blue", ""};

int tempValue;

int rectPosition;

int selection1;
int selection2;

String menuItem[7];
String options[4];

int displayTimer = 0;

void setup() {
  //For Clock
  Wire.pins(D3, D5);// yes, see text
  Wire.begin(D3, D5); // 0=sda, 2=scl
  rtc.begin();

  if (! rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

  // MQTT Code Starts
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  initSensors();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  client.setServer(SERVER, SERVERPORT);
  client.setCallback(callback);
  // MQTT Code Ends

  display.init();
  pinMode(D6, INPUT);
  pinMode(A0, INPUT);
  menuItem[0] = "";
  menuItem[1] = "Clock";
  menuItem[2] = "Pedometer";
  menuItem[3] = "Light";
  menuItem[4] = "Led";
  menuItem[5] = "Lock";
  menuItem[6] = "";

  options[0] = "";
  options[1] = "OFF";
  options[2] = "ON";
  options[3] = "";

  tempValue = 0;

  rectPosition = 0;

  selection1 = -1;
  selection2 = -1;

  buttonClick = false;

  display.setContrast(3255);

  display.clear();
  display.flipScreenVertically();
}

unsigned int val;
void loop()
{
  val = analogRead(A0);
  checkFlag();
  if (buttonClick == true)
  {
    if (selection1 < 0)
    {
      selection1 = rectPosition;
      buttonClick = false;
      displayTimer = millis();
    }
  }
  if (selection1 < 0)
  {
    if (tempValue != val / 140 )
    {
      if (tempValue > val / 140)
      {
        rectPosition--;
      }
      if (tempValue < val / 140)
      {
        rectPosition++;
      }
      tempValue = val / 140;
      if (rectPosition < 0 )
      {
        rectPosition = 0;
      }
      if (rectPosition > 4)
      {
        rectPosition = 4;
      }
      drawMenu(rectPosition);
      Serial.println(rectPosition);
      Serial.println(val);
      Serial.println(tempValue);
    }
  }
  else
  {
    drawOptions();
  }
}

void initSensors()
{
  if (!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  if (!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while (1);
  }
}

void drawLedColor()
{
  checkFlag();
  if (buttonClick == true)
  {
    if (selection2 < 0)
    {
      selection2 = rectPosition;
      buttonClick = false;
    }
  }
  if (selection2 < 0)
  {
    if (tempValue != val / 80 )
    {
      if (tempValue > val / 80)
      {
        rectPosition--;
      }
      if (tempValue < val / 80)
      {
        rectPosition++;
      }
      tempValue = val / 80;
      if (rectPosition < 0 )
      {
        rectPosition = 0;
      }
      if (rectPosition > 9)
      {
        rectPosition = 9;
      }
      Serial.println(rectPosition);
      Serial.println(val);
      Serial.println(tempValue);
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 0, "Choose Color");
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 15, ledColorName[rectPosition]);
      display.setFont(ArialMT_Plain_24);
      display.drawString(0, 25, ledColorName[rectPosition + 1]);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 50, ledColorName[rectPosition + 2]);
      display.display();
    }
  }
  else
  {
    Serial.println(selection1);
    Serial.println(selection2);
    selection1 = -1;
    selection2 = -1;
    char tempSub[4];
    char tempData[8];
    String tempLed = "Led";
    tempLed.toCharArray(tempSub, 4);
    ledColor[rectPosition].toCharArray(tempData, 8);
    rectPosition = 0;
    tempValue = 0;
    subscribeTopic();
    delay(500);
    publishData(tempSub, tempData);
    delay(500);
    display.clear();
  }
}

void pedo()
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;

  /* Calculate the altitude using the barometric pressure sensor */
  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */
    float temperature;
    bmp.getTemperature(&temperature);
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Alt:");
    display.setFont(ArialMT_Plain_16);
    display.drawString(40, 0, String(bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure, temperature)));
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 25, "Temp:");
    display.setFont(ArialMT_Plain_24);
    display.drawString(60, 25, String(temperature));
    display.display();
  }
  checkFlag();
  if (millis() - displayTimer > 10000 || buttonClick == true)
  {
    selection1 = -1;
    buttonClick = false;
    drawMenu(rectPosition);
  }
  delay(100);
}

void drawOnOffChoice(String sub)
{
  checkFlag();
  if (buttonClick == true)
  {
    if (selection2 < 0)
    {
      selection2 = rectPosition;
      buttonClick = false;
    }
  }
  if (selection2 < 0)
  {
    if (tempValue != val / 240 )
    {
      if (tempValue > val / 240)
      {
        rectPosition--;
      }
      if (tempValue < val / 240)
      {
        rectPosition++;
      }
      tempValue = val / 240;
      if (rectPosition < 0 )
      {
        rectPosition = 0;
      }
      if (rectPosition > 1)
      {
        rectPosition = 1;
      }
      Serial.println(rectPosition);
      Serial.println(val);
      Serial.println(tempValue);
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 0, sub);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 15, options[rectPosition]);
      display.setFont(ArialMT_Plain_24);
      display.drawString(0, 25, options[rectPosition + 1]);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 50, options[rectPosition + 2]);
      display.display();
    }
  }
  else
  {
    Serial.println(selection1);
    Serial.println(selection2);
    selection1 = -1;
    selection2 = -1;
    char tempSub[6];
    char tempData[4];
    sub.toCharArray(tempSub, 5);
    rectPosition++;
    options[rectPosition].toCharArray(tempData, 4);
    rectPosition = 0;
    tempValue = 0;
    subscribeTopic();
    publishData(tempSub, tempData);
    delay(500);
    display.clear();
  }
}

void drawMenu(int pos)
{
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Menu");
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 15, menuItem[pos]);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 25, menuItem[pos + 1]);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 50, menuItem[pos + 2]);
  display.display();
  delay(50);
}

void drawOptions()
{
  switch (selection1)
  {
    case 0:
      {
        esp_clock();
        break;
      }
    case 1:
      {
        pedo();
        break;
      }
    case 2:
      {
        drawOnOffChoice("Bulb");
        break;
      }

    case 3:
      {
        drawLedColor();
        break;
      }
    case 4:
      {
        drawOnOffChoice("Lock");
        break;
      }
  }
}



void esp_clock()
{
  display.clear();
  String amPm = "AM";
  DateTime now = rtc.now();
  da = now.dayOfTheWeek();
  j = now.year();
  mo = now.month();
  d = now.day();
  h = now.hour();
  if (h > 12)
  {
    h = h - 12;
    amPm = "PM";
  }
  m = now.minute();
  s = now.second();


  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  String t = String(h) + ":" + String(m) + ":" + String(s);
  String t2 = String(d) + ":"  + String(mo) + ":" + String(j);
  display.drawString(25, 20, t);
  display.setFont(ArialMT_Plain_16);
  display.drawString(105, 20, amPm);
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, t2);
  display.drawString(80, 0, months[mo - 1]);
  String d = days[da];
  d = d + "day";//adding the word 'da' (=da)  to the names of the days
  display.drawString(35, 45, d);
  // write the buffer to the display
  display.display();
  checkFlag();
  if (millis() - displayTimer > 10000 || buttonClick == true)
  {
    selection1 = -1;
    buttonClick = false;
    drawMenu(rectPosition);
  }
  delay(10);
}

void checkFlag()
{
  if (digitalRead(D6) == LOW)
  {
    while (digitalRead(D6) == LOW);
    buttonClick = true;
    delay(100);
  }
}

void subscribeTopic()
{
  if (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("", MQTT_USERNAME, MQTT_KEY))
    {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe(USERNAME PREAMBLE "Bulb", 1);
      client.subscribe(USERNAME PREAMBLE "Led", 1);
      client.subscribe(USERNAME PREAMBLE "Lock", 1);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void publishData(char* topic, char* data)
{
  //Serial.println("loop");
  if (client.connected())
  {
    switch (topic[1])
    {
      case 'u': //Bulb's Second Character
        {
          Serial.println("Publish Bulb State");
          client.publish(USERNAME PREAMBLE "Bulb", data);
          Serial.println(data);
          delay(500);
          break;
        }

      case 'o': //Lock's Second Character
        {
          Serial.println("Publish Lock State");
          //          String state = "";
          //          if (data[1] == 'F')
          //          {
          //            state = "OFF";
          //            state.toCharArray(postData, 4);
          //          }
          //          else
          //          {
          //            state = "ON";
          //            state.toCharArray(postData, 3);
          //          }
          client.publish(USERNAME PREAMBLE "Lock", data);
          Serial.println(data);
          delay(500);
          break;
        }

      case 'e': //LED's Second Character
        {
          Serial.println("Publish Led State");
          client.publish(USERNAME PREAMBLE "Led", data);
          Serial.println(data);
          delay(500);
          break;
        }
    }
  }
  client.loop();
}


//----------------------------------------------------------------------


void callback(char* topic, byte * data, unsigned int length)
{
  //  // handle message arrived {
  //  Serial.print(topic);
  //  Serial.print(": ");
  //  for (int i = 0; i < length; i++)
  //  {
  //    Serial.print((char)data[i]);
  //  }
  //  switch (topic[1])
  //  {
  //    case 'u': //Bulb's Second Character
  //      {
  //        if (data[1] == 'F')
  //        {
  //          lightPin == false;
  //        }
  //        else
  //        {
  //          lightPin == true;
  //        }
  //        break;
  //      }
  //
  //    case 'o': //Lock's Second Character
  //      {
  //        if (data[1] == 'F')
  //        {
  //          lockPin == false;
  //        }
  //        else
  //        {
  //          lockPin == true;
  //        }
  //        break;
  //      }
  //
  //    case 'e': //LED's Second Character
  //      {
  //        ledColor = "";
  //        for (int i = 0; i < length; i++)
  //        {
  //          ledColor += (char)data[i]);
  //        }
  //        break;
  //      }
  //  }
}

