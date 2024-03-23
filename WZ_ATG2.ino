// WZ_ATG2.ino V0.1
// modified: https://github.com/karamo
// ================

/* Original
  Name:	       OTA_buttonQuery.ino
  Created:     29/03/2021
  Author:      Tolentino Cotesta <cotestatnt@yahoo.com>
  Description: an example that check for incoming messages and install update remotely.
*/
#include <AsyncTelegram2.h>

#include "credentials.h"
/*
const char* ssid = "*****";               // WIFI SSID
const char* pass = "*****";               // WIFI PASSWORD
const char* token = "*****:*****-*****";  // TELEGRAM BOT TOKEN
int64_t chat_id = -123456789;             // Telegram Group-ID
*/

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

#define PDW false
#define BTN true

// Timezone definition
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"
#include <time.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h>  
  #define UPDATER ESPhttpUpdate
  BearSSL::WiFiClientSecure SSLclient;
  BearSSL::Session   session;
  BearSSL::X509List  certificate(telegram_cert);
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
  #include <HTTPUpdate.h>  
  #define UPDATER httpUpdate
  #include <WiFiClientSecure.h>
  WiFiClientSecure SSLclient;
#endif


#define CANCEL  "CANCEL"
#define CONFIRM "FLASH_FW"

const char* firmware_version = __TIMESTAMP__;
const char* fw_password = "update";

String locMAC = WiFi.macAddress();                                     // WiFi xx:xx:xx:xx:xx:xx
String locMACx = locMAC.substring(12, 14) + locMAC.substring(15, 17);  // last 2 Bytes of MAC
String selectedNode;  // MAC or "XXXX"
bool firstrun = true;

TBMessage msg;  // a variable to store telegram message data

AsyncTelegram2 myBot(SSLclient);


//=============================================================================
// This function will be excuted once the message was sent
// if succesfull sent will be true, otherwise false
void messageSent(bool sent) {
  if (sent) { Serial.println("Last message was delivered"); }
  else { Serial.println("Last message was NOT delivered"); }
}


//=============================================================================
// Install firmware update
void handleUpdate(TBMessage msg, String file_path) {
  SSLclient.setInsecure();

  Serial.printf("Firmware path: %s\n", file_path.c_str());

  // On a good connection the LED should flash regularly. On a bad connection the LED will be
  // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
  // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
  UPDATER.setLedPin(LED_BUILTIN, LOW);

  UPDATER.onProgress([](int cur, int total) {
    static uint32_t sendT;
    if (millis() - sendT > 1000) {
      sendT = millis();
      Serial.printf("Updating %d of %d bytes...\n", cur, total);
    }
  });

  t_httpUpdate_return ret = UPDATER.update(SSLclient, file_path);
  Serial.println("Update done!");
  SSLclient.stop();

  String report;
  String text;

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      report = "HTTP_UPDATE_FAILED Error (";
      report += UPDATER.getLastError();
      report += "): ";
      report += UPDATER.getLastErrorString();
      myBot.sendMessage(msg, report.c_str());
      Serial.printf("%s\n", report.c_str());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      myBot.sendMessage(msg, "HTTP_UPDATE_NO_UPDATES");
      Serial.printf("HTTP_UPDATE_NO_UPDATES\n");
      break;

    case HTTP_UPDATE_OK:
      myBot.begin();
      text = locMACx + " UPDATE OK.\nRestarting in few seconds...";
      myBot.sendMessage(msg, text);
      // Wait until bot synced with telegram to prevent cyclic reboot
      // while (!myBot.noNewMessage()) { Serial.print("."); delay(50); }
      Serial.printf("%s HTTP_UPDATE_OK ... Restart\n", locMACx.c_str());
      WiFi.disconnect();
      ESP.restart();
      break;

    default:
      break;
  }
}

//=============================================================================
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.printf("\n\nMAC: %s V: %s\n", locMACx.c_str(), firmware_version);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  delay(500);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.printf("\nWiFi connected\n");

#ifdef ESP8266
  // Sync time with NTP, to check properly Telegram certificate
  configTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  // Set ceritficate, session and some other base SSLclient properies
  SSLclient.setSession(&session);
  SSLclient.setTrustAnchors(&certificate);
  SSLclient.setBufferSizes(1024, 1024);
