# wattcycle-bms-monitor

Mit diesem Arduino-Projekt ist es möglich das BMS einer LiFePo4 WattCycle 48V 100A Rack-Batterie per RS485 oder Bluetooth auszulesen, und die Werte per MQTT an z.B. iobroker zu senden. Das Projekt umfasst ein Webinterface in dem die Daten visuell dargestellt werden, zudem kann auch durch einen Bluetooth-Scan die Batterie gesucht, und dessen MAC übernommen werden. 


> [!WARNING]
> Ich übernehme keinerlei Haftung für Schäden an Personen oder Hardware die durch dieses Projekt entstehen!
 

## Firmware installieren
Damit die Arduino-Ide nicht installieren werden muss, kann die Firmware zur Erstinstallation direkt mit einem Web-Flasher geschrieben werden. Nach der Erstinstallation können Updates per OTA eingespielt werden.
Wichtig! Es werden alle Daten inklusive Wlanverbindungsdaten aus dem Speicher des ESP32 gelöscht. Nachdem der ESP neu gestartet ist, wird ein Accesspoint mit mit der Kennung  ESP32_000000 aufgemacht. Hier kann man sich mit dem Smartphone verbinden um die Wlan- und MQTT-Broker-Verbindungsdaten einzutragen. Nach dem Speichern muss der ESP neu gestartet werden.
<br>
<br>
[https://matlen67.github.io/wattcycle-bms-monitor/esp32-webflasher](https://matlen67.github.io/wattcycle-bms-monitor/esp32-webflasher/)
<br>
<br>
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/wlanconfig.png" width="256">
<br>
<br>







## Schaltung
### Bauteile
- ESP32-S3 DevKitC-1
- RS485 Entwicklungsboard TTL zu RS485, MAX485 
- RJ45 Stecker mit Kabel. RJ45 Pin 1 (white/orange) to RS485 B / RJ45 Pin 2 (orange) to RS485 A
  
Das RS485 Entwicklungsboard und das Kabel mit RJ45-Stecker wird nur benötigt, wenn die Verbindung per RS485 aufgebaut werden soll
<br>
<br>
Hinweis: Das RS485 Entwicklungsboard verwendet einen MAX485 Pegelwandler der für eine Versorgungsspannung von 5V ausgelegt ist. Da die GPIO's des ESP32 dauerhaft nur 3.3V vertragen wird die Spannung Vcc vom RS485 Entwicklungsboard am 3.3V Ausgang des ESP32 abgegriffen. Das RS485 Etwicklungsboard arbeitet auch zuverlässig mit 3.3V. Die 5V Spannungsversorgung des ESP32 kann entweder über USB oder den Anschlus-Pin VIN erfolgen.


### Bild 1: Schaltung
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/schaltplan.png" width="512">

### Bild 2: mqtt iobroker
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/mqtt_iobroker.png" width="512">

### Bild 3: webif
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/webif1_20261024.png" width="512">
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/webif_konfig_20261024.png" width="512">

### Bild 4: Telnet Log RS485
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnet_log.png" width="512">

### Bild 5: Telnet Log Bluetooth
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnetlog_bluetooth.png" width="512">


