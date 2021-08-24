#include <SoftwareSerial.h> 
#include <AHT10.h>
#include <Wire.h>

int ledPin = 13; // LED 13 will be used to let the user know when the code has looped back again.

// ThingSpeak Write API key, this can be found in your channel under API Keys and you'll want the "Write API Key". 
String apiKey = "INSERT_API_THINGSPEAK"; // Insert Your Thingspeak API Key

#define SSID "Enter your SSID" // Remove the text but keep the ""
#define PASS "Enter Your SSID Password"// Same as above, keep the ""
SoftwareSerial SoftSer(11, 10);
AHT10 myAHT10(0x38);
float temp;
float humidity;

void setup() 
{
  Wire.begin();
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  SoftSer.begin(115200); 
}

void loop() 
{
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);

  temp = myAHT10.readTemperature(); // If you're not using a BMP 180, remove bmp.readTemperature(); and enter the 
  humidity = myAHT10.readHumidity();// analog pin you're reading from e.g: "float temp = analogRead(pin3);"
  
  Serial.println(temp);
  Serial.println(humidity);

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
  getStr += String(temp);
  getStr += "&field2=";
  getStr += String(humidity);
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
  // The AT+RST command resets the ESP8266, I'm doing this because data is uploaded much more reliably. 
  SoftSer.println("AT+RST");
  // ThingSpeak needs on average, a 15 second delay between updates. Change this to whatever suits your application
  // e.g: delay(60000) is ten minutes.
  delay(60000);
}
