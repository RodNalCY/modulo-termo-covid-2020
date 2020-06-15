/* This code works with MLX90614 (GY906) and OLED screen
 * It measures both ambient and object temperature in Fahrenheit and display it on the screen
 * Please visit www.surtrtech.com for more details
 * https://surtrtech.com/2019/05/04/arduino-contactless-thermometer-with-mlx90614-oled-lcd/
 *  
 * Display oled
 *  SCL -> D1
 *  SDA -> D2
 * MLX90614
 *  SCL -> D1
 *  SDA -> D2  
 */ 



#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float global_temperatura_objeto = 0; // Distancia de 1 a 10 cm

void setup() {  
  mlx.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();

}

void loop() {

  readTemperaturaObjeto();  
  writeDisplayTemperatura();
}

void readTemperaturaObjeto(){

  for(int i=0; i<20; i++){
    global_temperatura_objeto = mlx.readObjectTempC() + global_temperatura_objeto;
    delay(10);
  }
  global_temperatura_objeto /=20.0;
  //global_temperatura_objeto += 2.22;
  
}

void writeDisplayTemperatura(){
  display.clearDisplay();
  
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,2); 
  display.print("T:");                
  display.print("90.0");
  display.print(" "); 
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.print("C");
  display.print(" - "); 
  display.print("H:"); 
  display.print("82.3"); 
  display.println(" %"); 
  
 
  display.setTextSize(1);                    
  display.setTextColor(WHITE);             
  display.setCursor(0,18);                
  display.println("T.C : "); 
  
  display.setTextSize(2);
  display.setCursor(37,15);
  display.println(global_temperatura_objeto,1);  
  display.setCursor(95,15);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.println("C");
  
  display.display();
  
  delay(800);
  
}
