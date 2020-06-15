#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>

#include "config.h"  // Sustituir con datos de vuestra red
#include "API.hpp"
#include "Server.hpp"
#include "ESP8266_Utils.hpp"
String global_str_temperatura="";
void setup() 
{
	Serial.begin(115200);
	ConnectWiFi_STA();
	InitServer();
}

void loop() 
{
  float f_temp = 0;
  for(int i=0; i<100; i++){
    f_temp += 1.8; 
  }
  Serial.print("temperatura: ");
  Serial.println(f_temp);
  delay(1000);
}
