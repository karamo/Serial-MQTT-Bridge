# Web-Sever mit Node.js & Express

Von verschiedenen Quellen (nicht mehr nachvollziehbar) die einzelnen Elemente zu einem letztendlich funktionierenen Web-Server zusammengestellt.

Hardware: **Raspberry Pi Zero 2W**

Installationsablauf Kurzfassung:  
```
sudo apt update  
sudo apt install nodejs  
sudo apt install npm  
mkdir OTAserver
cd OTAserver
npm init
sudo npm install express
```

**index.js** = Web-Server Code in das erstellte Verzeichnis ```OTAserver``` kopieren.

• Autostart von ```~/OTAserver/index.js``` eintragen.  
Im Verzeichnis von ```index.js``` befindend den kompletten Pfad ermitteln:  
• ```which node``` liefert ```/usr/bin/node```  
• ```pwd``` liefert z.B. ```/home/karamo/OTAserver```  

```sudo crontab -e```  
• am Ende der Datei folgende Zeile eintragen:  
```@reboot sudo /usr/bin/node /home/karamo/OTAserver/index.js &```  

```sudo reboot``` nicht vergessen.
