#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * const char* ssid     = "$_WiFi-Plus_$";
 * const char* password = "MiRedWifi9876543210AZ";
 */
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
const String AP_NAME_SERVER = "WF-COVID-2020";
const String AP_PASW_SERVER = "123456789";
String ssid     = "";
String password = "";
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "password";
IPAddress local_IP(172, 168, 0, 7);
IPAddress gateway(172, 168, 0, 254);
IPAddress subnet(255, 255, 255, 0);
AsyncWebServer server(80);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define pinALERTA   D5 
#define pinLedGRN   D6
#define pinLEDRED   D7
#define pinLedDISTANCIA   D8
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int global_active_json_system_alert = 0; 
int global_active_json_server = 0; 
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
// HTML web page to handle 3 input fields (ssid, password, passwordconf)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>Termo Covid 19</title>
<style>
body{
  background-color: #E6C00B;
  margin:0;
}
.form{
  width:660px;
  height:440px;
  background:#e6e6e6;
  border-radius:8px;
  box-shadow:0 0 40px -10px #000;
  margin:calc(50vh - 220px) auto;padding:20px 30px;
  max-width:calc(100vw - 40px);
  box-sizing:border-box;
  font-family:'Montserrat',sans-serif;
  position:relative;
}
h2{
  margin:10px 0;
  padding-bottom:10px;
  width:260px;
  color:#78788c;
  border-bottom:3px solid #78788c
}
h4{
  color:#78788c;
}
.mensaje{  
  font-family:'Montserrat';
  color:#78788c;
}
input{
  width:100%;
  padding:10px;
  box-sizing:border-box;
  background:none;
  outline:none;
  resize:none;
  border:0;
  font-family:'Montserrat',sans-serif;transition:all .3s;
  border-bottom:2px solid #bebed2;
}
input:focus{
  border-bottom:3px solid #78788c;
}
p:before{
  content:attr(type);
  display:block;
  margin:28px 0 0;
  font-size:14px;
  color:#5a5a5a;
}
.enviar{
  float:right;
  padding:8px 12px;
  margin:8px 0 0;
  font-family:'Montserrat',sans-serif;
  border:2px solid #78788c;
  background:0;
  color:#5a5a6e;
  cursor:pointer;
  transition:all .3s;
  border-radius: 5px;
}
.enviar:hover{
  background:#78788c;
  color:#fff
}
.contacto{
  content:'Hi';
  position:absolute;
  bottom:-15px;
  right:-20px;
  background:#50505a;
  color:#fff;
  width:330px;
  padding:16px 4px 16px 0;
  border-radius:6px;
  font-size:13px;
  box-shadow:10px 10px 40px -14px #000;
}
span{
  margin:0 5px 0 15px;
}
.card {
  box-shadow: 0 5px 8px 0 rgba(0,0,0,0.2);
  transition: 0.3s;
  width: 70%;
  padding: 35px;
}
.card:hover {
  box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);
}
</style>
<script type="text/javascript">
 function validarPasswd () {   
  var p1 = document.getElementById("password").value;
  var p2 = document.getElementById("passwordconf").value;
  var espacios = false;
  var cont = 0;
  // Este bucle recorre la cadena para comprobar
  // que no todo son espacios
  while (!espacios && (cont < p1.length)) {
    if (p1.charAt(cont) == " ")
      espacios = true;
    cont++;
  }   
  if (espacios) {
   alert ("La contraseña no puede contener espacios en blanco");
   return false;
  }
   
  if (p1.length == 0 || p2.length == 0) {
   alert("Los campos de la password no pueden quedar vacios");
   return false;
  }
   
  if (p1 != p2) {
   alert("Las passwords deben de coincidir");
   return false;
  } else {
   alert("Todo esta correcto");
   return true; 
  }
 }
