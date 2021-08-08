/***************************************************
  This is an example for the AHT10 Humidity & Temp Sensor

  Designed specifically to work with the AHT10, modified using library from:
  Source: https://github.com/enjoyneering/

  These displays use I2C to communicate, 2 pins are required to
  interface
 ****************************************************/

#include <Wire.h>
#include <AHT10.h>
#include <LiquidCrystal.h>

// Connect Vin to 3-5VDC
// Connect GND to ground
// Connect SCL to I2C clock pin (A5 on UNO)
// Connect SDA to I2C data pin (A4 on UNO)

AHT10 myAHT10(0x38);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  Wire.begin();
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Temperature & ");
  lcd.setCursor(0,1);
  lcd.print("Humidity Meter");
  delay(2000);
  lcd.clear();
  Serial.begin(9600);
  Serial.println("AHT10 test");

  if (!myAHT10.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
}

void loop() {
    float temp = myAHT10.readTemperature();
    float rel_hum = myAHT10.readHumidity();
    Serial.print("Temp: "); Serial.print(temp); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(rel_hum); Serial.println(" \%");
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.setCursor(5,0);
    lcd.print(temp);
    lcd.setCursor(10,0);
    lcd.print((char)223);
    lcd.setCursor(11,0);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print("RH:");
    lcd.setCursor(3,1);
    lcd.print(rel_hum);
    lcd.setCursor(8,1);
    lcd.print("%");
    delay(500);
}
