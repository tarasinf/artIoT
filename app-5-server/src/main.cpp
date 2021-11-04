#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>


// const char *ssid = "G6";  // replace with your SSID
// const char *password = "quertyui";  // replace with your Password

const char *ssid = "Hatka";  // replace with your SSID
const char *password = "***";  // replace with your Password

const uint8_t servoPin = D4;  // replace with servo pin
/* Create Servo Object */
Servo servo;
// Create Server instance
AsyncWebServer server(80);
void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  servo.attach(servoPin);
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    WL_NO_SHIELD;
    WL_IDLE_STATUS;
    WL_NO_SSID_AVAIL;
    WL_SCAN_COMPLETED;
    WL_CONNECT_FAILED;
    delay(1000);

    Serial.print(".");
    Serial.print(WiFi.status());
    Serial.println(WiFi.localIP().toString());
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print("IP address:");
  Serial.print(WiFi.localIP());

  // Send home page from SPIFFS
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Send index.html.");
    request->send(SPIFFS, "/index.html", "text/html");
  });
  // Receive Angle from client and process it 
  server.on("/angle", HTTP_POST, [](AsyncWebServerRequest *request) {
    String angle = request->arg("angle");
    Serial.println("Current Position: " + angle + "°");
    servo.write(angle.toInt());
    request->send(200);
  });

  server.on("/turn", HTTP_POST, [](AsyncWebServerRequest *request) {
    String delta = request->arg("delta");
    int angle = servo.read();
    Serial.println("Current Position: " + String(angle) + "°" + " moving " + delta);
    servo.write(delta.toInt() + angle);
    request->send(200);
  });

  server.on("/led", HTTP_POST, [](AsyncWebServerRequest *request) {
    String state = request->arg("state");
    Serial.println("Led: " + String(state));

    digitalWrite(LED_BUILTIN, state.toInt());
    request->send(200);
  });

  

  // Send Favicon 
  server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
  // Begin Server
  server.begin();
}
void loop()
{
}
