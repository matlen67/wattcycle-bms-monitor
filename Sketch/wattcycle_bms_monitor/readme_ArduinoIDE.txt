ESP32-S3 16R8 Einstellungen Arduino

Flash Mode:         QIO 80Mhz
Flash Size:         16MB (128Mb)

Partitionscheme:    custom  (wegen OTA update)
PSARAM:             OPI PSARAM


Datei in Sketch ordner anlegen: partitions.csv

# Name,   Type, SubType, Offset,  Size, Flags
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x480000,
app1,     app,  ota_1,   0x490000,0x480000,
spiffs,   data, spiffs,  0x910000,0x6E0000,
coredump, data, coredump,0xFF0000,0x10000,



