#include <Arduino.h>
#include <Servo.h>
// https://community.platformio.org/t/solved-cant-use-servo-library-with-esp8266-in-platformio-but-it-works-in-arduino-ide/20647/3

Servo servo;
int pos = 0;


void setup() {
  Serial.begin(9600);

  servo.attach(2);  // D4
  servo.write(0);
  delay(2000);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                    // waits 100 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                    // waits 100 ms for the servo to reach the position
  }
  delay(1000);
}