#elif defined(ESP32)
  // Sync time with NTP
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  SSLclient.setCACert(telegram_cert);
#endif

  // Set the Telegram bot properies
  myBot.setUpdateTime(2000);
  myBot.setTelegramToken(token);

  // Add the callback function to bot
  myBot.addSentCallback(messageSent);

  // Check if all things are ok
  Serial.print("Test Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");

  // clear all messages
//  while (myBot.getNewMessage(msg)) { Serial.printf("OK: %s\n", msg.text.c_str()); }
  while (myBot.getNewMessage(msg)) { Serial.printf("."); }
  Serial.printf("\n");

  char welcome_msg[128];
  msg.messageType = MessageText;
  snprintf(welcome_msg, 128, "BOT @%s online", myBot.getBotName());
  myBot.sendTo(chat_id, welcome_msg);

  // We have to handle reboot manually after sync with TG server
  UPDATER.rebootOnUpdate(false);

  Serial.printf("end of setup\n");
  firstrun = true;
}


//=============================================================================
void loop() {

  // blink LED
  static uint32_t ledTime = millis();
  if (millis() - ledTime > 300) { ledTime = millis();
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  if (firstrun) {
    String fw = "MAC: " + String(locMACx) + " Version: " + String(firmware_version);
    myBot.sendTo(chat_id, fw);
    firstrun = false;
  }

  if (myBot.getNewMessage(msg)) {  // if there is an incoming message...
    String tgReply;
    static String document;
    String filename;

    Serial.printf("RecMsg from chatId: %lld\n", msg.chatId);

    switch (msg.messageType) {
      case MessageDocument :
        document = msg.document.file_path;
        filename = msg.document.file_name;
        selectedNode = msg.text;  // MAC or "XXXX"
        Serial.printf("File:>%s<\n", filename.c_str());
        Serial.printf("for MAC:>%s<\n", msg.text.c_str());
        if (msg.document.file_exists) {
          // Check file extension of received document (firmware MUST be .bin)
          if ((document.endsWith(".bin")) && ((selectedNode == locMACx) || (selectedNode == "XXXX"))) {
            Serial.printf(".bin file received ...\n");
            String report = "MAC: " + locMACx + "\nStart firmware update?\nFile name: " + msg.document.file_name
                            + "\nFile size: " + String(msg.document.file_size);
              // Query user for flash confirmation
              InlineKeyboard confirmKbd;
              confirmKbd.addButton("FLASH", CONFIRM, KeyboardButtonQuery);
              confirmKbd.addButton("CANCEL", CANCEL, KeyboardButtonQuery);
              myBot.sendMessage(msg, report.c_str(), confirmKbd);
          } else { Serial.printf("... not for me ;-)"); }
        } else { myBot.sendMessage(msg, "File is unavailable. Maybe size limit 20MB was reached or file deleted"); }
        break;

      case MessageQuery:
        tgReply = msg.callbackQueryData;  // received a callback query message
        if (tgReply.equalsIgnoreCase(CONFIRM) && ((selectedNode == locMACx) || (selectedNode == "XXXX"))) { // User has confirmed flash start
/*          if (PWD) {
            tgReply = msg.text;
            if (tgReply.equals(fw_password)) {}  // User has confirmed flash start with right password
          }
*/
          myBot.endQuery(msg, "Start flashing... please wait (~30/60s)", true);
          Serial.printf("Update confirmed ...\n");
          handleUpdate(msg, document);
          document.clear();
        } else if (tgReply.equalsIgnoreCase(CANCEL)) {  // User has canceled the command
          myBot.endQuery(msg, "Flashing canceled");
          Serial.printf("Update CANCELED\n");
          } // else { myBot.sendMessage(msg, "You have entered wrong password"); }
        break;

      default:
        tgReply = msg.text;
        if (tgReply.equalsIgnoreCase("/version")) {
          int i = strtol(locMACx.c_str(),NULL,16) / 65;
          Serial.printf("Query Version . %i\n", i);
          String fw = "MAC: " + String(locMACx) + " Version: " + String(firmware_version);
          delay(i);
          myBot.sendMessage(msg, fw);
        }
        else { myBot.sendMessage(msg, "Send firmware binary file ###.bin to start update\n"
                            "/version for print the current firmware version\n");
        }
        break;
    }
  }
}
