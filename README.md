# wattcycle-bms-monitor

Mit diesem Arduino-Projekt ist es möglich das BMS einer LiFePo4 WattCycle 48V 100A Rack-Batterie per RS485 oder Bluetooth auszulesen, und die Werte per MQTT an iobroker zu senden. Das Projekt umfasst auch ein Webinterface in dem die Daten visuell dargestellt werden. 

> [!NOTE]
> Das Projekt ist noch am Beginn der Entwicklung! Bluetooth ist im aktuellen *.ino noch nicht enthalten  

> [!WARNING]
> Ich übernehme keinerlei Haftung für Schäden an Personen oder Hardware die durch dieses Projekt entstehen. Arbeiten an Spannungen größer 24V sollten nur von Fachpersonal durchgeführt werden!  
 

# Bluetooth
Aktuell arbeite ich an der Implementierung über Bluetooth, erste Erfolge habe ich schon.


# Firmware installieren
Damit nicht jeder die Arduino-Ide installieren muss, kann die Firmware hier direkt mit einem Web-Flasher geschrieben werden

[https://matlen67.github.io/wattcycle-bms-monitor/esp32-webflasher](https://matlen67.github.io/wattcycle-bms-monitor/esp32-webflasher/)






## Schaltung
### Bauteile
- ESP32-S3 DevKitC-1
- RS485 Entwicklungsboard TTL zu RS485, MAX485
- RJ45 Stecker mit Kabel. RJ45 Pin 1 (white/orange) to RS485 B / RJ45 Pin 2 (orange) to RS485 A

Hinweis: Das RS485 Entwicklungsboard verwendet einen MAX485 Pegelwandler der für eine Versorgungsspannung von 5V ausgelegt ist. Da die GPIO's des ESP32 dauerhaft nur 3.3V vertragen wird die Spannung Vcc vom RS485 Entwicklungsboard am 3.3V Ausgang des ESP32 abgegriffen. Das RS485 Etwicklungsboard arbeitet auch zuverlässig mit 3.3V. Die 5V Spannungsversorgung des ESP32 kann entweder über USB oder den Anschlus-Pin VIN erfolgen.


### Bild 1: Schaltung
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/schaltplan.png" width="512">

### Bild 2: mqtt iobroker
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/mqtt_iobroker.png" width="512">

### Bild 3: webif
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/webif1_20261022.png" width="512">
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/webif_konfig_20261024.png" width="512">

### Bild 4: Telnet Log RS485
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnet_log.png" width="512">

### Bild 5: Telnet Log Bluetooth
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnetlog_bluetooth.png" width="512">


