//////////////////////////WIFI//CREDENCIALES///////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////
// HTML web page to handle 3 input fields (ssid, password, passwordconf)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta http-equiv="X-UA-Compatible" content="ie=edge">
<title>Termo Covid 20</title>
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
          <input type="text" 
          name="password" 
          id="password" 
          placeholder="✎ Ingrese su password"></input>
        </p>
        <p type="Password:">
          <input type="text" 
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
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {

  /////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(9600); 
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
     if(WiFi.localIP()){
        Serial.print("GET IP :");
        Serial.println(WiFi.localIP());
      }else{
       Serial.println("IP NO ENCONTRADA");
     }   

}

void loop() {  
  
  if (WiFi.localIP()) { 
       Serial.println("Hola Mundo");
       delay(2000);
    
  }
}
