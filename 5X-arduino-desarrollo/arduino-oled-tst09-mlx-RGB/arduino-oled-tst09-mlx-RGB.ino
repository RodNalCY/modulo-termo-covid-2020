/// => https://www.youtube.com/watch?v=eLann3shOT4
/// => https://programarfacil.com/blog/arduino-blog/termometro-infrarrojo-con-arduino-mlx90614/

#include <Adafruit_MLX90614.h> 
// Instanciar objeto
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();
float global_temperatura_objeto = 0; // Distancia de 1 a 10 cm

#define LED_RED     D5 
#define LED_GREEN   D6 
#define LED_BLUE    D7

void setup() {
  // Iniciar comunicación serie
  Serial.begin(9600); 
  // Iniciar termómetro infrarrojo con Arduino
  termometroIR.begin();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  analogWriteRange(255);
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
}
 
void loop() {
  
readTemperaturaObjeto();
  Serial.print("Temp. objeto   => ");
  Serial.print(global_temperatura_objeto);
  Serial.println(" ºC");

  if(global_temperatura_objeto>=35.5){
     alarmaBlueAlert();
    if(global_temperatura_objeto >= 36 && global_temperatura_objeto<37.5){
        alarmaGreenAlert();
    }else if(global_temperatura_objeto >= 37.5 && global_temperatura_objeto <= 40.5){
       alarmaRedAlert();
    }
    
  } 
 
}

void alarmaBlueAlert(){
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 1);  
  delay(100);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(100);
  
}
void alarmaRedAlert(){
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_RED, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_RED, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_RED, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
}
void alarmaGreenAlert(){
  analogWrite(LED_RED, 255);    
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_GREEN, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
  analogWrite(LED_RED, 255);    
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_GREEN, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
  analogWrite(LED_RED, 255);    
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_GREEN, 1);
  delay(1000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
}

void readTemperaturaObjeto(){

  for(int i=0; i<20; i++){
    global_temperatura_objeto = termometroIR.readObjectTempC() + global_temperatura_objeto;
    delay(10);
  }
  global_temperatura_objeto /=20.0;
  global_temperatura_objeto += 0.5;
  
}
