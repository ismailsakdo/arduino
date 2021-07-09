//Diagram and source code: https://drive.google.com/drive/folders/1kUBgtWRL4FcZ2qgNtLdpHVs6cRJ-oleh?usp=sharing

#include <Adafruit_BMP085.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_BMP085 bmp;
  
void setup() {
  Wire.begin();
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}
  
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    lcd.setCursor(1 - 1, 1 - 1); //set coursor Column (1-16) and row (1-2)
    lcd.print("Temp(*C):");
    lcd.setCursor(10 - 1, 1 - 1);
    lcd.print(bmp.readTemperature());
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    lcd.setCursor(1 - 1, 2 - 1);
    lcd.print("P:");
    lcd.setCursor(3 - 1, 2 - 1);
    lcd.print(bmp.readPressure());
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    lcd.setCursor(10 - 1, 2 - 1);
    lcd.print("A:");
    lcd.setCursor(12 - 1, 2 - 1);
    lcd.print(bmp.readAltitude());   


    
    Serial.println();
    delay(500);
}
