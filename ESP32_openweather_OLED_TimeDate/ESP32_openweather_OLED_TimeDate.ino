//Source: https://www.youtube.com/watch?v=qwM8oiy0Vy4

#include <WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);

const char* ssid     = "YOUR_SSID";                 // SSID of local network
const char* password = "YOUR_PASSWORD";                    // Password on network
String APIKEY = "YOUR_API";
String CityID = "YOUR_CITY_ID";          //Your City ID
bool id = false;
WiFiClient client;
char servername[] = "api.openweathermap.org";            // remote server we will connect to
String result;

WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
NTPClient timeClient(ntpUDP, "my.pool.ntp.org", 28800, 60000); // Malaysia setup NEED the value of 28800, for GMT+8

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);   //   create wifi station
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3D for 128x64
  delay(200);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("Connecting.");
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected ");
  display.println("IP Address: ");
  display.println(WiFi.localIP());
  display.display();
  delay(1000);
  display.clearDisplay();
  timeClient.begin();

/*
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  //timeClient.setTimeOffset(3600); - As result Malaysia is GMT + 8, therefore, the value will be 28800                    
  //getData();
  //delay(500);
*/

}

void loop()                    
{
  timeClient.update();
  if (client.connect(servername, 80))
  { //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");        //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1);                                          //waits for data
  while (client.connected() || client.available())
  { //connected or data available
    char c = client.read();                     //gets byte from ethernet buffer
    result = result + c;
  }

  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  //Serial.println(result);
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonDocument<1024> doc;
  DeserializationError  error = deserializeJson(doc, jsonArray);


  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }

  String location = doc["name"];
  String country = doc["sys"]["country"];
  int temperature = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  float pressure = doc["main"]["pressure"];
  int id = doc["id"];
  float Speed = doc["wind"]["speed"];
  int degree = doc["wind"]["deg"];
  float longitude = doc["coord"]["lon"];
  float latitude = doc["coord"]["lat"];

  Serial.println();
  Serial.print("Country: ");
  Serial.println(country);
  Serial.print("Location: ");
  Serial.println(location);
  Serial.print("Location ID: ");
  Serial.println(id);
  Serial.printf("Temperature: %d°C\r\n", temperature);
  Serial.printf("Humidity: %d %%\r\n", humidity);
  Serial.printf("Pressure: %.2f hpa\r\n", pressure);
  Serial.printf("Wind speed: %.1f m/s\r\n", Speed);
  Serial.printf("Wind degree: %d°\r\n", degree);
  Serial.printf("Longitude: %.2f\r\n", longitude);
  Serial.printf("Latitude: %.2f\r\n", latitude);
  //Serial.println(timeClient.getFormattedTime()); - First Option
  
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);

  display.clearDisplay();
  display.setCursor(0, 0);            // Start at top-left corner
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.print(" Location: ");
  display.print(country);
  display.print(" ");
  display.println(location);
 
  display.println();
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.print("T: ");
  display.print(temperature);
  display.print((char)247);
  display.print("C     ");
  display.print("H: ");
  display.print(humidity);
  display.println("%  ");
  display.print("Pressure:");
  display.print(pressure);
  display.println("hpa");
  display.print("WS: ");
  display.print(Speed);
  display.print("m/s  ");
  display.print("WA: ");
  display.print(degree);
  display.println((char)247);
  display.print("Lat: ");
  display.print(latitude);
  display.print(" ");
  display.print("Lon: ");
  display.println(longitude);
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  //display.print(timeClient.getFormattedTime()); - Option Only Time data/ output
  //display.println(timeClient.getFormattedDate()); - Mixing overall
  display.print("Date: ");
  display.println(dayStamp);
  display.print("Time: ");
  display.println(timeStamp);
  
  
  display.display();
  
  delay(60000);     // 10 minuts delay

}
