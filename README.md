# wattcycle-bms-monitor

Mit diesem Arduino-Projekt ist es möglich das BMS einer LiFePo4 WattCycle 48V 100A Rack-Batterie per RS485 oder Bluetooth auszulesen, und die Werte per MQTT an iobroker zu senden. Das Projekt umfasst auch ein Webinterface in dem die Daten visuell dargestellt werden.

> [!NOTE]
> Das Projekt ist noch am Beginn der Entwicklung! Bluetooth ist im aktuellen *.ino noch nicht enthalten  

> [!WARNING]
> Ich übernehme keinerlei Haftung für Schäden an Personen oder Hardware die durch dieses Projekt entstehen. Arbeiten an Spannungen größer 24V sollten nur von Fachpersonal durchgeführt werden!  
 

# Bluetooth
Aktuell arbeite ich an der Implementierung über Bluetooth, erste Erfolge habe ich schon.
```
[WATT] Heartbeat Link=1 | GATT=1 | RX=7 | Drops=0 | Heap=131308
[WATT LOOP] Init=1 | Link=1 | GATT=1 | Worker=1 | RX=7 | Heap=131520 | Uptime=136 s
[WATT TX] DP140 FFF2:7E 00 01 03 00 8C 00 00 99 42 0D
[WATT TX] DP140 writeValue=OK | Link=1
[WATT RX] Notify | 71 Bytes | Notification #8
[WATT RX HEX] 7E 04 01 03 00 8C 00 3C 10 0C E8 0C E8 0C E6 0C E6 0C E7 0C E8 0C E8 0C E8 0C E8 0C E6 0C E6 0C
[WATT RX HEX] E7 0C E7 0C E8 0C E7 0C E8 06 0B 22 0B 3C 0B 0C 0B 11 0B 0C 0B 0B 40 00 14 A5 02 A4 03 E8 00 0B
[WATT RX HEX] 03 E8 00 44 AA F8 0D
[WATT DEC] DP140: 16 Zellen
[WATT CELL] Z01 = 3304 mV (3.304 V)
[WATT CELL] Z02 = 3304 mV (3.304 V)
[WATT CELL] Z03 = 3302 mV (3.302 V)
[WATT CELL] Z04 = 3302 mV (3.302 V)
[WATT CELL] Z05 = 3303 mV (3.303 V)
[WATT CELL] Z06 = 3304 mV (3.304 V)
[WATT CELL] Z07 = 3304 mV (3.304 V)
[WATT CELL] Z08 = 3304 mV (3.304 V)
[WATT CELL] Z09 = 3304 mV (3.304 V)
[WATT CELL] Z10 = 3302 mV (3.302 V)
[WATT CELL] Z11 = 3302 mV (3.302 V)
[WATT CELL] Z12 = 3303 mV (3.303 V)
[WATT CELL] Z13 = 3303 mV (3.303 V)
[WATT CELL] Z14 = 3304 mV (3.304 V)
[WATT CELL] Z15 = 3303 mV (3.303 V)
[WATT CELL] Z16 = 3304 mV (3.304 V)
[WATT CELL] Summe=52.852 V | Min=3302 mV | Max=3304 mV | Delta=2 mV
[WATT DEC] 6 Temperaturfuehler
[WATT TEMP] T1 = 11.85 C (raw=0x0B22)
[WATT TEMP] T2 = 14.45 C (raw=0x0B3C)
[WATT TEMP] T3 = 9.65 C (raw=0x0B0C)
[WATT TEMP] T4 = 10.15 C (raw=0x0B11)
[WATT TEMP] T5 = 9.65 C (raw=0x0B0C)
[WATT TEMP] T6 = 9.55 C (raw=0x0B0B)
[WATT BMS] Gesamtspannung = 52.85 V
[WATT BMS] Strom = 0.0 A
[WATT BMS] SOC = 68 %
[WATT BMS] Restkapazitaet = 67.6 Ah | Gesamtkapazitaet = 100.0 Ah
[WATT BMS] Ladezyklen = 11 | Designkapazitaet = 100.0 Ah
```






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
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/webif2_20261022.png" width="512">

### Bild 4: Telnet Log RS485
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnet_log.png" width="512">

### Bild 5: Telnet Log Bluetooth
<img src="https://github.com/matlen67/wattcycle-bms-parser/blob/main/image/telnetlog_bluetooth.png" width="512">


