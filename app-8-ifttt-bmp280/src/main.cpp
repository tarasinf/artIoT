#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280  bmp;

const char *ssid = "som_sam2";  // replace with your SSID
const char *password = "0630407836";  // replace with your Password

float temp = 0;
int prevButtonState = 0;
int currButtonState = 0; 
const int buttonPin = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

    if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor"));
    while (1) delay(10);
  }
  bmp.setSampling( /* Default settings from datasheet. */
    Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(1000);

    Serial.print(".");
    Serial.print(WiFi.status());
    Serial.println(WiFi.localIP().toString());
  }
  Serial.println("Connected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print(" IP address:");
  Serial.println(WiFi.localIP());
}

void loop()
{
  prevButtonState = currButtonState;
  currButtonState = digitalRead(buttonPin);

  // 0 - pressed
  // 1 - not pressed
  if (currButtonState == 0 && prevButtonState == 1) {
    Serial.println("Pressed");
    temp = bmp.readTemperature();
    Serial.print(F("Temperature = "));
    Serial.print(temp);
    Serial.println(" *C");

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected");
      HTTPClient http;

      http.begin("http://maker.ifttt.com/trigger/EVENT_NAME/with/key/SECRET_KEY?value1=" + String(temp));
      int httpCode = http.GET();  // Send the request                                                           
      Serial.println("Sent");

      if (httpCode > 0) {
        String payload = http.getString();  // Get the request response payload
        Serial.print("Response: ");
        Serial.println(payload);
      }
      http.end();  // Close connection
    }
  }
}
