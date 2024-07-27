#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include <stdio.h>
#include <SPI.h>
#include <WiFiUdp.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>
#include <Preferences.h>

Preferences preferences;

const double joistick_radius = 60.0;

/* WiFi */
const char *ssid = "ESP32ap";
const char *password = "12345678";
const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0); 
WiFiUDP udp;
WebServer server(80);

/* ESC */
Servo esc;
const int esc_pin = D0;
int esc_minUs = 800;
int esc_maxUs = 2000;

/* Servo for Horizontal Wing */
Servo servo;
const int servo_pin = D2;
int servo_minUs = 1000;
int servo_maxUs = 2000;
int servo_neutral = 1500;

/* Servo for Vertical Wing */
Servo servo2;
const int servo2_pin = D1;
int servo2_minUs = 1200;
int servo2_maxUs = 1800;
int servo2_neutral = 1500;


void handleSaveParams() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, body);
    
    servo_maxUs     = doc["param1"];
    servo_minUs     = doc["param2"];
    servo2_maxUs    = doc["param3"];
    servo2_neutral  = doc["param4"];
    servo2_minUs    = doc["param5"];
    esc_maxUs       = doc["param6"];

    /* Preferences */
    preferences.begin("my-params", false);
    preferences.putInt("param1", doc["param1"]);
    preferences.putInt("param2", doc["param2"]);
    preferences.putInt("param3", doc["param3"]);
    preferences.putInt("param4", doc["param4"]);
    preferences.putInt("param5", doc["param5"]);
    preferences.putInt("param6", doc["param6"]);
    preferences.end();
    
    server.send(200, "text/plain", "Parameters saved");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void handleGetParameter() {
  preferences.begin("my-params", false);
  int param1 = preferences.getInt("param1", -1);
  int param2 = preferences.getInt("param2", -1);
  int param3 = preferences.getInt("param3", -1);
  int param4 = preferences.getInt("param4", -1);
  int param5 = preferences.getInt("param5", -1);
  int param6 = preferences.getInt("param6", -1);
  preferences.end();
  // JSON形式で値を返す
  String jsonResponse = "{\"param1\":" + String(param1) + 
                          ",\"param2\":" + String(param2) +
                          ",\"param3\":" + String(param3) +
                          ",\"param4\":" + String(param4) +
                          ",\"param5\":" + String(param5) + 
                          ",\"param6\":" + String(param6) + "}";
  server.send(200, "application/json", jsonResponse);
}

void handleData() {
  const int left_x = server.arg("leftX").toInt();
  const int left_y = server.arg("leftY").toInt();
  const int right_x = server.arg("rightX").toInt();
  const int right_y = server.arg("rightY").toInt();
  const bool leftButton = server.arg("leftButton").toInt();
  const bool rightButton = server.arg("rightButton").toInt();

  /* response to client*/
  server.send(200, "text/plain", "");

  /* message */
  Serial.printf("left X: %d left Y: %d right X: %d right Y: %d L: %d R: %d\n", left_x, left_y, right_x, right_y, leftButton, rightButton);

  /* Control esc */
  int esc_min_duty = map(esc_minUs, 0, 20000, 0, 1023);
  int esc_max_duty = map(esc_maxUs, 0, 20000, 0, 1023);
  ledcWrite(1, map(constrain(left_y, 0, joistick_radius), 0, joistick_radius, esc_min_duty, esc_max_duty));

  /* Control horizontal servo */
  int servo_min_duty = map(servo_minUs, 0, 20000, 0, 1023);
  int servo_max_duty = map(servo_maxUs, 0, 20000, 0, 1023);
  ledcWrite(2, map(constrain(right_y, -joistick_radius, joistick_radius), -joistick_radius, joistick_radius, servo_min_duty, servo_max_duty));

  /* Control vertical servo */
  //  state     | right button | left button
  //  neutoral  |      0       |     0
  //  max       |      0       |     1
  //  min       |      1       |     0
  //  neutoral  |      1       |     1
  int servo2_us = servo2_maxUs * rightButton + servo2_minUs * leftButton;
  if (rightButton == leftButton) {
    servo2_us = servo2_neutral;
  }
  ledcWrite(3, map(servo2_us, 0, 20000, 0, 1023));
}

void setup() {
  /* Serial */
  Serial.begin(9600);

  /* Calibration ESC */
  pinMode(esc_pin, OUTPUT);
  ledcSetup(1, 50, 10);
  ledcAttachPin(esc_pin, 1);
  int esc_min_duty = map(esc_minUs, 0, 20000, 0, 1023);
  ledcWrite(1, esc_min_duty);

  /* Calibration Horizontal Servo */
  pinMode(servo_pin, OUTPUT);
  ledcSetup(2, 50, 10);
  ledcAttachPin(servo_pin, 2);
  int servo_min_duty = map(servo_minUs, 0, 20000, 0, 1023);
  ledcWrite(2, servo_min_duty);

  /* Calibration Vertical Servo */
  pinMode(servo2_pin, OUTPUT);
  ledcSetup(3, 50, 10);
  ledcAttachPin(servo2_pin, 3);
  int servo2_min_duty = map(servo2_neutral, 0, 20000, 0, 1023);
  ledcWrite(3, servo2_min_duty);

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
  server.serveStatic("/", SPIFFS, "/index.html");
  server.serveStatic("/styles.css", SPIFFS, "/styles.css");
  server.serveStatic("/script.js", SPIFFS, "/script.js");
  server.serveStatic("/joystick.js", SPIFFS, "/joystick.js");
  server.serveStatic("/virtualjoystick.js", SPIFFS, "/virtualjoystick.js");
  server.serveStatic("/params.html", SPIFFS, "/params.html");
  server.serveStatic("/params.js", SPIFFS, "/params.js");
  
  server.on("/index.html", HTTP_PUT, handleData);
  server.on("/save-params", HTTP_POST, handleSaveParams);
  server.on("/getParameters", handleGetParameter);
  server.begin();
  Serial.println("start server");
}

void loop() {
  server.handleClient();
}