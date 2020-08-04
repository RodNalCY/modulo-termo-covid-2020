int i=0;
#define BUZZER_ACTIVO D8

void setup(){
  pinMode(BUZZER_ACTIVO, OUTPUT);
}

void loop(){
//distancia();
//good();
//bad();
tst();
}

void distancia(){
  tone(BUZZER_ACTIVO, 1000, 500);
  delay(1000);
}
void good(){
  tone(BUZZER_ACTIVO, 2000, 500);
  delay(1000);
}
void bad(){
  tone(BUZZER_ACTIVO, 125, 500);
  delay(1000);
}

void tst(){
  tone(BUZZER_ACTIVO, 125, 500);
  delay(500);
}
