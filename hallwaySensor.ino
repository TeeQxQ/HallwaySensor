#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

extern "C"
{
  #include "gpio.h"
}

extern "C"
{
  //#include "user_interface.h"
}

#define ISR_PREFIX ICACHE_RAM_ATTR

//Hardware parameters:
const int PIR_PIN = 2;

//Wifi station mode (STA) parameters
const char* network_ssid = "Kalat_ja_Rapu_2G"; //"TeeQNote9"; //"Kalat_ja_Rapu_2G";
const char* network_password = "rutaQlli";

volatile bool turnLightsOn = false;

//Handles interrupt caused by the button press
/*ISR_PREFIX void handleInterrupt()
{

  if (digitalRead(PIR_PIN) == HIGH)
  {
    //Serial.println("HIGH");
    turnLightsOn = true;
  }

}*/

void connectWifi()
{
  
}

void setup() {
  pinMode(PIR_PIN, INPUT);
  
  //Serial connection for debugging purposes
  Serial.begin(115200);
  delay(100);

  //Explicitely define this is a station, not access point
  WiFi.mode(WIFI_STA);

  //Connect to wifi network:
  WiFi.begin(network_ssid, network_password);

  Serial.println("Wifi (STA) parameters:");
  Serial.print("- ssid: ");
  Serial.println(network_ssid);
  Serial.print("- password: ");
  Serial.println(network_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to wifi...");
    delay(1000);
  }

  Serial.println("Wifi connected");

  Serial.println("Starting to sleep...");
  ESP.deepSleep(0);
  Serial.println("I am awake!");

  //attachInterrupt(digitalPinToInterrupt(PIR_PIN), handleInterrupt, CHANGE);

}

void loop() {
  /*
  if (turnLightsOn)
  {
    turnLightsOn = false;
    Serial.println("Turn on!");
    
    if(WiFi.status()== WL_CONNECTED)
    {
      HTTPClient http;
      http.begin(String("http://192.168.1.112/api/pB-JbLG1vkk-6axlykmaEBOzM4y3u8lUUZVgbj8M/lights/1/state"));
      http.addHeader("Content-Type", "text/plain");
      http.addHeader("Connection", "close");

      String put_string;
      put_string = "{\"on\":";
      put_string += "true";
      put_string += "}";
      int httpResponseCode = http.PUT(put_string);
      Serial.println(httpResponseCode); 
 
      http.end();
    }
  }*/
}
