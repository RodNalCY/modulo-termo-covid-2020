/// => https://www.youtube.com/watch?v=eLann3shOT4
/// => https://programarfacil.com/blog/arduino-blog/termometro-infrarrojo-con-arduino-mlx90614/

#include <Adafruit_MLX90614.h> 
// Instanciar objeto
Adafruit_MLX90614 termometroIR = Adafruit_MLX90614();
float global_temperatura_objeto = 0; // Distancia de 1 a 10 cm

#define pinALERTA   D5 
#define pinLedGRN   D6
#define pinLEDRED   D7
void setup() {
  // Iniciar comunicación serie
  Serial.begin(9600); 
  // Iniciar termómetro infrarrojo con Arduino
  termometroIR.begin();

  pinMode(pinALERTA, OUTPUT);
  pinMode(pinLEDRED, OUTPUT);
  pinMode(pinLedGRN, OUTPUT);
}
 
void loop() {
  
readTemperaturaObjeto();
  Serial.print("Temp. objeto   => ");
  Serial.print(global_temperatura_objeto);
  Serial.println(" ºC");

  if(global_temperatura_objeto >= 36 && global_temperatura_objeto<37.5){
     alarmaTemperaturaNormal();
  }else if(global_temperatura_objeto >= 37.5){
     alarmaTemperaturaAlta();
  }
 
}

void readTemperaturaObjeto(){

  for(int i=0; i<20; i++){
    global_temperatura_objeto = termometroIR.readObjectTempC() + global_temperatura_objeto;
    delay(10);
  }
  global_temperatura_objeto /=20.0;
  //global_temperatura_objeto += 2.22;
  
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
