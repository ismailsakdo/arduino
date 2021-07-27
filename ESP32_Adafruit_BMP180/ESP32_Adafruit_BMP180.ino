/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP32     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>
Adafruit_BMP085 bmp;

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME  "YOUR_USERNAME_IN_IOADAFRUIT" //replace with your own channel
#define AIO_KEY  "YOUR_API_KEY_IOADAFRUIT" //replace with your own API Key
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}

Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Pressure = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Pressure");
Adafruit_MQTT_Publish Altitude = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Altitude");
void setup()
{
  pinMode(2,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  bmp.begin();
  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("YOUR_SSID_NAME","YOUR_PASSWORD"); //insert your SSID and PASSWORD
  while ((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("..");

  }
  Serial.println("Connected");
  Serial.println("Your IP is");
  Serial.println((WiFi.localIP()));

}


void loop()
{

    float temp = bmp.readTemperature();
    float pres = bmp.readPressure();
    float alt = bmp.readAltitude();
    
    if (MQTT_connect()) {
      if (Temperature.publish(temp)) {
        Serial.println("Sent!!!");
        Serial.println(temp);

      } else {
        Serial.println("Error");
        
      }
      if (Pressure.publish(pres)) {
        Serial.println("Sent!!!");
        Serial.println(pres);

      } else {
        Serial.println("Error");

      }
      if (Altitude.publish(alt)) {
        Serial.println("Sent!!!");
        Serial.println(alt);

      } else {
        Serial.println("Error");

      }

    }
    delay(300000);

}
