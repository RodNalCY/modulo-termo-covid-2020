/////////////////////////////////////DHT-SENSOR////////////////////////////////////////////////
#include "DHT.h"
#define DHTPIN D7     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);
float medidaTEM = 0;
float medidaHUM = 0;
float medidaTempCoorporal = 33.5;
/////////////////////////////////////OLED-DISPLAY/////////////////////////////////////////////
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"` 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D4, D3);  //D4=SDA  D3=SCL  As per labeling on NodeMCU
//////////////////////////////////////LED-ALARM////////////////////////////////////////////////
#define LEDPINRED D6 
#define LEDPINGRE D5
 
void setup() {
  Serial.begin(9600);
/////////////////////////////////////////////////////////////////////////////////////////////
  dht.begin(); //Se inicia el sensor de DHT22
/////////////////////////////////////////////////////////////////////////////////////////////
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);  
  display.clear();
/////////////////////////////////////////////////////////////////////////////////////////////
pinMode(LEDPINRED,OUTPUT);
pinMode(LEDPINGRE,OUTPUT);
}

void loop() {
   
   medidaTEM = dht.readTemperature(); //Se lee la temperatura
   medidaHUM = dht.readHumidity(); //Se lee la humedad
   String th_ambiente = "T: "+ String(medidaTEM)+" °C ~ H: "+String(medidaHUM)+" %";  

        
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(2, 0, th_ambiente);
    display.setFont(ArialMT_Plain_10);
    display.drawString(4, 10, "Temperatura Coorporal"); 
    display.setFont(ArialMT_Plain_24);
    display.drawString(4, 26, String(medidaTempCoorporal)+"°C");
    //display.setFont(ArialMT_Plain_10);
    //display.drawString(82, 33, "Bien");
    display.display();
    medidaTempCoorporal+=0.5;

    
   if(medidaTempCoorporal >= 35.5 && medidaTempCoorporal <= 37.5){
     digitalWrite(LEDPINGRE,HIGH);
     delay(100);
     digitalWrite(LEDPINGRE,LOW);
     delay(100);
     digitalWrite(LEDPINGRE,HIGH);
     delay(100);
     digitalWrite(LEDPINGRE,LOW);
     delay(100);
     digitalWrite(LEDPINGRE,HIGH);
     delay(100);
     digitalWrite(LEDPINGRE,LOW);
   }else if(medidaTempCoorporal > 37.5){
     digitalWrite(LEDPINRED,HIGH);
     delay(100);
     digitalWrite(LEDPINRED,LOW);
     delay(100);
     digitalWrite(LEDPINRED,HIGH);
     delay(100);
     digitalWrite(LEDPINRED,LOW);  
     delay(100);
     digitalWrite(LEDPINRED,HIGH); 
     delay(100);
     digitalWrite(LEDPINRED,LOW);
   }

   if(medidaTempCoorporal>=40){
   medidaTempCoorporal=0;
   }
  delay(1000);
}
