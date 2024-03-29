#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

bool ledStatus = false;  // Variable de ejemplo

#include "config.h"  // Sustituir con datos de vuestra red
#include "index.h"
#include "Server.hpp"
#include "ESP8266_Utils.hpp"

void setup(void) 
{
	Serial.begin(115200);

	ConnectWiFi_STA();

	InitServer();
}

void loop()
{
	server.handleClient();
}
