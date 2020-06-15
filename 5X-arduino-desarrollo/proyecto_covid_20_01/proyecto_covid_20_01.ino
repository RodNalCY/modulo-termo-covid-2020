/* 
 * Display oled
 *  SCL -> D1
 *  SDA -> D2
 * MLX90614
 *  SCL -> D1
 *  SDA -> D2  
 */ 
//////////////////////////////////////////////OLED_DISPLAY/////////////////////////////////////////////////////
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//////////////////////////////////////////////Adafruit_MLX90614/////////////////////////////////////////////////////
#include <Adafruit_MLX90614.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float global_temperatura_objeto = 0; // Distancia de 1 a 10 cm
//////////////////////////////////////////////DHT 22/////////////////////////////////////////////////////
#include "DHT.h"
#define DHTPIN D7     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);
float global_medidaTEM = 0;
float global_medidaHUM = 0;
float global_medidaCoorporal = 0;
//////////////////////////////////////////////LED-ALARMAS/////////////////////////////////////////////////////
void setup() { 
   Serial.begin(9600); 
/////////////////////////////////////////////////////////////////////////////////////////////
  mlx.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();
/////////////////////////////////////////////////////////////////////////////////////////////
  dht.begin(); //Se inicia el sensor de DHT22
/////////////////////////////////////////////////////////////////////////////////////////////
}

void loop() {

   global_medidaTEM = dht.readTemperature(); //Se lee la temperatura
   global_medidaHUM = dht.readHumidity(); //Se lee la humedad
   global_medidaCoorporal = readTemperaturaObjeto();     
   writeDisplayTemperatura();
}

static float readTemperaturaObjeto(){
  for(int i=0; i<20; i++){
    global_temperatura_objeto = mlx.readObjectTempC() + global_temperatura_objeto;
    delay(10);
  }
  global_temperatura_objeto /=20.0;
  //global_temperatura_objeto += 2.22;
  return global_temperatura_objeto;
}

void writeDisplayTemperatura(){
  display.clearDisplay();

  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,1); 
  display.print("IP: ");    
  display.println("192.168.100.149"); 
  
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,9); 
  display.print("T:");                
  display.print(global_medidaTEM,1);
  display.print(" "); 
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.print("C");
  display.print(" - "); 
  display.print("H:"); 
  display.print(global_medidaHUM,1); 
  display.println(" %"); 
  
 
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,18);                
  display.println("T.C : "); 
  
  display.setTextSize(2);
  display.setCursor(37,18);
  display.println(global_medidaCoorporal,1);  
  display.setCursor(95,18);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.println("C");
  
  display.display();
  delay(500);
  
}
