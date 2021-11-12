#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESPAsyncTCP.h>

const char *ssid = "som_sam2";  // replace with your SSID
const char *password = "0630407836";  // replace with your Password

int prevButtonState = 0;
int currButtonState = 0; 
const int buttonPin = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

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

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected");
      HTTPClient http;

      http.begin("http://maker.ifttt.com/trigger/XXXX/with/key/XXXXXXXXXX?value1=28");
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
