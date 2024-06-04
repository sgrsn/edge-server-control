#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include <stdio.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include "SPIFFS.h"

/* WiFi */
const char *ssid = "ESP32ap";
const char *password = "12345678";
const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0); 
WiFiUDP udp;
WebServer server(80);

int left_x;
int left_y;
int right_x;
int right_y;

void handleData() {
  left_x = server.arg("leftX").toInt();
  left_y = server.arg("leftY").toInt();
  right_x = server.arg("rightX").toInt();
  right_y = server.arg("rightY").toInt();

  /* response to client*/
  server.send(200, "text/plain", "");

  Serial.printf("left X: %d left Y: %d right X: %d right Y: %d\n", left_x, left_y, right_x, right_y);
}

void setup() {
  /* Serial */
  Serial.begin(9600);
  /* Wifi access point Setup */
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* UDP */
  unsigned int localUdpPort = 4210;
  Serial.println("begin UDP port");
  udp.begin(localUdpPort);
  Serial.print("local UDP port: ");
  Serial.println(localUdpPort);
  
  /* SPIFFS */
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed to mount");
    Serial.println("Please run following command to format SPIFFS:");
    Serial.println("\033[31mpio run -t uploadfs\033[0m");
  } else {
    Serial.println("SPIFFS OK");
  }

  /* WebServer */
  server.serveStatic("/", SPIFFS, "/joystick.html");
  server.serveStatic("/virtualjoystick.js", SPIFFS, "/virtualjoystick.js");
  server.on("/index.html", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}