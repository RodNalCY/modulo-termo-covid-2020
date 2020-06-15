#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
///////////////////////////////////////////////////////
const char* ssid     = "$_WiFi-Plus_$";
const char* password = "MiRedWifi9876543210AZ";
///////////////////////////////////////////////////////
const char* PARAM_FILTER = "filter";
///////////////////////////////////////////////////////
AsyncWebServer server(80);

void homeRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "Hello, world");
}
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
///////////////////////////////////////////////////////
void setup() {
   Serial.begin(115200);
   ///////////////////////////////////////////////////////
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   
   while (WiFi.status() != WL_CONNECTED) 
   { 
     delay(100);  
     Serial.print('.'); 
   }
 
   Serial.println("");
   Serial.print("Iniciado STA:\t");
   Serial.println(ssid);
   Serial.print("IP address:\t");
   Serial.println(WiFi.localIP());
   ///////////////////////////////////////////////////////
   server.on("/", HTTP_GET, homeRequest);
   server.on("/temperature", HTTP_GET, getRequestTemperature); 
   server.onNotFound(notFound);
   server.begin();
   Serial.println("HTTP server started");
   
}
int GetIdFromURL(AsyncWebServerRequest *request, String root)
{
  String string_id = request->url();
  string_id.replace(root, "");
  int id = string_id.toInt();
  return id;
}
void getAll(AsyncWebServerRequest *request)
{
  String message = "Get All Temperature";
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getFiltered(AsyncWebServerRequest *request)
{
  String message = "Get filtered by " + request->getParam(PARAM_FILTER)->value();
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getByIdTemperature(AsyncWebServerRequest *request)
{
  int id = GetIdFromURL(request, "/temperature/");
 // String message = String("value Temperature ") + id; {"status": "success", "data": {"temperature": 37} }
  String message = String("{'status': 'success', 'data': {'temperature': 37} }");
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getRequestTemperature(AsyncWebServerRequest *request) {
  
  if (request->hasParam(PARAM_FILTER)) {
    getFiltered(request);
  }
  else if(request->url().indexOf("/temperature/") != -1)
  {
    getByIdTemperature(request);
  }
  else {
    getAll(request);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
