#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <SoftwareSerial.h>

#define REPORTING_PERIOD_MS 1000
SoftwareSerial bt_serial(2, 3); // TX, RX
LiquidCrystal_I2C lcd(0x27, 16, 2);
PulseOximeter pox;
uint32_t tsLastReport = 0;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Wire.begin();
  Serial.begin(9600); Serial.flush(); while (Serial.available() > 0)Serial.read();
  lcd.init();
  lcd.backlight();
  Serial.print("Initializing pulse oximeter..");
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  bt_serial.begin(9600);
  
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); // Initialized Pulseoximeter
  pox.setOnBeatDetectedCallback(onBeatDetected);   // Register a callback for the beat detection
}

//Function can be as follows:

void loop ()
{
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(String("HR:"));
    lcd.setCursor(4,0);
    lcd.print(pox.getHeartRate());
    lcd.setCursor(0,1);
    lcd.print(String("SpO2(%):"));
    lcd.setCursor(9,1);
    lcd.print(pox.getSpO2());
    bt_serial.println(String("HR:")+(pox.getHeartRate())+String("//")
    +String("SPO2:")+(pox.getSpO2()));
    tsLastReport = millis();
  }

}
