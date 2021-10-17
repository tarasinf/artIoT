#include <Arduino.h>

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED On!");

  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED Off!");
  delay(1000);  
}
