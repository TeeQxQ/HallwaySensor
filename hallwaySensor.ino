#include "WiFi.h"
#include <HTTPClient.h>

#define DEBUG 0
#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 in hex
#define ON 1
#define OFF 0
#define ISR_PREFIX ICACHE_RAM_ATTR

//Hardware parameters:
const int PIR_PIN = 33;

//Wifi station mode (STA) parameters
const char* network_ssid = "Kalat_ja_Rapu_2G"; //"TeeQNote9"; //"Kalat_ja_Rapu_2G";
const char* network_password = "rutaQlli";

volatile unsigned long lightTimerStart_ms = 0;
const unsigned long lightsOnThreshold_ms = 60 * 1000;

//Handles interrupt caused by the button press
ISR_PREFIX void handleInterrupt()
{
  println("Interrupt");
  lightTimerStart_ms = millis();
}

void print(const String &msg)
{
  if(DEBUG)
  {
    Serial.print(msg);
  }
}

void println(const String &msg)
{
  print(msg + '\n');
}

void connectWifi()
{
  //Explicitely define this is a station, not access point
  WiFi.mode(WIFI_STA);
  
  //Connect to wifi network:
  WiFi.begin(network_ssid, network_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(10);
  }

  println("Wifi connected");
}

int lights(bool turnOn)
{
  if(WiFi.status()== WL_CONNECTED)
  {
    HTTPClient http;
    http.begin(String("http://192.168.1.112/api/pB-JbLG1vkk-6axlykmaEBOzM4y3u8lUUZVgbj8M/lights/1/state"));
    http.addHeader("Content-Type", "text/plain");
    http.addHeader("Connection", "close");

    String put_string;
    put_string = "{\"on\":";

    if (turnOn)
    {
      put_string += "true";
    }
    else
    {
      put_string += "false";
    }
    put_string += "}";
    int httpResponseCode = http.PUT(put_string);
    http.end();
    
    return httpResponseCode;
  }
  return 0;
}

void setup() {

  //Serial connection for debugging purposes
  if(DEBUG)
  {
    Serial.begin(115200);
    delay(10);
  }

  pinMode(PIR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), handleInterrupt, RISING);

  //initialize timer
  lightTimerStart_ms = millis();

  //Enable waking up from deep sleep by external interrupt
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

  //Initialize wifi
  connectWifi();

  //Turn lights on
  lights(ON);

  //delay(2000);

  //lights(OFF);

}

void loop() 
{
  unsigned long currentTime_ms = millis();

  if (currentTime_ms - lightTimerStart_ms > lightsOnThreshold_ms)
  {
    //No movement withing timeout interval
    println("Turning lights off");
    lights(OFF);
    println("Starting to sleep");
    esp_deep_sleep_start();
  }
  
  //Intentionally left empty
  Serial.println(digitalRead(PIR_PIN));
  delay(1000);
}
