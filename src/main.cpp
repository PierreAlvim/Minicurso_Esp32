#include <Arduino.h>
#include <ESP32Servo.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <uri/UriBraces.h>

WebServer server(80);

const char ssid[] = "House";
const char password[] = "africaSul456";
const char hostname[] = "Esp32_pierre";

#define SERNO_PIN 27
#define LED_PIN 2

Servo myservo;

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

  server.on("/", []() {
    server.send(200, "text/plain", "Hello, world!");
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  });

  server.on(UriBraces("/servo/{}"), []() {
    server.send(200, "text/plain", "Servo Acionado");
    String param = server.pathArg(0);
    int ang = param.toInt();
    if (ang > 180)
      ang = 180;
    else if (ang < 0)
      ang = 0;
  });

  server.begin();
  Serial.println("HTTP server started");

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}