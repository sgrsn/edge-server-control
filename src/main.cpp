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

/* ESC */
Servo esc;
const int esc_pin = 2;
int esc_minUs = 800;
int esc_maxUs = 2200;

/* Servo */
Servo servo;
const int servo_pin = 4;
int servo_minUs = 1500;
int servo_maxUs = 1800;

void handleData() {
  const int left_x = server.arg("leftX").toInt();
  const int left_y = server.arg("leftY").toInt();
  const int right_x = server.arg("rightX").toInt();
  const int right_y = server.arg("rightY").toInt();

  /* response to client*/
  server.send(200, "text/plain", "");

  /* message */
  Serial.printf("left X: %d left Y: %d right X: %d right Y: %d\n", left_x, left_y, right_x, right_y);

  /* Control esc */
  esc.writeMicroseconds(map(left_y, -120, 120, esc_minUs, esc_maxUs));
}

void setup() {
  /* Serial */
  Serial.begin(9600);

  /* Calibration ESC */
  ESP32PWM::allocateTimer(0);
  esc.setPeriodHertz(50);      // Standard 50hz servo
  esc.attach(esc_pin, esc_minUs, esc_maxUs);
  esc.writeMicroseconds(esc_minUs);
  servo.setPeriodHertz(50);      // Standard 50hz servo
  servo.attach(servo_pin, servo_minUs, servo_maxUs);
  servo.writeMicroseconds(map(0, -120, 120, servo_minUs, servo_maxUs));

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