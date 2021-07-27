// James. - LINK TO YOUTUBE: https://www.youtube.com/watch?v=Z-H6AxiIeL0&t=328s

#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BMP085.h> // For BMP 180.
Adafruit_BMP085 bmp; // For BMP 180.

int ledPin = 13; 
String apiKey = "YOUR_API_KEY"; // thingspeak

#define SSID "YOUR_SSID" // Remove the text but keep the ""
#define PASS "YOUR_PASSWORD"// Same as above, keep the ""

SoftwareSerial SoftSer(11, 10); // RX - pin 10; TX - pin 11

void setup() 
{
  Wire.begin();
  pinMode(ledPin, OUTPUT);

  if (!bmp.begin()) // For BMP 180.
  {
    Serial.println("BMP180 sensor not found");
    while (1) {}
  }
  
  // Begins serial communication at a baud of 9600.
  Serial.begin(9600);
  
  // Begins software serial communication at a baud of 9600.
  SoftSer.begin(115200); 

}

void loop() 
{
  
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);


  float temp = bmp.readTemperature(); // your sensor
  
  char buf[16]; // buf array can hold up to 16 characters. 
  String strTemp = dtostrf(temp, 4, 1, buf); 
  
  Serial.println(strTemp); // Prints the recorded temperature to the serial print.

  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  SoftSer.println(cmd);

  if (SoftSer.find("Error")) 
  {
    Serial.println("AT+CIPSTART error");
    return;
  }

  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr += "&field1=";
  getStr += String(strTemp);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  SoftSer.println(cmd);

  if (SoftSer.find(">")) 
  {
    SoftSer.print(getStr); // Send data.
  }
  else 
  {
    SoftSer.println("AT+CIPCLOSE");
    
    Serial.println("AT+CIPCLOSE"); // If this shows on the serial monitor the data did not send.
  }
  // The AT+RST command resets the ESP8266
  SoftSer.println("AT+RST");
  
  delay(15000);
}
