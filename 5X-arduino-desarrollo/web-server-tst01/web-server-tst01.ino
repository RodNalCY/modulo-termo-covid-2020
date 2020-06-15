#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
 
//Nombre de la red
const char* nombre = "$_WiFi-Plus_$";
 
//Password
const char* password = "MiRedWifi9876543210AZ";
 
//Puerto (dejar en 80 por defecto)
WiFiServer server(80);
 
StaticJsonDocument<200> doc;
JsonArray data = doc.createNestedArray("data");

void setup() {
  Serial.begin(9600);
  delay(50); //Hay que dejar un margen de tiempo
 
  
  // Conectar a la red WiFi
  WiFi.begin(nombre, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(100); //aqui tambien hay que dejar un margen de tiempo
  Serial.println("Conectado a la red WiFi");
  
  // Iniciar el servidor
  server.begin();
  Serial.println("Se ha iniciado el servidor");
  
  // Escribir la direccion IP
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

   
 
  
}
  
void loop() {
  // Comprobar si se ha conectado algun cliente
  WiFiClient client = server.available();
   
  if (client) {
    Serial.println("Nueva conexion");
    ///////////////////////////////////////////////////
   doc["sensor"] = "gps";
   doc["time"] = 1351824120;    
    data.add(48.756080);
    data.add(2.302038);
    data = doc.createNestedArray("data");
    serializeJson(doc, Serial);
    Serial.println();
    serializeJsonPretty(doc, Serial);
    //////////////////////////////////////////////////
    
    // Escribir las cabeceras HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    /*client.println("");
    client.println("<!DOCTYPE HTML>");   
    //Escribir el contenido que vamos a visualizar
    client.println("<html>");
    client.print("ROBOLOGS")*/;
    serializeJson(doc, client);
  /*  client.println("</html>");*/
     
  }
  
}
