#define LED_RED     D5 // D1
#define LED_GREEN   D6 // D6
#define LED_BLUE    D7 // D7

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  analogWriteRange(255);
  analogWrite(LED_RED, 255);
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
}

void loop() {
  
  alarmaRGBall();
}

void alarmaBlueAlert(){
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 1);  
  delay(500);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
}
void alarmaRedAlert(){
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_RED, 1);
  delay(500);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
}
void alarmaGreenAlert(){
  analogWrite(LED_RED, 255);    
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_GREEN, 1);
  delay(500);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);  
  delay(500);
}
void alarmaRGBall(){
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_RED, 1);
  delay(2000);
  analogWrite(LED_RED, 255);    
  analogWrite(LED_BLUE, 255);
  analogWrite(LED_GREEN, 1);
  delay(2000);
  analogWrite(LED_RED, 255);  
  analogWrite(LED_GREEN, 255);
  analogWrite(LED_BLUE, 1);  
  delay(2000);

}
