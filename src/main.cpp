#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>

WebServer server(80);

const char ssid[] = "House";
const char password[] = "africaSul456";
const char hostname[] = "Esp32_pierre";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
  });

  server.begin();
  Serial.println("HTTP server started");

  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}