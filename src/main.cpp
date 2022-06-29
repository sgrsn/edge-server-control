#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "index_html.h"

//const char* ssid = "iPhone2";
//const char* password = "mbedonline";

const char *ssid = "ESP32ap";
const char *password = "12345678";
const IPAddress ip(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0); 

const int led_pin = 14;
String slider_value = "0";

const int frequency = 5000;
const int led_channel = 0;
const int resolution = 8;

const char* input_parameter = "value";

AsyncWebServer server(80);

//const char index_html[] PROGMEM = R"rawliteral(
//const char index_html[]= INDEX_HTML;
//)rawliteral";
const char* index_html = INDEX_HTML; // index_html.hより読み込み

String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return slider_value;
  }
  return String();
}

void setup(){
  Serial.begin(9600);

  ledcSetup(led_channel, frequency, resolution);
  ledcAttachPin(led_pin, led_channel);
  ledcWrite(led_channel, slider_value.toInt());

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
      slider_value = message;
      ledcWrite(led_channel, slider_value.toInt());
    }
    else {
      message = "No message sent";
    }
    Serial.println(message);
    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
}
  
void loop() {
  
}