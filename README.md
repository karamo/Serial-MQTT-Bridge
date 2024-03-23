# Self-OTA-Update
• Minimal-Versionen als Beispiele für **Self-OTA-Update** von verschiedener Art von Web-Servern.  
• einzelne **.bin** zum Testen.

## 1) Github https
Beispielprogramm: ![ESP8266_Self_OTA_Update_minimal.ino](ESP8266_Self_OTA_Update_minimal.ino)

• Github Repository MUSS **public** sein !!!  
• genereller Aufbau des Links zu einer Datei

https://raw.githubusercontent.com/<Github_username>/<repository_name>/<branch_name>/<file_name>

• Beispiel für dieses Repository  
https://www.github.com/karamo/firmware  
https://raw.githubusercontent.com/karamo/firmware/master/README.md  
https://raw.githubusercontent.com/karamo/firmware/master/fw_x.y.z.bin

• Serial-Befehl für meine **Bridge-FW**  
@@OTAupdate https://raw.githubusercontent.com/karamo/firmware/master/01_ESP-01S_Serial-MQTT-Bridge_v0.8.12http_s01.bin

## 2) RPi Webserver
• lokaler Web-Server auf einem **Raspberry Pi Zero 2W** mit **Node.js** und **Express**

exemplarische Daten:  

<ins>∆ WinSCP:</ins>  
karamo@rpiz2w.local :22  
karamo - rpiz2w  
IP: 192.168.1.171

∆ Der Web-Server läuft in: /home/karamo/OTAserver/  
∆ Die Datei befindet sich in: /home/karamo/OTAserver/firmware/

**rpiz2w.local** geht nicht, es muss die IP verwendet werden!

• Serial-Befehle für meine **Bridge-FW**  
@@@@@@@@@@ASCII  
@echo on  
@@OTAupdate http://192.168.1.171:3000/firmware/01_ESP-01S_Serial-MQTT-Bridge_v0.8.12http_s01.bin 

## 3) Telegram Group
Beispielprogramm: ![WZ_ATG2_xxx.ino](WZ_ATG2_xxx.ino)

• Telegram Bot erstellen. Man erhält ein **token**  
• Telegram Gruppe erstellen und **Group-ID** herausfinden  

v 0.1: läuft u.U. noch nicht selektiv in einer Gruppe, falls der Bot in mehreren Gruppen aktiv ist.

________________________________________________
• Syntax für die Formatierung von .md Dateien:  
https://docs.github.com/de/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax  
https://docs.github.com/de/get-started/writing-on-github/working-with-advanced-formatting  
https://hellocoding.de/blog/coding-language/allgemein/markdown-guide  
https://github.github.com/gfm/#images  

