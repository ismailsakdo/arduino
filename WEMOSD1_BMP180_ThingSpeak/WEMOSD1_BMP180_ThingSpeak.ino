#include <ESP8266WiFi.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h> // For BMP 180.
Adafruit_BMP085 bmp; // For BMP 180.
WiFiClient client;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "INSERT YOUR API KEY";
const char* ssid = "WIFI_SSID";
const char* password = "PASSWORD_SSID";
const char* server = "api.thingspeak.com";

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  Wire.begin();
  delay(10);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Checking the BMP Sensor
  if (!bmp.begin()) // For BMP 180.
  {
    Serial.println("BMP180 sensor not found");
    while (1) {}
  }
}

  void loop(){
    float temp = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float att = bmp.readAltitude();
    if (isnan(temp) || isnan(pressure) || isnan(att))
    {
      Serial.println("Failed to read from BMP180 sensor!");
      return;
    }

    if (client.connect(server, 80)) {
      String postStr = apiKey;
      postStr += "&field1=";
      postStr += String(temp);
      postStr += "&field2=";
      postStr += String(pressure);
      postStr += "&field3=";
      postStr += String(att);
      postStr += "\r\n\r\n"; // Insert this to return to original

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);

      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.print(" degrees Celsius, Pressure (m): ");
      Serial.print(pressure);
      Serial.print(" Altitude:");
      Serial.print(att);
      Serial.println("Sending data to Thingspeak");

      //display on LCD
      lcd.setCursor(1 - 1, 1 - 1); //set coursor Column (1-16) and row (1-2)
      lcd.print("Temp(*C):");
      lcd.setCursor(10 - 1, 1 - 1);
      lcd.print(bmp.readTemperature());

      lcd.setCursor(1 - 1, 2 - 1);
      lcd.print("P:");
      lcd.setCursor(3 - 1, 2 - 1);
      lcd.print(bmp.readPressure());

      lcd.setCursor(10 - 1, 2 - 1);
      lcd.print("A:");
      lcd.setCursor(12 - 1, 2 - 1);
      lcd.print(bmp.readAltitude());
    }

    client.stop();

    Serial.println("Waiting 5 minutes");
    // thingspeak needs at least a 15 sec delay between updates
    // 20 seconds to be safe
    delay(15000);
}
