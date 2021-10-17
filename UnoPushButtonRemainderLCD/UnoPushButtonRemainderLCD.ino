//This arduino code is generated by PictoBlox

//Included Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int buttonState = 0; //change float as int with the initial stage of 0
int lastButtonState = 0;
int buttonPushCounter = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(2, INPUT);
  pinMode(13, OUTPUT);

}

void loop() {

  buttonState = digitalRead(2);
  if ((buttonState != lastButtonState)) { //change the == into !=
    if ((buttonState == HIGH)) { //change "HIGH" as HIGH
      buttonPushCounter += 1;
      lcd.clear();
      Serial.println("on");
      Serial.println("Number of Button Push:");
      Serial.println(buttonPushCounter);
      lcd.setCursor(1 - 1, 1 - 1);
      lcd.print("No. But. Push:");
      lcd.setCursor(1 - 1, 2 - 1);
      lcd.print(buttonPushCounter);
    }
    else {
      Serial.println("off");
    }
    delay(0.05 * 1000);
  }
  lastButtonState = buttonState;
  if (((buttonPushCounter % 4) == 1)) { //change / as %
    digitalWrite(13, true);
  }
  else {
    digitalWrite(13, false);
  }
}
