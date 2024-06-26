Die bei der Entwicklung einer **transparenten Serial-MQTT-Bridge** (full ASCII) getesteten und verwendeten **HW-Module** und **SW-Detaillösungen** sind hier (noch unvollständig) dargestellt.

# Self-OTA-Update
• Minimal-Versionen als Beispiele für **Self-OTA-Update** von verschiedener Art von Web-Servern.  
• einzelne **.bin** zum Testen.

## 1) Github https
Beispielprogramm: ![ESP8266_Self_OTA_Update_minimal.ino](ESP8266_Self_OTA_Update_minimal.ino)

• Github Repository MUSS **public** sein !!!  
• genereller Aufbau des Links zu einer Datei

https://raw.githubusercontent.com/<Github_username>/<repository_name>/<branch_name>/<file_name>

• Beispiel für dieses Repository  
https://raw.githubusercontent.com/karamo/firmware/master/README.md  

• Serial-Befehl für meine **Bridge-FW**  
@@OTAupdate https://raw.githubusercontent.com/karamo/firmware/master/WZ_ESP-01S_SMQBridge_v0_8_19.bin

## 2) RPi Webserver http
Beispielprogramm: ![ESP8266_Self_OTA_Update_minimal.ino](ESP8266_Self_OTA_Update_minimal.ino)

• lokaler Web-Server auf einem **Raspberry Pi Zero 2W** mit **Node.js** und **Express**  
Erstellung siehe [OTAserver](https://github.com/karamo/firmware/tree/main/OTAserver)  

exemplarische Daten:  

<ins>• WinSCP:</ins>  
karamo@rpiz2w.local :22  
karamo - rpiz2w  
IP: 192.168.1.171

• Der Web-Server läuft in: /home/karamo/OTAserver/  
• Die Datei befindet sich in: /home/karamo/OTAserver/firmware/

**rpiz2w.local** geht nicht, es muss die IP verwendet werden!

• Serial-Befehle für meine **Bridge-FW**  
@@@@@@@@@@ASCII  
@echo on  
@@OTAupdate http://192.168.1.171:3000/firmware/WZ_ESP-01S_SMQBridge_v0_8_19.bin 

## 3) Telegram Group
Beispielprogramm: ![WZ_ATG2.ino](WZ_ATG2.ino)

• Telegram Bot erstellen > Man erhält ein **token**  
• Telegram Gruppe erstellen und **Group-ID** herausfinden  
• **credentials.h** muss erstellt werden (s. Muster in .ino)

v 0.1: läuft u.U. noch nicht selektiv in einer Gruppe, falls der Bot in mehreren Gruppen aktiv ist.

# ESP-WiFi-Manager & ESP-Web-Server
Varianten von WiFi-Managern und Web-Servern auf einem ESP für WiFi- und MQTT-Konfiguration u/o Steuer- u/o Meldeaufgaben.



________________________________________________
• Syntax für die Formatierung von .md Dateien:  
https://docs.github.com/de/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax  
https://docs.github.com/de/get-started/writing-on-github/working-with-advanced-formatting  
https://hellocoding.de/blog/coding-language/allgemein/markdown-guide  
https://github.github.com/gfm/#images  

