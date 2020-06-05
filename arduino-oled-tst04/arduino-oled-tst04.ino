#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SSD1306.h"
const char* ssid = "WiFi-INO"; //Nombre de la red WiFi a la que nos vamos a conectar
const char* password = "987654321AZ"; //Contrasena de la red WiFi a la que nos vamos a conectar
SSD1306 display(0x3c, 4, 5); //GPIO 5 = D1, GPIO 4 = D2
#define flipDisplay true

void setup(){
Serial.begin(115200);
delay(10);
/* Inicializamos la pantalla */
display.init();
if (flipDisplay) display.flipScreenVertically();

/* Mostramos la pantalla de bienvenida */
display.clear();
display.setFont(ArialMT_Plain_16);
display.drawString(15, 25, "Gestores");
display.display();
delay (3000);  
 /*Mostrar red en pantala*/
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid); 
 display.clear();
 display.setFont(ArialMT_Plain_16);
 display.drawString(0, 0, "Connecting to:");
 display.drawString(0, 32, ssid);
 display.display();

 /*Conectarnos a la red WiFi*/
 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
}
void loop(){}