</script>
</head>
<body>
<form class="form" onSubmit="return validarPasswd()" action="/get">  
  <table>
    <tr>
       <td>
        <div class="card">       
            <h4>⌨ Panel de Configuración</h4> 
            <p class="mensaje">
              ⚡ 
              Bienvenid@, en el siguiente formulario escriba correctamente sus credenciales de WiFi.
              <br><br>
              Gracias por colaborar con PM 2.5, usted ahora podra monitorear la calidad de aire en su zona.
            </p> 
          </div>        
       </td>
       
       <td>
        <h2>Termo ☁ Covid </h2>
        <p type="SSID:">        
          <input type="text" 
          name="ssid" 
          id="ssid" 
          placeholder="✎ Ingrese su SSID"></input>
        </p>
        <p type="Password:">
          <input type="password" 
          name="password" 
          id="password" 
          placeholder="✎ Ingrese su password"></input>
        </p>
        <p type="Password:">
          <input type="password" 
          name="passwordconf" 
          id="passwordconf" 
          placeholder=" ✎Vuelva a escribir su password ">
        </input>
        </p>        
        <input class="enviar" type="submit" value="Enviar">
       </td>
    </tr>
  </table>
  <div class="contacto">
    <span class="fa fa-phone"></span>☏ (+51) 912 101 970
    <span class="fa fa-envelope-o"></span>✉ rcabello@covid.com.pe
  </div>
