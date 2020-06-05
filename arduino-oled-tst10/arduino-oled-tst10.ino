#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64 // alto pantalla OLED
 
// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);
 
// Temporizador
unsigned long marcaTiempoDate = 0;
unsigned long tiempoRefreshDate = 1000;
 
// Variables almacena tiempo millis
int dias;
int horas;
int minutos;
int segundos;
 
// Cadena para almacenar texto formateado
char cadenaTiempo[16];
 
/*
  Función que convierte millis() a segundos, minutos, horas y días
  Almacena la información en variables globales
*/
void millisToTiempo(unsigned long valMillis) {
  // Se obtienen los segundos
  valMillis = valMillis / 1000;
 
  segundos = valMillis % 60; // se divide entre segundos por minuto y te quedas con el resto
  minutos = valMillis / 60; // Se convierte a minutos
  minutos = minutos % 60; // se divide entre minutos por hora y te quedas con el resto
  horas = (valMillis / 60) / 60; // Se convierte en horas
  horas = horas % 24; // se divide entre horas al día y te quedas con el resto
  dias = ((valMillis / 60) / 60) / 24; // Se convierte en días
#ifdef __DEBUG__
  Serial.print("Segundos = ");
  Serial.println(valMillis);
  Serial.print(dias);
  Serial.print(":");
  Serial.print(horas);
  Serial.print(":");
  Serial.print(minutos);
  Serial.print(":");
  Serial.println(segundos);
#endif
}
 
void setup() {
#ifdef __DEBUG__
  Serial.begin(9600);
  delay(100);
  Serial.println("Iniciando pantalla OLED");
#endif
 
  // Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
#ifdef __DEBUG__
    Serial.println("No se encuentra la pantalla OLED");
#endif
    while (true);
  }
}
 
void loop() {
 
  // Protección overflow
  if (millis() < marcaTiempoDate) {
    marcaTiempoDate = millis();
  }
 
  // Comprobar is hay que actualizar temperatura
  if (millis() - marcaTiempoDate >= tiempoRefreshDate)
  {
    // Actualizar variables de tiempo
    millisToTiempo(millis());
    // Componer cadena con la información del tiempo formateada
    sprintf(cadenaTiempo, "%02d:%02d:%02d:%02d", dias, horas, minutos, segundos);
    // Marca de tiempo
    marcaTiempoDate = millis();
  }
 
  // Limpiar buffer pantalla
  display.clearDisplay();
  // Dibujar línea horizontal
  display.drawLine(0, 18, display.width(), 18, SSD1306_WHITE);
 
  // Dibujar texto tiempo
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(cadenaTiempo);
 
  // Enviar a pantalla
  display.display();
}
