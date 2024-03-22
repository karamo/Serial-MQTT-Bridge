# Self-OTA-Update
Beispiele für Self-OTA-Update von verschiedener Art von Web-Servern.

## 1) Github https
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
• lokaler Web-Server auf einem Raspberry Pi Zero 2W mit Node.js und Express

<ul>WinSCP:</ul>  
karamo@rpiz2w.local :22  
karamo - rpiz2w 

Der Web-Server läuft in: /home/karamo/OTAserver/  
Die Datei befindet sich in: /home/karamo/OTAserver/firmware/

**rpiz2w.local** geht nicht, es muss die IP verwendet werden: 192.168.1.171

• Serial-Befehle für meine **Bridge-FW**  
@@@@@@@@@@ASCII  
@echo on  
@@OTAupdate http://192.168.1.171:3000/firmware/01_ESP-01S_Serial-MQTT-Bridge_v0.8.12http_s01.bin 


________________________________________________
• Syntax für die Formatierung:  
https://docs.github.com/de/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax