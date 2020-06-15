#include "DHT.h"
#define DHTPIN D5     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);
float medidaTEM = 0;
float medidaHUM = 0;

void setup() {
  Serial.begin(9600);
  /////////////////////////////////////////////////////////////////////////////////////////////
  dht.begin(); //Se inicia el sensor de DHT22
  /////////////////////////////////////////////////////////////////////////////////////////////

}

void loop() {

   medidaTEM = dht.readTemperature(); //Se lee la temperatura
   medidaHUM = dht.readHumidity(); //Se lee la humedad
      Serial.println("-----------------------------------------");
      Serial.print("Humedad: ");
      Serial.println(medidaHUM);
      Serial.print("Temperatura: ");
      Serial.println(medidaTEM);
      Serial.println("-----------------------------------------");

      delay(2000);
}
