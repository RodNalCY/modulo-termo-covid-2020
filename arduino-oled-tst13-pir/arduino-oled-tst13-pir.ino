#define PIR_PIN D6    
#define VOCINA_PIN D7    
int estado_pir = 0;
void setup() {
 Serial.begin(9600);
 pinMode(PIR_PIN, INPUT);
 pinMode(VOCINA_PIN, OUTPUT);

}

void loop() { 
estado_pir = digitalRead(PIR_PIN);
Serial.print("PIR: ");
Serial.println(estado_pir);
delay(1000);
}
