#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* ssid     = "INTACHABLE.Phone";
const char* password = "12345678";
/////////////////////////////////////////////////////////////////////////////////////////////////////////
const char* PARAM_FILTER = "filter";
String global_json_payload = "";
String json_status_success = "success";
String json_status_error   = "error";
String json_status_warning = "warning";
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Adafruit_MLX90614.h> 
// Instanciar objeto
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();
float global_medida_coorp_objeto= 0;
String global_medida_coorp_objeto_str= "";
//////////////////////////////////////////////OLED_DISPLAY//////////////////////////////////////////////////
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
AsyncWebServer server(80);

void homeRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "200 - Server TermoCovID 20 Successful !!");
}
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 - Server TermoCovID 20 Not Found");
}
/////////////////////////////////////////////////////////////////////////////////////////
void dontConexionWifiAlert(){
  display.clearDisplay();
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,10);                
  display.println("! WIFI NO CONECTADO !"); 
  display.setCursor(60,20);       
   display.println("XO");  
  display.display();
  delay(500);
}
/////////////////////////////////////////////////////////////////////////////////////////
void setup() {
   Serial.begin(115200);
   /////////////////////////////////////////////////////////////////////////////////////////////
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
   display.clearDisplay();
   display.display();
   ///////////////////////////////////////////////////////
   termometroIR.begin();
   ///////////////////////////////////////////////////////
   Serial.println("");
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   /////////////////////////////////////////////////////////////////////////////////////////////       
     if(WiFi.status() != WL_CONNECTED){
        Serial.println("IP NO ENCONTRADA");
        dontConexionWifiAlert();
   }
   /////////////////////////////////////////////////////////////////////////////////////////////    
   while (WiFi.status() != WL_CONNECTED){ 
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
  String message = "200 - Wellcome to the API TermoCovID 20";
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getFiltered(AsyncWebServerRequest *request)
{
  String message = "Key get filtered by " + request->getParam(PARAM_FILTER)->value();
  Serial.println(message);
  request->send(200, "text/plain", message);
}
//Metodo HTTP GET -> Metodo de Respuesta en JSON
void getByTemperatureHandler(AsyncWebServerRequest *request)
{
  int key = GetIdFromURL(request, "/temperature/");
     
  if(key==1 && global_medida_coorp_objeto_str >= "35.00" && global_medida_coorp_objeto_str<"38.00"){
      handleRootSuccess();
  }else if(key==1 && global_medida_coorp_objeto_str >="20.00" && global_medida_coorp_objeto_str <= "35.00"){
      handleRootWarning(); 
  }else if (key==1 && global_medida_coorp_objeto_str >="1000") {
      handleRootError(); 
  }else if(key!=1){
    handleRootNotFund();    
  }
    
  String message = global_json_payload;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

//Metodo que obtiene el HTTP GET -> Metodo de Entrada
void getRequestTemperature(AsyncWebServerRequest *request) {
  
  if (request->hasParam(PARAM_FILTER)) {
    getFiltered(request);
  }
  else if(request->url().indexOf("/temperature/") != -1)
  {
    getByTemperatureHandler(request);
  }
  else {
    getAll(request);
  }
}

//Handles http request Success
void handleRootSuccess() {
  global_json_payload="";       
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["status"]      = json_status_success; //Put Sensor value
  root["temperatura"] = global_medida_coorp_objeto_str; //Reads Flash Button Status 
  root.printTo(global_json_payload);  //Store JSON in String variable
}
//Handles http request Warning
void handleRootWarning() { 
  global_json_payload="";  
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["status"]  = json_status_warning; //Put Sensor value
  root["message"] = "Superficie del objeto no detectado"; //Reads Flash Button Status 
  root.printTo(global_json_payload);  //Store JSON in String variable
}
//Handles http request Error
void handleRootError() {
  global_json_payload="";  
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["status"]  = json_status_error; //Put Sensor value
  root["message"] = "Sensor Averiado o no conectado"; //Reads Flash Button Status 
  root.printTo(global_json_payload);  //Store JSON in String variable
}
//Handles http request Error
void handleRootNotFund() {
  global_json_payload="";  
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["status"]  = json_status_error; //Put Sensor value
  root["message"] = "Error 404 - Server Not Found"; //Reads Flash Button Status 
  root.printTo(global_json_payload);  //Store JSON in String variable
}

static float readTemperaturaObjeto(){
  for(int i=0; i<20; i++){
    global_medida_coorp_objeto = termometroIR.readObjectTempC() + global_medida_coorp_objeto;
    delay(10);
  }
  global_medida_coorp_objeto /=20.0;  
  return global_medida_coorp_objeto;
}
void writeDisplayTemperatura(){
  display.clearDisplay();

  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,1); 
  display.print("IP: ");    
  display.println(WiFi.localIP());   

  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,18);                
  display.println("T.C : "); 
  
  display.setTextSize(2);
  display.setCursor(34,15);
  display.println(global_medida_coorp_objeto_str);  
  display.setCursor(95,15);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.println("C");
  
  display.display();
  delay(100);
  
}

void loop() { 

  if (WiFi.localIP()) {
    global_medida_coorp_objeto_str = String( readTemperaturaObjeto());  
    Serial.print("T.C: ");
    Serial.println(global_medida_coorp_objeto_str);  
    writeDisplayTemperatura();
    delay(100);
  } 
}
