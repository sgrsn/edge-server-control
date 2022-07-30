#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include "index_html.h"

/* WiFi */
const char *ssid = "ESP32ap";
const char *password = "12345678";
const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0); 

/* ESC */
Servo servo1;
const int esc_pin = 21;
const int led_pin = 22;
int minUs = 800;
int maxUs = 2400;

/* Watchdogtimer */
unsigned long http_watchdog_time = 0;

/* HTTP Server */
AsyncWebServer server(80);
const char* index_html = INDEX_HTML; // index_html.hより読み込み
const char* input_parameter = "value";

/* Timer (watchdogtimer) */
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer() {
  http_watchdog_time++;
  if (http_watchdog_time > 5)
  {
    servo1.writeMicroseconds(minUs);
    digitalWrite(led_pin, !digitalRead(led_pin));
  }
  else{
    digitalWrite(led_pin, LOW);
  }
}

/* XMLHttpRequest Set initial value of slider */
String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return "0";
  }
  return String();
}

void setup(){
  /* Serial */
  Serial.begin(9600);
  
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  /* Timer Setup */
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100000, true);
  timerAlarmEnable(timer);

  /* Wifi access point Setup */
  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(ip, ip, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  /* Server Setup */
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