#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "pi_zam";
const char* password = "lalalayeyeye";
const char* serverip = "192.168.4.1";

WiFiUDP Udp;
unsigned int localUdpPort = 8080;  // local port to listen on
char incomingPacket[1024];  // buffer for incoming packets
int msgnum = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", serverip, localUdpPort);
}


void loop()
{
  char buff[100];
  sprintf(buff, "ESP %d\n", msgnum);
  Serial.print(buff);
  msgnum++;
  if (msgnum>100) msgnum = 0;
  
  int packetSize = Udp.parsePacket();
  Udp.beginPacket(serverip, localUdpPort);
  Udp.write(buff);
  Udp.endPacket();
  
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    Serial.printf("from server: %s\n", incomingPacket);
  }
  delay(500);
}

//https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/udp-examples.html
