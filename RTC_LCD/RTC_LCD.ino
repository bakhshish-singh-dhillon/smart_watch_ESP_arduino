/*Read the time from RTC and display on OLED
 with an ESP8266<br> sda=0, scl=2
* *************************************/

// Libraries
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <Wire.h>
#include "RTClib.h" //  Lady Ada
//Object declarations
RTC_DS1307 rtc;            // RTC
SSD1306  display(0x3c, D3, D5);//0x3C being the usual address of the OLED

//months and da Arrays. Put in Language of your choice, omitt the 'da' part of the weekdays
char *months[] =
{
  "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};
char *days[] = {"Sun", "Mon", "Tues", "Wednes", "Thurs", "Fri", "Satur" };


// date and time variables
byte m = 0;          // contains the minutes, refreshed each loop
byte h = 0;          // contains the hours, refreshed each loop
byte s = 0;         // contains the seconds, refreshed each loop
byte mo = 0;       // contains the months, refreshes each loop
int j = 0;           // contains the year, refreshed each loop
byte d = 0;         // contains the da (1-31)
byte da = 0;     // contains da of week (0-6)

void setup() {
  Wire.pins(D3, D5);// yes, see text
  Wire.begin(D3,D5);// 0=sda, 2=scl
  rtc.begin();

// reading of time here only necessary if you want to use it in setup
  DateTime now = rtc.now();
  da = now.dayOfTheWeek();
  j = now.year();
  mo = now.month();
  d = now.day();
  h = now.hour();
  m = now.minute();
  s = now.second();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime())
  {
    Serial.print(F("Current Unix time"));
    Serial.println(now.unixtime());
    Serial.print(F("Compiled Unix time"));
    Serial.println(compiled.unixtime());
    Serial.println("RTC is older than compile time! Updating");
    // following line sets the RTC to the date & time this sketch was compiled<br>   // uncomment to set the time
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialise the display.
  display.init();
  display.flipScreenVertically();// flipping came in handy for me with regard 
                                                                // to screen position
  display.setFont(ArialMT_Plain_10);

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // This will reflect the time that your sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }

}


void loop() {
  display.clear();
  DateTime now = rtc.now();
  da = now.dayOfTheWeek();
  j = now.year();
  mo = now.month();
  d = now.day();
  h = now.hour();
  m = now.minute();
  s = now.second();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  String t = String(h) + ":" + String(m) + ":" + String(s);
  String t2 = String(d) + ":"  + String(mo) + ":" + String(j);
  display.drawString(0, 5, t);//
  display.drawString(0, 15, t2);
  display.drawString(0, 25, months[mo - 1]);
  String d = days[da];
  d = d + "day";//adding the word 'da' (=da)  to the names of the days
  display.drawString(0, 35, d);
  // write the buffer to the display
  display.display();
  delay(10);
}