</form>
</body></html>
)rawliteral";
/////////////////////////////////////////////////////////////////////////////////////////
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
void dontConexionSeonsorOffAlert(){
  display.clearDisplay();
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,10);                
  display.println("!Sensor No Conectado!"); 
  display.setCursor(60,20);       
  display.println("XO");  
  display.display();
  delay(500);
}
//////////////////////////////////////////////////////////////////////////////////////
void getFiltered(AsyncWebServerRequest *request)
{
  String message = "Key get filtered by " + request->getParam(PARAM_FILTER)->value();
  Serial.println(message);
  request->send(200, "text/plain", message);
}
int GetIdFromURL(AsyncWebServerRequest *request, String root)
{
  String string_id = request->url();
  string_id.replace(root, "");
  int id = string_id.toInt();
  return id;
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
  root["message"] = "Error 404 - JSON Server Not Found"; //Reads Flash Button Status 
  root.printTo(global_json_payload);  //Store JSON in String variable
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
void writeDisplayTemperaturaDefault(){
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
  display.println("00.00");  
  display.setCursor(95,15);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.println("C");
  
  display.display();
  delay(100);
  
}
void alarmaTemperaturaNormal(){
   digitalWrite(pinLedGRN,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLedGRN,LOW);
    digitalWrite(pinALERTA,LOW);
    delay(300);
    digitalWrite(pinLedGRN,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLedGRN,LOW); 
    digitalWrite(pinALERTA,LOW);
    delay(300);
    digitalWrite(pinLedGRN,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLedGRN,LOW);
    digitalWrite(pinALERTA,LOW);
}

void alarmaTemperaturaAlta(){
    digitalWrite(pinLEDRED,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLEDRED,LOW);
    digitalWrite(pinALERTA,LOW);
    delay(300);
    digitalWrite(pinLEDRED,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLEDRED,LOW);
    digitalWrite(pinALERTA,LOW);
    delay(300);
    digitalWrite(pinLEDRED,HIGH);
    digitalWrite(pinALERTA,HIGH);
    delay(300);
    digitalWrite(pinLEDRED,LOW);
    digitalWrite(pinALERTA,LOW);
}
//Metodo HTTP GET -> Metodo de Respuesta en JSON
void getByTemperatureHandler(AsyncWebServerRequest *request)
{

  int key = GetIdFromURL(request, "/temperature/");
  global_active_json_system_alert = 1; 
  if(key==1 && global_medida_coorp_objeto_str >= "36.00" && global_medida_coorp_objeto_str <= "40.00"){
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
void getAll(AsyncWebServerRequest *request)
{
  String message = "200 - Wellcome to the API TermoCovID 20";
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

void servidorAPIRestfullOn(){
   server.on("/", HTTP_GET, homeRequest);
   server.on("/temperature", HTTP_GET, getRequestTemperature); 
   server.onNotFound(notFound);
   server.begin();
   Serial.println("HTTP server API started");
}

static float readTemperaturaObjeto(){
  for(int i=0; i<20; i++){
    global_medida_coorp_objeto = termometroIR.readObjectTempC() + global_medida_coorp_objeto;
    delay(10);
  }
  global_medida_coorp_objeto /=20.0;  
  global_medida_coorp_objeto += 0.5;
  return global_medida_coorp_objeto;
}
/////////////////////////////////////////////////////////////////////////////////////////
void setup() {
   Serial.begin(9600);
   /////////////////////////////////////////////////////////////////////////////////////////////
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
   display.clearDisplay();
   display.display();
   ///////////////////////////////////////////////////////
    pinMode(pinALERTA, OUTPUT);
    pinMode(pinLEDRED, OUTPUT);
    pinMode(pinLedGRN, OUTPUT);
    pinMode(pinLedDISTANCIA, OUTPUT);
   
   ///////////////////////////////////////////////////////
   termometroIR.begin();
   /////////////////////////////////////////////////////////////////////////////////////////////
  Serial.print("Node MCU Chip Id : ");
  String chip_id = String(ESP.getChipId());
  Serial.println(chip_id);
  /////////////////////////////////////////////////////////////////////////////////////////////
  WiFi.mode(WIFI_STA);
  Serial.println();
  Serial.print("Setting "+AP_NAME_SERVER+" configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting "+AP_NAME_SERVER+" ... ");
  Serial.println(WiFi.softAP(AP_NAME_SERVER, AP_PASW_SERVER) ? "Ready" : "Failed!");
  Serial.print(AP_NAME_SERVER+" IP address = ");
  Serial.println(WiFi.softAPIP());
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });
  /////////////////////////////////////////////////////////////////////////////////////////////
  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String gettingSSID;
    String gettingPASS;
    String inputParamSSID;
    String inputParamPASS;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      gettingSSID  = request->getParam(PARAM_INPUT_1)->value();
      gettingPASS = request->getParam(PARAM_INPUT_2)->value();
      inputParamSSID  = PARAM_INPUT_1;
      inputParamPASS  = PARAM_INPUT_2;
      //WiFi.softAPdisconnect(true);
      //WiFi.disconnect(true);
    }
    else {
      gettingSSID = "No message sent SSID";
      inputParamSSID = "none SSID";
      gettingPASS = "No message sent PASS";
      inputParamPASS = "none PASS";
    }
    Serial.println(gettingSSID);
    Serial.println(gettingPASS);
    ssid = gettingSSID;
    password = gettingPASS;
    /////////////////////////////////////////////////////////////////////////////////////////////
    if (ssid != "" && password != "") {
      Serial.print("'Conectando Modulo' a la RED WiFi espere por favor (...) : ");
      Serial.println(ssid);
      delay(700);
      WiFi.begin(ssid, password);
      Serial.println("WiFi Conectado");
      Serial.println("Mi direccion IP: ");
      Serial.println(WiFi.localIP());   
    }       
    /////////////////////////////////////////////////////////////////////////////////////////////
    request->send(200, "text/html", "<script>alert('Gracias'); window.location.href = '/';</script>");
  });
  /////////////////////////////////////////////////////////////////////////////////////////////
  server.onNotFound(notFound);
  server.begin();  
  /////////////////////////////////////////////////////////////////////////////////////////////
  delay(2000); 
  
  global_active_json_server = 0;
     if(WiFi.localIP()){
        Serial.print("GET IP :");
        Serial.println(WiFi.localIP());   
      }else{
       Serial.println("IP NO ENCONTRADA");
       dontConexionWifiAlert();
     }    
}
void loop() { 
  
  if (WiFi.localIP()) {   
    
    if(global_active_json_server==0){
      servidorAPIRestfullOn();
      global_active_json_server=1;
    }
    
    
    readTemperaturaObjeto(); 
    //writeDisplayTemperaturaDefault();
    global_medida_coorp_objeto_str = String(global_medida_coorp_objeto);   
    Serial.print("T.C: ");
    Serial.println(global_medida_coorp_objeto_str);

    if(global_medida_coorp_objeto>=35.7 && global_medida_coorp_objeto <= 40.00){
       /* digitalWrite(pinLedDISTANCIA,HIGH);pinALERTA
        delay(100);
        digitalWrite(pinLedDISTANCIA,LOW);
        delay(100);*/  

        digitalWrite(pinALERTA,HIGH);
        delay(100);
        digitalWrite(pinALERTA,LOW);
        delay(100); 

        if(global_active_json_system_alert == 1){
           if(global_medida_coorp_objeto >= 37.51 && global_medida_coorp_objeto <= 40.00){
               //writeDisplayTemperatura();
               alarmaTemperaturaAlta();
               delay(1000);
           }else if(global_medida_coorp_objeto >= 36.00 && global_medida_coorp_objeto < 37.50){
              //writeDisplayTemperatura();
              alarmaTemperaturaNormal();    
              delay(1000);
           }
          Serial.println(global_active_json_system_alert);
          global_active_json_system_alert = 0;
         }
        
    }   
    
  }
}


  
