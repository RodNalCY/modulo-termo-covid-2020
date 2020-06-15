#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
 
const char* wifiName = "$_WiFi-Plus_$";
const char* wifiPass = "MiRedWifi9876543210AZ";
  
ESP8266WebServer server(80);  //Define server object
 
//Handles http request 
void handleRoot() {
  String webPage;
  
  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<500> jsonBuffer;
 
  // Create the root object
  JsonObject& root = jsonBuffer.createObject();

  root["status"] = "success"; //Put Sensor value
  root["temperatura"] = "36.00"; //Reads Flash Button Status
 
  root.printTo(webPage);  //Store JSON in String variable
  server.send(200, "text/plain", webPage);
}
   
// the setup function runs once when you press reset or power the board
void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println();
  
  Serial.print("Connecting");
  
  WiFi.begin(wifiName, wifiPass);   //Connect to WiFi
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //You can get IP address assigned to ESP
  Serial.print("LocalServe:");  
  Serial.print(WiFi.localIP());  
  Serial.println("/temperature");  
 
  server.on("/temperature", handleRoot);      //Associate handler function to web requests    
  server.begin(); //Start web server
  Serial.println("HTTP server started");
}
 
void loop() {
  //Handle Clinet requests
  server.handleClient();
}
