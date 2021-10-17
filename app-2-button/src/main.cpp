#include <Arduino.h>

int buttonState = 0; 
const int buttonPin = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.print("buttonState ");
  Serial.println(buttonState);

  if (buttonState == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

// void loop() {
//   prevState = buttonState;
//   buttonState = digitalRead(buttonPin);
//   Serial.print("buttonState ");
//   Serial.println(buttonState);
  
//   if (locked == false) {
//     if (buttonState == 0) {
//       digitalWrite(LED_BUILTIN, LOW);
//     } else {
//       digitalWrite(LED_BUILTIN, HIGH);
//     }
//   }

//   if (buttonState == 0 && prevState == 1) {
//     counter += 1;
//     Serial.print("counter ");
//     Serial.println(counter);
//   }

//   if (counter >= 5 && counter < 10) {
//     digitalWrite(LED_BUILTIN, LOW);
//     locked = true;
//   }

//   if (counter >= 10) {
//     // digitalWrite(LED_BUILTIN, HIGH);
//     locked = false;
//   }

// }