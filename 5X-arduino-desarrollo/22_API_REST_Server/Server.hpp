AsyncWebServer server(80);

void homeRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "Hello, world");
}

void notFound(AsyncWebServerRequest *request) {
	request->send(404, "text/plain", "Not found");
}

void InitServer()
{
	server.on("/", HTTP_GET, homeRequest);
  server.on("/temperature", HTTP_GET, getRequestTemperature);
	
	server.onNotFound(notFound);

	server.begin();
    Serial.println("HTTP server started");
}
