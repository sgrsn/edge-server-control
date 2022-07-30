#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include "index_html.h"

const char *ssid = "ESP32ap";
const char *password = "12345678";
const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0); 

Servo servo1;

const int esc_pin = 21;
const int led_pin = 22;
String slider_value = "0";
// Published values for SG90 servos; adjust if needed
int minUs = 800;
int maxUs = 2400;

const char* input_parameter = "value";

AsyncWebServer server(80);
const char* index_html = INDEX_HTML; // index_html.hより読み込み

String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return slider_value;
  }
  return String();
}

void setup(){
  Serial.begin(9600);
  
  pinMode(led_pin, OUTPUT);

  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  //WiFi.begin(ssid, password);
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }*/

  //Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter)) {
      message = request->getParam(input_parameter)->value();
      servo1.writeMicroseconds(map(message.toInt(), 0, 255, minUs, maxUs));
    }
    else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();

  // Calibration ESC
  ESP32PWM::allocateTimer(0);
  servo1.setPeriodHertz(50);      // Standard 50hz servo
  servo1.attach(esc_pin, minUs, maxUs);
  servo1.writeMicroseconds(800);
}
  
void loop() {
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(500);
}