#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <ArduinoJson.h>
#include <AsyncJson.h>

AsyncWebServer server(80);

const char ssid[] = "House";
const char password[] = "africaSul456";
const char hostname[] = "Esp32_pierre";

#define SERNO_PIN 27
#define LED_PIN 2

Servo myservo;

const char *homepage PROGMEM = {
#include "index.html"
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  myservo.attach(SERNO_PIN);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(hostname)) {
    Serial.println("MDNS responder started:");
    Serial.println(hostname);
  }

  server.on("/", [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", homepage);
  });

  server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data,
                          size_t len, size_t index, size_t total) {
    if (request->url() == "/servo") {
      StaticJsonDocument<256> buff;
      auto error = deserializeJson(buff, (const char *)data);
      if (error)
        request->send(402, "text/plain", "error");
      else if (buff.containsKey("ang")) {
        int ang = buff["ang"].as<int>();
        myservo.write(ang);
        request->send(200, "text/plain", "end");
      }
      request->send(412, "text/plain", "end");
    }
  });

  // server.on("/servo", HTTP_POST, [](AsyncWebServerRequest *request) {
  //   if (request->) {
  //     auto param = request->getParam("ang");
  //     int ang = param->value().toInt();
  //     if (ang > 180)
  //       ang = 180;
  //     else if (ang < 0)
  //       ang = 0;
  //     myservo.write(ang);
  //     request->send(200, "text/plain", "Servo Acionado: " + param->value());
  //   } else {
  //     request->send(412, "falta o angulo");
  //   }
  // });

  server.on("/led", [](AsyncWebServerRequest *request) {
    bool out = !digitalRead(LED_PIN);
    digitalWrite(LED_PIN, out);
    request->send(200, "text/plain", String(out));
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  // server.handleClient();
}