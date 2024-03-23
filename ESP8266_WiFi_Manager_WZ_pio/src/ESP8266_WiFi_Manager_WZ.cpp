/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp8266-nodemcu-wi-fi-manager-asyncwebserver/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include "LittleFS.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";
const char* PARAM_INPUT_5 = "mqttserver";
const char* PARAM_INPUT_6 = "mqttuser";
const char* PARAM_INPUT_7 = "mqttpwd";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;
String mqttserver;
String mqttuser;
String mqttpwd;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";
const char* mqttserverPath = "/mqttserver.txt";
const char* mqttuserPath = "/mqttuser.txt";
const char* mqttpwdPath = "/mqttpwd.txt";

IPAddress localIP;  // IPAddress localIP(192, 168, 1, 200); // hardcoded
IPAddress localGateway;  // IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

const int ledPin = 2;  // Set LED GPIO
String ledState;      // Stores LED state

boolean restart = false;

//=============================================================================
// Initialize LittleFS
void initFS() {
  if (!LittleFS.begin()) {Serial.println("\n\nAn error has occurred while mounting LittleFS");}
  else {Serial.println("\n\nLittleFS mounted successfully");}
}

//=============================================================================
// Read File from LittleFS
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }

  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;
  }
  file.close();
  return fileContent;
}

//=============================================================================
// Write file to LittleFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){ Serial.println("- file written"); }
  else { Serial.println("- file write failed"); }
  file.close();
}

//=============================================================================
// Initialize WiFi
bool initWiFi() {
  if((ssid.length() <=2) || (ip=="")){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);  // Set WiFi to station mode
  WiFi.disconnect();  // Disconnect from an AP if it was previously connected
  delay(100);
  
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());

  if (!WiFi.config(localIP, localGateway, subnet)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());

  Serial.printf("Connecting to WiFi %s %s\n", ssid.c_str(), pass.c_str());
  for (int i=0; i<20; i++) {
    delay(1000);
    Serial.print(".");
    if(WiFi.status() == WL_CONNECTED) {
      Serial.print("\nWiFi connected ");
      Serial.print(WiFi.localIP());
      Serial.printf(" RSSI %i dBm\n", WiFi.RSSI());
      break;
    }
  }
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect.");
  }
  return (WiFi.status() == WL_CONNECTED);
}

//=============================================================================
// Replaces placeholder with LED state value
String processor(const String& var) {
  if(var == "STATE") {
    if(!digitalRead(ledPin)) { ledState = "ON"; }
    else { ledState = "OFF"; }
    return ledState;
  }
  return String();
}

//=============================================================================
void StartConfig() {
  Serial.println("Setting AP (Access Point)");
  WiFi.softAP("ESP-WIFI-MANAGER", NULL);    // NULL sets an open Access Point
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP); 

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/wifimanager.html", "text/html");
  });
  Serial.println("request sent");

  server.serveStatic("/", LittleFS, "/");
  Serial.println("serveStatic done");

  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        // HTTP POST ssid value
        if (p->name() == PARAM_INPUT_1) {
          ssid = p->value().c_str();
          Serial.printf("SSID set to: %s\n", ssid.c_str());
          writeFile(LittleFS, ssidPath, ssid.c_str());  // write value into file
        }
        // HTTP POST pass value
        if (p->name() == PARAM_INPUT_2) {
          pass = p->value().c_str();
          Serial.printf("Password set to: %s\n", pass.c_str());
          writeFile(LittleFS, passPath, pass.c_str());  // write value into file
        }
        // HTTP POST ip value
        if (p->name() == PARAM_INPUT_3) {
          ip = p->value().c_str();
          Serial.printf("IP Address set to: %s\n", ip.c_str());
          writeFile(LittleFS, ipPath, ip.c_str());  // write value into file
        }
        // HTTP POST gateway value
        if (p->name() == PARAM_INPUT_4) {
          gateway = p->value().c_str();
          Serial.printf("Gateway set to: %s\n", gateway.c_str());
          writeFile(LittleFS, gatewayPath, gateway.c_str());  // write value into file
        }

        // die weiteren MQTT Parameter 5 6 7 8
        // ...

        //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
      }
    }
    restart = true;
    request->send(200, "text/plain", "Done. ESP will restart, connect to your router and go to IP address: " + ip);
  });
  Serial.println("server begin");
  server.begin();
}

//============================================================================
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  initFS();

  // Set GPIO 2 as an OUTPUT
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Load values saved in LittleFS
  ssid = readFile(LittleFS, ssidPath);
  Serial.printf("SSID %s\n", ssid.c_str());
  pass = readFile(LittleFS, passPath);
  Serial.printf("PWD %s\n", pass.c_str());
  ip = readFile(LittleFS, ipPath);
  Serial.printf("IP %s\n", ip.c_str());
  gateway = readFile (LittleFS, gatewayPath);
  Serial.printf("GW %s\n", gateway.c_str());

  if(initWiFi()) {  // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });
    
    server.serveStatic("/", LittleFS, "/");
    
    // Route to set GPIO state to HIGH
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.printf("LED set ON");
      digitalWrite(ledPin, LOW);
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    // Route to set GPIO state to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
      Serial.printf("LED set OFF");
      digitalWrite(ledPin, HIGH);
      request->send(LittleFS, "/index.html", "text/html", false, processor);
    });

    server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
      restart = true;
      LittleFS.remove(ssidPath);
      WiFi.disconnect();
      Serial.printf("%s deleted ... restart\n", ssidPath);
      return;
    });

    Serial.println("server begin");
    server.begin();
  } else { StartConfig(); } // Connect to Wi-Fi network with SSID and password
}


//============================================================================
void loop() {
  if (restart){
    Serial.println("Restart in 5s...");
    delay(5000);
    ESP.restart();
  }
}
