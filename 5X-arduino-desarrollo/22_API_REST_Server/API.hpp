#include "ESP8266_Utils_APIREST.hpp"
const char* PARAM_FILTER = "filter";

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
  String message = String("value Temperature ") + id;
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
