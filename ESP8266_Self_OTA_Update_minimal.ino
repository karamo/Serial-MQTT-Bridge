// Minimal-Demo Self-OTA-Update from Github

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
// HTTPS
#include <WiFiClientSecureBearSSL.h>
BearSSL::WiFiClientSecure SSLclient;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  WiFi.begin("SSID", "PWD");
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
  Serial.println("Connected, updating");
  delay(1000);

  ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
  SSLclient.setInsecure();  // Ignore SSL certificate validation

  t_httpUpdate_return ret = ESPhttpUpdate.update(SSLclient, "https://raw.githubusercontent.com/karamo/firmware/master/01_ESP-01S_Serial-MQTT-Bridge_v0.8.12http_s01.bin");

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.printf("HTTP_UPDATE_NO_UPDATES\n");
      break;

    case HTTP_UPDATE_OK:
      Serial.printf("HTTP_UPDATE_OK  - restart in 5s\n");
      delay(5000); // Wait a second and restart
      ESP.restart();
      break;
  }
}

void loop() {
}