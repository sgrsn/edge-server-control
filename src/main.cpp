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

unsigned long http_watchdog_time = 0;

const char* input_parameter = "value";

AsyncWebServer server(80);
const char* index_html = INDEX_HTML; // index_html.hより読み込み

hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer() {
  digitalWrite(led_pin, !digitalRead(led_pin));
  http_watchdog_time++;
  if (http_watchdog_time > 5)
  {
    servo1.writeMicroseconds(minUs);
    digitalWrite(led_pin, HIGH);
  }
}

String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return slider_value;
  }
  return String();
}

void setup(){
  Serial.begin(9600);
  
  pinMode(led_pin, OUTPUT);

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);

  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    timerAlarmEnable(timer);
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
    http_watchdog_time = 0;
  });
  
  server.begin();

  // Calibration ESC
  ESP32PWM::allocateTimer(0);
  servo1.setPeriodHertz(50);      // Standard 50hz servo
  servo1.attach(esc_pin, minUs, maxUs);
  servo1.writeMicroseconds(minUs);
}
  
void loop() {
}