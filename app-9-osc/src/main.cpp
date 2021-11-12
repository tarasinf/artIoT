#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

const char *ssid = "AndroidAP_9281";  // replace with your SSID
const char *password = "12346780";  // replace with your Password


WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192,168,43,140);      // remote IP of your computer
const unsigned int outPort = 554433;        // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)
int temp = 0;
int pressure = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

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
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

  randomSeed(analogRead(0));
}
void loop()
{
  // read data (generate random data)
  temp = random(100);
  pressure = random(100000);

  OSCMessage msg("/vvvv");
  msg.add(temp);
  msg.add(pressure);

  // msg.add("Text");
  // msg.add(3456);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  
  Serial.print("Sent ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(pressure);

  Udp.endPacket();
  msg.empty();
  delay(500);
}
