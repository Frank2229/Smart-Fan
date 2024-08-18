#include <DHT.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "time.h"

#define DHT_DATA 32
#define DHT_MODEL DHT22
#define RELAY 33
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// VARIABLES DETERMINED BY THE MASTER
float maximumTemperature = 71.0;
float cooledTemperature = 69.0;
const int gmtOffsetMult = -5;
const int daylightOffsetMult = 1;
int bedtimeStart_Hour = 21;
int bedtimeStart_Minute = 0;
int bedtimeEnd_Hour = 5;
int bedtimeEnd_Minute = 59;
int maxCounter = 2;

// INITIALIZATION OF DHT DEVICE & CORRESPONDING TEMPERATURE VARIABLES
DHT dht(DHT_DATA, DHT_MODEL);
float temperature = 0.0;
float humidity = 0.0;
bool isRoomCooling = false;

// WIFI ARIABLES
const char* ssid = "GET YOUR OWN WIFI!!";
const char* password = "GET YOUR OWN PASSWORD!!";
const char* ntpServer = "in.pool.ntp.org";

// INITIALIZATION OF TIME VARIABLES
const long secondsPerHour = 3600;
bool isBedTime = false;
int hour = 0;
int minute = 0;
char* dayPart;

// SETUP OLED DISPLAY
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int displayCounter = 0;
int currentMetric = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAY, OUTPUT);

  // Initialization of the OLED w/ introduction message.
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Smart Fan");
  display.println("By: Frank");
  display.println("    Glaser");
  display.display();

  // Connect to internet to retrieve world date and time.
  // Adjust time based on local time zone and daylight savings.
  // After date and time is found and stored, sever wifi connection.
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  configTime((secondsPerHour * gmtOffsetMult), (secondsPerHour * daylightOffsetMult), ntpServer);
  isBedtime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  // Retrieve and store current room temperature and humidity.
  temperature = dht.readTemperature(true);
  humidity = dht.readHumidity();

  // Assess whether or not the current time is within the Master's sleep interval.
  // If the current time is deemed to be not within the sleeping interval, turn the fan on if the temperature is above the maximum temperature.
  isBedTime = isBedtime();
  if (isBedTime) digitalWrite(RELAY, HIGH);
  else {
    if (isRoomCooling) {
      if (temperature <= cooledTemperature) {
        digitalWrite(RELAY, LOW);
        isRoomCooling = false;
      }
    } else {
      if (temperature >= maximumTemperature) {
        digitalWrite(RELAY, HIGH);
        isRoomCooling = true;
      } else {
        digitalWrite(RELAY, LOW);
        isRoomCooling = false;
      }
    }
  }

  switch (currentMetric) {
    case 0:
      if (hour > 12) {
        hour -= 12;
        dayPart = "pm";
      }
      else if (hour == 0) {
        dayPart = "am";
        hour = 12;
      }
      else if (hour == 12) dayPart = "pm";
      else dayPart = "am";
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("Time");
      display.setTextSize(3);
      display.setCursor(0, 20);
      display.print(hour);
      display.print(":");
      if (minute < 10) display.print("0");
      display.print(minute);
      display.print(dayPart);
      display.display();
      if (displayCounter < maxCounter) displayCounter++;
      else {
        displayCounter = 0;
        currentMetric++;
      }
      break;
    case 1:
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("Temp");
      display.setTextSize(3);
      display.setCursor(0, 20);
      display.print(temperature);
      display.print(char(247));
      display.display();
      if (displayCounter < maxCounter) displayCounter++;
      else {
        displayCounter = 0;
        currentMetric++;
      }
      break;
    case 2:
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.println("Humidity");
      display.setTextSize(3);
      display.setCursor(0, 20);
      display.print(humidity);
      display.print("%");
      display.display();
      if (displayCounter < maxCounter) displayCounter++;
      else {
        displayCounter = 0;
        currentMetric = 0;
      }
      break;
  }

  delay(2000);
}

// Assess whether current time is within the master's sleeping interval. Master needs his sleep.
// If current time is outside of the specified interval or cannot retrieve local time, return false.
bool isBedtime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return false;
  hour = timeinfo.tm_hour;
  minute = timeinfo.tm_min;
  Serial.print(hour);
  Serial.print(" ");
  Serial.println(minute);
  if ((hour >= bedtimeStart_Hour && bedtimeStart_Minute >= 0) || (hour <= bedtimeEnd_Hour && bedtimeEnd_Minute <= 59)) return true;
  else return false;
}