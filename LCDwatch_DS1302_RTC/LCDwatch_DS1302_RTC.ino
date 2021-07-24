#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <virtuabotixRTC.h> //ArduinoRTClibrary (Special for RTC1302 using it) - Set the Time Using RTC by Makuna Library
LiquidCrystal_I2C lcd(0x27, 16, 2);
virtuabotixRTC myRTC(5, 4, 2); //CLK, DAT, RST (RTC1302)

void setup()
{
  Wire.begin();
  Serial.begin(9600); Serial.flush(); while(Serial.available()>0)Serial.read();
  lcd.init();
  lcd.backlight();
}

void loop()
{
    myRTC.updateTime();
    lcdstatus();
    serial2();
    delay(2000);
}

//Function can be as follows:

void serial2() {
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print(" ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  Serial.print(":");
}

void lcdstatus(){
  lcd.setCursor(0,0);
  lcd.print("Time:");
  lcd.setCursor(6,0);
  lcd.print(String(myRTC.hours)+String(":")+String(myRTC.minutes)+String(":")+String(myRTC.seconds));
  lcd.setCursor(0,1);
  lcd.print("Date:");
  lcd.setCursor(6,1);
  lcd.print(String(myRTC.dayofmonth)+String("/")+String(myRTC.month)+String("/")+String(myRTC.year));
  
}
