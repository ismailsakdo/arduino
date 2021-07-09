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
#define AIO_USERNAME  "INSERT_YOUR_IO_USERNAME" // username
#define AIO_KEY  "INSERT_YOUR_IO_API_KEY" // key
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

boolean MQTT_connect();

boolean MQTT_connect() {  int8_t ret; if (mqtt.connected()) {    return true; }  uint8_t retries = 3;  while ((ret = mqtt.connect()) != 0) { mqtt.disconnect(); delay(2000);  retries--;if (retries == 0) { return false; }} return true;}

Adafruit_MQTT_Publish temptest = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temptest");
void setup()
{
  Wire.begin();
  bmp.begin();
  Serial.begin(9600);

  WiFi.disconnect();
  delay(3000);
  Serial.println("START");
  WiFi.begin("SSID","YOUR_PASSWORD"); // Insert your SSID and Password
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
 
    if (MQTT_connect()) {
      if (temptest.publish(temp)) {
        Serial.println(temp);
        Serial.println("SENT!!");

      }

    } else {
      Serial.println("FAIL!!!");

    }
    delay(15000);

}
