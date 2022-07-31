#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,4,5,6,7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0)
  lcd.display();
  // print the number of seconds since reset:
 
}
