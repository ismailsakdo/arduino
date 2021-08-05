#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <virtuabotixRTC.h> //ArduinoRTClibrary (Special for RTC1302 using it) - Set the Time Using RTC by Makuna Library
#include <MAX30100_PulseOximeter.h>
#include <SPI.h>
#include <SD.h>

#define REPORTING_PERIOD_MS 1500

LiquidCrystal_I2C lcd(0x27, 16, 2);
const int chipSelect = 10;
File sd_file; //SPI Interface: (Sensor Shield ---> SD Card Module)
//VCC, GND, MOSI, D10, SCK, MISO ===> Match accordingly from left to right
virtuabotixRTC myRTC(5, 4, 2); //CLK, DAT, RST (RTC1302)
PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  lcd.init(); lcd.backlight();
  Serial.print("Initializing pulse oximeter..");
  lcd.print("Initializing...");
  delay(2000); lcd.clear();

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); // Initialized Pulseoximeter
  pox.setOnBeatDetectedCallback(onBeatDetected);   // Register a callback for the beat detection

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

//Function can be as follows:

void poxStatus()
{
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    tsLastReport = millis();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T=");
    lcd.setCursor(2, 0);
    lcd.print(String(myRTC.hours) + String(":") + String(myRTC.minutes) + String(" "));
    lcd.setCursor(7, 0);
    lcd.print(String(" D="));
    lcd.setCursor(10, 0);
    lcd.print(String(myRTC.dayofmonth) + String("/") + String(myRTC.month));
    lcd.setCursor(0, 1);
    lcd.print(String("HR:"));
    lcd.setCursor(3, 1);
    lcd.print(pox.getHeartRate());
    lcd.setCursor(8, 1);
    lcd.print(String("| "));
    lcd.setCursor(9, 1);
    lcd.print(String("S:"));
    lcd.setCursor(12, 1);
    lcd.print(pox.getSpO2());
  }

}

void loop()
{
  myRTC.updateTime();
  poxStatus();
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(String(myRTC.hours)+String(":")+String(myRTC.minutes)+String(":"));
    dataFile.print(String(myRTC.dayofmonth)+String("/")+String(myRTC.month)+String("/")+String(myRTC.year));
    dataFile.println(String("||")+String(pox.getHeartRate())+String(";")+String(pox.getSpO2()));
    dataFile.close();
    }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
