/// => https://www.youtube.com/watch?v=eLann3shOT4
/// => https://programarfacil.com/blog/arduino-blog/termometro-infrarrojo-con-arduino-mlx90614/
////////////////////////////////////MLX90614/////////////////////////////////////////////
#include <Adafruit_MLX90614.h> 
// Instanciar objeto
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();
float global_temperatura_objeto = 0; // Distancia de 1 a 10 cm
/////////////////////////////////////OLED-DISPLAY/////////////////////////////////////////////
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy include: `#include "SSD1306.h"` 
// Initialize the OLED display using Wire library
SSD1306Wire  display(0x3c, D2, D1);  //D2=SDA  D1=SCL  As per labeling on NodeMCU

void setup() {
  // Iniciar comunicación serie
  Serial.begin(9600); 
  // Iniciar termómetro infrarrojo con Arduino
  termometroIR.begin();

  /////////////////////////////////////////////////////////////////////////////////////////////
 /* display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);  
  display.clear();
 */
}
 
void loop() {
  
  readTemperaturaObjeto();
  Serial.print("Temp. objeto   => ");
  Serial.print(global_temperatura_objeto);
  Serial.println(" ºC");

  /*if(global_temperatura_objeto >= 36 && global_temperatura_objeto<37.5){
     Serial.print(" :: SALUDABLE :: ");
  }else if(global_temperatura_objeto >= 37.5){
    Serial.print(" :: ENFERMO :: ");
  }*/

  /* String th_ambiente = "T: 24.00 °C ~ H:80 %";          
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(2, 0, th_ambiente);
    display.setFont(ArialMT_Plain_10);
    display.drawString(4, 10, "Temperatura Coorporal"); 
    display.setFont(ArialMT_Plain_24);
    display.drawString(4, 26, String(global_temperatura_objeto)+"°C");
    //display.setFont(ArialMT_Plain_10);
    //display.drawString(82, 33, "Bien");
    display.display();*/
 
}

void readTemperaturaObjeto(){

  for(int i=0; i<20; i++){
    global_temperatura_objeto = termometroIR.readObjectTempC() + global_temperatura_objeto;
    delay(10);
  }
  global_temperatura_objeto /=20.0;
  //global_temperatura_objeto += 2.22;
  
}
