#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

const char *ssid = "AndroidAP_9281";  // replace with your SSID
const char *password = "12346780";  // replace with your Password


WiFiUDP UdpIn;                              // A UDP instance to let us send and receive packets over UDP
WiFiUDP UdpOut;                             // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 43, 140);   // remote IP of your computer
const unsigned int outPort = 554433;        // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

unsigned int ledState = LOW;
OSCErrorCode error;

int temp = 0;
int pressure = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(1000);

    Serial.print(".");
    Serial.print(WiFi.status());
    Serial.println(WiFi.localIP().toString());
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.print(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  UdpIn.begin(localPort);

  Serial.print("Local port: ");
  Serial.println(UdpIn.localPort());

  randomSeed(analogRead(0));
}

void led(OSCMessage &msg) {
  ledState = msg.getInt(0);
  digitalWrite(BUILTIN_LED, ledState);

  Serial.print("/led: ");
  Serial.println(ledState);
}

void loop()
{
  // SEND DATA

  // read data (generate random data)
  temp = random(100);
  pressure = random(100000);

  OSCMessage msgSend("/vvvv");
  msgSend.add(temp);
  msgSend.add(pressure);

  // msg.add("Text");
  // msg.add(3456);
  UdpOut.beginPacket(outIp, outPort);
  msgSend.send(UdpOut);
  
  Serial.print("Sent ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(pressure);

  UdpOut.endPacket();
  msgSend.empty();
  delay(500);


  // RECEIVE DATA
  OSCBundle msgIn;
  int size = UdpIn.parsePacket();

  if (size > 0) {
    while (size--) {
      msgIn.fill(UdpIn.read());
    }

    if (!msgIn.hasError()) {
      msgIn.dispatch("/led", led);
    } else {
      error = msgIn.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
