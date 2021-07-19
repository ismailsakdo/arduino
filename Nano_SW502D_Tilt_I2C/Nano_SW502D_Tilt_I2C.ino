/*
 * Created with Arduinoblocks.com
 * Link project: http://www.arduinoblocks.com/web/project/558448
 * 
 * Video Instructions: 
 * 
 */

#include <Wire.h>
#include "ABlocks_LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27,16,2);

void setup()
{
  lcd.begin();lcd.noCursor();lcd.backlight();
  pinMode(2, INPUT);
  Serial.begin(9600);
}


void loop()
{

    if (digitalRead(2)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String("Status:"));
      lcd.setCursor(0, 1);
      lcd.print(String("SAFE"));
      Serial.println("SAFE");

    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(String("Status:"));
      lcd.setCursor(0, 1);
      lcd.print(String("DANGER!!!"));
      Serial.println("NOT SAFE!!!");

    }
    delay(500);

}
