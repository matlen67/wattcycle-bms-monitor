/***************************************************************************
  wattcycle-bms-monitor @matlen67
  Version: 1.0

  ----------------------------
  Wiring: ESP32 - RS485 Modul

  RX    GPIO 14 - RS485 RO
  TX    GPIO 13 - RS485 DI
  DE_RE GPIO  1 - RS485 DE/RE
  -----------------------------
  Wiring: RS485 - RJ45

  A - Pin 2 (Orange)
  B - Pin 1 (White/Orange)
  -----------------------------

  Requestframe WattCycle BMS RS485-Mode(PY/GosPow1) = "~20024642E00202FD33\r"

  Achtung! Dieses Projekt verwendet die Lib ESPAsyncWebServer. Hier ist als Delimeter Trennzeichen das % Zeichen definiert.
            Das macht Probleme beim darstellen der Webseite. Hierzu bitte das % Zeichen in der Datei
            ArduinoIDE\libraries\ESP_Async_WebServer\src\WebResponseImpl.h ändern.
            org:  #define TEMPLATE_PLACEHOLDER '%'
            neu:  #define TEMPLATE_PLACEHOLDER '~'
           
  
****************************************************************************/

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>      
#include <ArduinoJson.h>

#include <Adafruit_NeoPixel.h>
#include "nvs_flash.h" // nvs_flash_init(), ...
#include "html.h"

#include <TelnetSpy.h>
TelnetSpy SerialAndTelnet;
#define SERIAL_TELNET SerialAndTelnet


#define RGB_LED_PIN 48
#define RGB_LED_COUNT  1
Adafruit_NeoPixel rgbled(RGB_LED_COUNT, RGB_LED_PIN, NEO_GRB + NEO_KHZ800);


WiFiClient espClient;
PubSubClient client(espClient);

AsyncWebServer server(80);
AsyncEventSource events("/events");

// wlan
const char *ssid = "Hier deine SSID";
const char *password = "Hier dein Wlanpass";

// mqtt
char mqtt_server[16] = "192.168.178.10";
char mqtt_port[5] = "1889";

const char* topic_root = "wattcycle/battery";
char buffer_topic[128];
char buffer_payload[8];

// Pylontech Checksum
#define MAX_FRAME_LENGTH 256
char rxBuffer[MAX_FRAME_LENGTH];
int rxIndex = 0;
bool recording = false;

//RS485 control
#define BMS_SERIAL Serial2
#define rxPin    14  
#define txPin    13  
#define DE_RE_PIN   1
#define RS485_TRANSMITT  HIGH
#define RS485_RECIVE    LOW

// Requestframe 
char *REQUEST = "~20024642E00202FD33\r";


//#define DEBUG_SERIAL Serial       // Wenn der ESP 32 am PC per USB angeschlossen ist
#define DEBUG_SERIAL serverClient   // Serial Log über Telnet

// unmark for Serial Log
#define DEBUG

#ifdef DEBUG
  #define DBG_PRINT(x) DEBUG_SERIAL.print(x)
  #define DBG_PRINTLN(x) DEBUG_SERIAL.println(x)
#else
  #define DBG_PRINT(x)
  #define DBG_PRINTLN(x)
#endif

#define FORMAT_LITTLEFS_IF_FAILED true

char dbgbuffer[128]; 
//**********************************************************************************************************
// Serial.printf workaround:
// Da Serial.printf("Zahl in Hex: %02X\n", intNr); mit define nicht funktioniert!
// sprintf(dbgbuffer,""Zahl in Hex: %02X\n", intNr);  // formatierten string nach dbgbuffer kopieren
// DBG_PRINT(dbgbuffer);                              // und dann ausgeben
//**********************************************************************************************************


// timer
unsigned long startTime = millis();
bool new_connect = true;
const char* PARAM_MESSAGE = "message";
bool checkbox_mqttenabled = true;
char mqtt_state[20] = "disabled";
unsigned long ota_progress_millis = 0;
unsigned char mac[8] = {'0','0','0','0','0','0','\0'};
char clientId[16] = {'E','S','P','3','2','_','0','0','0','0','0','0','\0'};

float  bat_voltages = 0.0;
float  bat_current = 0.0;
float  bat_leistung = 0.0;
float  bat_temp_cells = 0.0;
float  bat_temp_mosfet = 0.0;

uint32_t  bat_remain_ah = 0;
uint16_t  bat_soc = 0;
uint16_t  bat_totalcapacity = 0; 
uint16_t  bat_cycles = 0;
uint16_t  bat_maxcellvoltage = 0;
uint16_t  bat_mincellvoltage = 0;  
uint16_t  bat_celldelta = 0;   
uint16_t  bat_cellmax = 0;     
uint16_t  bat_cellmin = 0; 
uint16_t  bat_cellvoltages[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  

// Wifi to percent
const int RSSI_MAX =-50;  
const int RSSI_MIN =-100; 
long rssi;

//flag for saving data
bool shouldSaveConfig = false;


// Telnet
void telnetConnected() {
  SERIAL_TELNET.println(F("Telnet connection established."));
}


void telnetDisconnected() {
  SERIAL_TELNET.println(F("Telnet connection closed."));
}


void disconnectClientWrapper() {
  SerialAndTelnet.disconnectClient();
}

// declare telnet server (do NOT put in setup())
WiFiServer telnetServer(23);
WiFiClient serverClient;


void saveConfigCallback () {
  DBG_PRINTLN("Should save config");
  shouldSaveConfig = true;
}


void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}


String processor(const String& var){ 
  return String();
}


int dBmtoPercent(int dBm){
  int percent;
  if(dBm <= RSSI_MIN){
    percent = 0;
  } else if(dBm >= RSSI_MAX) {  
    percent = 100;
  } else {
    percent = 2 * (dBm + 100);
  }

  return percent;
} 


void onOTAStart() {
  // Log when OTA has started
  DBG_PRINTLN("OTA update started!");
  // <Add your own code here>
}


void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();

    sprintf(dbgbuffer,"OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
    DBG_PRINT(dbgbuffer);
  }
}


void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    DBG_PRINTLN("OTA update finished successfully!");
  } else {
    DBG_PRINTLN("There was an error during OTA update!");
  }
  // <Add your own code here>
}


//read configuration from config.json
void readConfig(){
  DBG_PRINTLN(F("mounting FS..."));

  if (LittleFS.begin()) {
    DBG_PRINTLN(F("mounted file system"));

    if (LittleFS.exists("/config.json")) {
      //file exists, reading and loading
      DBG_PRINTLN("open config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        DBG_PRINTLN("read config file");
        size_t size = configFile.size();

        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

        JsonDocument jdoc;
        DeserializationError error = deserializeJson(jdoc, buf.get());

        serializeJson(jdoc, Serial);
        DBG_PRINTLN();

        configFile.close();

        if (!error) {

          strcpy(mqtt_server, jdoc["mqtt_server"]);
          strcpy(mqtt_port, jdoc["mqtt_port"]);

          char key_value[2];

          if(jdoc.containsKey("mqtt_on")){
            strcpy(key_value, jdoc["mqtt_on"]);
            if(strcmp(key_value, "1") == 0){
              checkbox_mqttenabled = true;
            }else{
              checkbox_mqttenabled = false;
            }
          }
        } else {
          DBG_PRINTLN(F("failed to load json config"));
        }
      }
    }
  } else {
    DBG_PRINTLN(F("failed to mount FS"));
  }
} 


// save data to config.json
void saveConfig(){
  DBG_PRINTLN(F("save data to config.json"));
  JsonDocument jdoc;
 
  jdoc["mqtt_server"] = mqtt_server;
  jdoc["mqtt_port"] = mqtt_port;

  if(checkbox_mqttenabled){
    jdoc["mqtt_on"] = "1";
  }else{
    jdoc["mqtt_on"] = "0";
  }

  File configFile = LittleFS.open("/config.json", "w");
  if (!configFile) {
    DBG_PRINTLN(F("failed to open config file for writing"));
    return;
  }


  if(serializeJson(jdoc, configFile) == 0) {
    DBG_PRINTLN(F("Failed to write to file"));
  }else{
    serializeJson(jdoc, DEBUG_SERIAL); // print to serial console
    DBG_PRINTLN();
  }; 

  configFile.close();
}


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        DBG_PRINTLN("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        DBG_PRINTLN(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            DBG_PRINT("  DIR : ");
            DBG_PRINTLN(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            DBG_PRINT("  FILE: ");
            DBG_PRINT(file.name());
            DBG_PRINT("\tSIZE: ");
            DBG_PRINTLN(file.size());
        }
        file = root.openNextFile();
    }
}


// wlan status
const char *str_status[]= {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};


// WiFi mode
const char *str_mode[]= { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };

// WiFi
void connectWifi() {
  DBG_PRINT("Connecting as wifi client to SSID: ");
  DBG_PRINTLN(ssid);

  WiFi.disconnect();
 
  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
  }

  WiFi.begin (ssid, password );

  //DBG_PRINTLN(WiFi.printDiag());

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    DBG_PRINT(".");
  }
  DBG_PRINTLN("");
  
  // Check connection
  if (WiFi.status() == WL_CONNECTED) {
    DBG_PRINT("WiFi connected; IP address: ");
    DBG_PRINTLN(WiFi.localIP());
  } else {
    DBG_PRINT("WiFi connect failed to ssid: ");
    DBG_PRINTLN(ssid);
    DBG_PRINT("WiFi password <");
    DBG_PRINT(password);
    DBG_PRINTLN(">");
    DBG_PRINTLN("Check for wrong typing!");
  }
} 


void signalError() {  // RGBLED blinking in case of error
  while(1) {
      rgbled.setPixelColor(0, rgbled.Color(255, 0, 0)); // Red
      rgbled.show();
      delay(500); // ms
      
      rgbled.setPixelColor(0, rgbled.Color(0, 0, 0)); // Off
      rgbled.show();
      delay(500); // ms
  }
}


void sendBmsRequest(char *msg) {

    digitalWrite(DE_RE_PIN, RS485_TRANSMITT);
    delayMicroseconds(100);
    BMS_SERIAL.write(msg);
    BMS_SERIAL.flush();
    delayMicroseconds(100);
    digitalWrite(DE_RE_PIN, RS485_RECIVE);

    DBG_PRINTLN("Send bms request");
}


// Beispiel frame (Checksumme: E25F)
// ~20024600B07E1102100D080D080D060D070D080D070D070D080D090D080D070D080D080D070D0A0D07050B650B380B380B380B3703C0D07AE34E04FFFF000900E34E0186A0E25F
// Berechnung der Pylontech-Checksumme
bool calculatePylontechChecksum(const char* data) {
    uint32_t sum = 0;
    size_t length = strlen(data);
    char chksum_hexframe[5];
    
    // Checksumme aus gesendeten frame extrahieren
    memcpy(chksum_hexframe, data + (length - 5), 4);
    chksum_hexframe[4] = '\0';    
    uint16_t chksum_frame = strtol(chksum_hexframe, NULL, 16);
   
    // Checksumme neu berechnen (Ohne Startzeichen und der bereits enthaltene Checksumme)
    for (size_t i = 1; i < length - 5; i++) {
        sum += (uint8_t)data[i];
    }
    uint16_t chksum_new = (~sum + 1) & 0xFFFF;

    if(chksum_frame == chksum_new){
      DBG_PRINTLN("Checksumme stimmt überein");
      return true;
    } else{
      DBG_PRINTLN("Fehler Checksumme!");
      return false;
    }
}


uint32_t parseHex(const char* str, int pos, int length) {
  uint32_t val = 0;
  for (int i = 0; i < length; i++) {
    char c = str[pos + i];
    val <<= 4;
    if (c >= '0' && c <= '9') val += (c - '0');
    else if (c >= 'A' && c <= 'F') val += (c - 'A' + 10);
    else if (c >= 'a' && c <= 'f') val += (c - 'a' + 10);
  }
  return val;
}


float calcAngel(float aktuellerWert, float minWert, float maxWert) {
  // Begrenzt den aktuellen Wert strikt zwischen Min und Max, um Fehler zu vermeiden
  aktuellerWert = constrain(aktuellerWert, minWert, maxWert);
  
  // Berechnet den prozentualen Fortschritt (0.0 bis 1.0)
  float fortschritt = (aktuellerWert - minWert) / (maxWert - minWert);
  
  // Berechnet den Winkel (0% = 360 Grad, 100% = 0 Grad)
  float winkel = 360.0 - (fortschritt * 360.0);
  
  return winkel;
}


void parsePylontechFrame(const char* frame) {

  DBG_PRINTLN(frame);
  
  uint16_t minCellmV = 9999; // Startwert sehr hoch ansetzen
  uint16_t maxCellmV = 0;
  uint8_t minCellNum = 0;
  uint8_t maxCellNum = 0;

  // 1. Validierung (Startzeichen prüfen)
  if (frame[0] != '~') return;

  // 2. Header parsen
  uint8_t version = parseHex(frame, 1, 2);
  uint8_t address = parseHex(frame, 3, 2);
  uint8_t cid1    = parseHex(frame, 5, 2);
  uint8_t cid2    = parseHex(frame, 7, 2);
  
  DBG_PRINTLN("=== PYLONTECH FRAME EMPFANGEN ===");
  sprintf(dbgbuffer,"Protokoll-Version: %02X\n", version);
  DBG_PRINT(dbgbuffer);

  sprintf(dbgbuffer,"Geräte-Adresse:    %d\n", address);
  DBG_PRINT(dbgbuffer);

  sprintf(dbgbuffer,"CID1 (Akkutyp):    %02X\n", cid1);
  DBG_PRINT(dbgbuffer);

  sprintf(dbgbuffer,"CID2 (Status):     %02X\n", cid2);
  DBG_PRINT(dbgbuffer);

      
  if (cid2 != 0) {
    DBG_PRINTLN("Warnung: BMS meldet einen Fehlercode!");
  }

  // 3. Zellenzahl
  int index = 17;

  // Zellspannungen
  uint8_t cellCount = parseHex(frame, index, 2);
  sprintf(dbgbuffer,"Anzahl Zellen:     %d\n", cellCount);
  DBG_PRINT(dbgbuffer);
  index += 2;

    
  DBG_PRINTLN("\n--- Einzelzellen ---");
  for (int i = 0; i < cellCount; i++) {
    uint16_t cellMv = parseHex(frame, index, 4);
    index += 4;

    sprintf(dbgbuffer,"Zelle %02d: %d mV\n", i + 1, cellMv);
    DBG_PRINT(dbgbuffer);
    
    //send mqtt data
    sprintf(buffer_topic,"%s%s_%02d", topic_root, "/voltages/cell", i+1);
    sprintf(buffer_payload,"%d", cellMv);
    client.publish(buffer_topic, buffer_payload);

    bat_cellvoltages[i] = cellMv;

    // Minimale Zellenspannung + Zelle ermitteln
    if (cellMv < minCellmV) {
      minCellmV = cellMv;
      minCellNum = i + 1; 
    }
    
    // Maximale Zellenspannung + Zelle ermitteln
    if (cellMv > maxCellmV) {
      maxCellmV = cellMv;
      maxCellNum = i + 1; 
    }
    
  }

  // Berechnung des Zell-Deltas (Spannungsunterschied)
  uint16_t deltaMv = maxCellmV - minCellmV;

  // Ausgabe
  DBG_PRINTLN("\n--- Zell-Statistiken ---");

  //DBG_PRINTF("Minimale Zelle: Zelle %02d (%d mV)\n", minCellNum, minCellmV);
  sprintf(dbgbuffer,"Minimale Zelle: Zelle %02d (%d mV)\n", minCellNum, minCellmV);
  DBG_PRINT(dbgbuffer);

  //DBG_PRINTF("Maximale Zelle: Zelle %02d (%d mV)\n", maxCellNum, maxCellmV );
  sprintf(dbgbuffer,"Maximale Zelle: Zelle %02d (%d mV)\n", maxCellNum, maxCellmV);
  DBG_PRINT(dbgbuffer);

  //DBG_PRINTF("Zell-Differenz (Delta): %d mV (%.3f V)\n", deltaMv, (float)deltaMv / 1000.0);
  sprintf(dbgbuffer,"Zell-Differenz (Delta): %d mV (%.3f V)\n", deltaMv, (float)deltaMv / 1000.0);
  DBG_PRINT(dbgbuffer);

  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/min_cell_voltage");
  sprintf(buffer_payload,"%d", minCellmV);
  client.publish(buffer_topic, buffer_payload);
  bat_mincellvoltage = minCellmV;  

  sprintf(buffer_topic,"%s%s", topic_root, "/cell_with_min_voltage");
  sprintf(buffer_payload,"C%d", minCellNum);
  client.publish(buffer_topic, buffer_payload);
  bat_cellmin = minCellNum;

  sprintf(buffer_topic,"%s%s", topic_root, "/max_cell_voltage");
  sprintf(buffer_payload,"%d", maxCellmV);
  client.publish(buffer_topic, buffer_payload);
  bat_maxcellvoltage =  maxCellmV;

  sprintf(buffer_topic,"%s%s", topic_root, "/cell_with_max_voltage");
  sprintf(buffer_payload,"C%d", maxCellNum);
  client.publish(buffer_topic, buffer_payload);
  bat_cellmax =  maxCellNum;

  sprintf(buffer_topic,"%s%s", topic_root, "/cell_difference");
  sprintf(buffer_payload,"%d", deltaMv);
  client.publish(buffer_topic, buffer_payload);
  bat_celldelta = deltaMv;


  // Temperaturen
  uint8_t tempCount = parseHex(frame, index, 2);
  index += 2;

  DBG_PRINTLN("\n--- Temperaturen ---");
  for (int i = 0; i < tempCount; i++) {
    uint16_t tempKelvin10 = parseHex(frame, index, 4);
    index += 4;
    float tempC = ((float)tempKelvin10 / 10.0) - 273.15;
    //DBG_PRINTF("Sensor %d: %.1f °C\n", i + 1, tempC);
    sprintf(dbgbuffer,"Sensor %d: %.1f °C\n", i + 1, tempC);
    DBG_PRINT(dbgbuffer);

    if(i==0){
      bat_temp_mosfet = round(tempC * 10) / 10;
    }
    if(i==1){
      bat_temp_cells = round(tempC * 10) / 10;
    }
    
    //send mqtt data
    sprintf(buffer_topic,"%s%s_%02d", topic_root, "/temperatures/sensor", i+1);
    sprintf(buffer_payload,"%.1f", tempC);
    client.publish(buffer_topic, buffer_payload);
  }

  // 4. Systemwerte
  // Strom (Signed 16-Bit für Laden/Entladen)
  int16_t currentRaw = (int16_t)parseHex(frame, index, 4);
  index += 4;
  float currentA = (float)currentRaw / 1000.0;
  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/current");
  sprintf(buffer_payload,"%.1f", currentA);
  client.publish(buffer_topic, buffer_payload); 
  bat_current = currentA;

  // Gesamtspannung
  uint16_t totalVoltageMv = parseHex(frame, index, 4);
  index += 14; // Sprung nach Zyklenzahl da bei Kapazitäten >65Ah das neue Protokoll greift
  float totalVoltageV = (float)totalVoltageMv / 1000.0;
  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/voltage");
  sprintf(buffer_payload,"%.1f", totalVoltageV);
  client.publish(buffer_topic, buffer_payload);
  bat_voltages = totalVoltageV;

  //aktuelle Leistung(Watt) berechnen
  bat_leistung = totalVoltageV * currentA;

  
  // Zyklenanzahl
  uint16_t cycles = parseHex(frame, index, 4);
  index += 4;
  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/charge_cycles");
  sprintf(buffer_payload,"%d", cycles);
  client.publish(buffer_topic, buffer_payload);
  bat_cycles = cycles;


  // Verbleibende Kapazität
  uint32_t remainCapacityMha = parseHex(frame, index, 6);
  index += 6;
  float remainCapacityAh = (float)remainCapacityMha / 1000.0;
  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/remain_capcity");
  sprintf(buffer_payload,"%.1f", remainCapacityAh);
  client.publish(buffer_topic, buffer_payload);

  bat_remain_ah = round(remainCapacityMha / 1000);
  bat_soc = ((uint16_t)round(remainCapacityAh));
 

  //Totale Kapazität der Batterie
  uint32_t totalCapacityMha = parseHex(frame, index, 6);
  float totalCapacityAh = (float)totalCapacityMha / 1000.0;
  //send mqtt data
  sprintf(buffer_topic,"%s%s", topic_root, "/total_capcity");
  sprintf(buffer_payload,"%.1f", totalCapacityAh);
  client.publish(buffer_topic, buffer_payload);
  bat_totalcapacity = ((uint16_t)round(totalCapacityAh));

  // Ende Frame
  // danach 2 Byte Checksumme

  
  DBG_PRINTLN("\n--- Systemwerte ---");
  //DBG_PRINTF("Gesamtspannung:    %.2f V\n", totalVoltageV);
  sprintf(dbgbuffer,"Gesamtspannung:    %.2f V\n", totalVoltageV);
  DBG_PRINT(dbgbuffer);

  //DBG_PRINTF("Stromfluss:        %.2f A\n", currentA);
  sprintf(dbgbuffer,"Stromfluss:        %.2f A\n", currentA);
  DBG_PRINT(dbgbuffer);

   //DBG_PRINTF("Ladezyklen:        %d\n", cycles);
  sprintf(dbgbuffer,"Ladezyklen:        %d\n", cycles);
  DBG_PRINT(dbgbuffer);

  //DBG_PRINTF("Restkapazität:     %.2f Ah\n", remainCapacityAh);
  sprintf(dbgbuffer,"Restkapazität:     %.2f Ah\n", remainCapacityAh);
  DBG_PRINT(dbgbuffer);

  //DBG_PRINTF("Gesamtkapazität:        %d\n", totalCapacityAh);
  sprintf(dbgbuffer,"Gesamtkapazität:   %.2f Ah\n", totalCapacityAh);
  DBG_PRINT(dbgbuffer);

  DBG_PRINTLN("=================================\n");

  
}


//mqtt reconnent
void reconnect() {
  DBG_PRINTLN("reconnect MQTT connection!");

  client.setServer(mqtt_server, atoi(mqtt_port));

  sprintf(dbgbuffer,"reconnect: mqtt_server: %s\n", mqtt_server);
  DBG_PRINT(dbgbuffer);
  sprintf(dbgbuffer,"reconnect: mqtt_port: %d\n", atoi(mqtt_port));
  DBG_PRINT(dbgbuffer);

  while (!client.connected()) {
    DBG_PRINTLN("Versuche MQTT-Verbindung...");
        
    if (client.connect(clientId)) {
      DBG_PRINTLN("verbunden!");
      client.publish("esp32s3/status", "online");
      strcpy(mqtt_state, "Verbunden");
    } else {
      strcpy(mqtt_state, "Fehler!");
      DBG_PRINT("Fehlgeschlagen, RC=");
      DBG_PRINT(client.state());
      DBG_PRINTLN(" - Nächster Versuch in 5 Sekunden");
      delay(5000);
    }
  }
}


void setup() {
  // Setup Telnet
  SerialAndTelnet.setWelcomeMsg(F("Welcome\r\n\n"));
  SerialAndTelnet.setCallbackOnConnect(telnetConnected);
  SerialAndTelnet.setCallbackOnDisconnect(telnetDisconnected);
  SerialAndTelnet.setFilter(char(1), F("\r\nNVT command: AO\r\n"), disconnectClientWrapper);

  // turn off RGB LED
  rgbled.begin();
  rgbled.clear();
    
  // Setup Serial
  Serial.begin(115200);
  rgbled.setPixelColor(0, rgbled.Color(0, 0, 255)); // Blue
  rgbled.show();
  delay(1000);
  rgbled.setPixelColor(0, rgbled.Color(0, 0, 0)); // off
  rgbled.show();
   
  // setup RS485 (Serial2)
  BMS_SERIAL.begin(9600, SERIAL_8N1, rxPin, txPin);
  pinMode(DE_RE_PIN, OUTPUT);
  digitalWrite(DE_RE_PIN, RS485_RECIVE); 

  DBG_PRINTLN("DBG 1");


  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) { 
     DBG_PRINT("LittleFS mount failed, ");
     DBG_PRINTLN(" Create Filesystem");
     return;
  }

  listDir(LittleFS, "/", 0);

  if (!LittleFS.exists("/config.json")) { 
    saveConfig();
  }

  readConfig(); //read config.json

 
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6); 
 

  WiFiManager wifiManager;
  
  wifiManager.setDebugOutput(true);
  //wifiManager.resetSettings();
 
  wifiManager.setClass("invert"); // dark theme
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setConnectTimeout(20);
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
 
  // create menu with only two entries
  std::vector<const char *> wm_menu  = {"wifi", "exit"};
  wifiManager.setShowInfoUpdate(false);
  wifiManager.setShowInfoErase(false);
  wifiManager.setMenu(wm_menu);
  
  // start WifiManager
  bool res = wifiManager.autoConnect(clientId);
  
  if(!res) {
    DBG_PRINTLN("Failed to connect");
    ESP.restart();
  } else {
    DBG_PRINT("WiFi connected to ");
    DBG_PRINTLN(String(WiFi.SSID()));
    DBG_PRINT("RSSI = ");
    DBG_PRINT(String(WiFi.RSSI()));
    DBG_PRINTLN(" dBm");
    DBG_PRINT("IP address  ");
    DBG_PRINTLN(WiFi.localIP());
    DBG_PRINTLN();

    //read updated parameters
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    
    //save the custom parameters to FS
    if (shouldSaveConfig) {
      saveConfig();
    }

    if(checkbox_mqttenabled){
      DBG_PRINTLN("set mqtt server!");
      DBG_PRINTLN(String("mqtt_server: ") + mqtt_server);
      DBG_PRINTLN(String("mqtt_port: ") + mqtt_port);

      client.setServer(mqtt_server, atoi(mqtt_port));
      //client.setCallback(mqtt_callback);
    }
  }

  WiFi.mode(WIFI_STA);
  WiFi.macAddress(mac);
  WiFi.persistent(true);

  sprintf(clientId, "ESP32_%02x%02x%02x", mac[3], mac[4], mac[5] );
 

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      DBG_PRINTLN("");
      sprintf(dbgbuffer,"Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      DBG_PRINTLN(dbgbuffer);
    }
    client->send("hello!", NULL, millis(), 10000);
  });

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    new_connect = true;
    request->send_P(200, "text/html", index_html, processor);
  });  

   // crate json and fetch data
  server.on("/json", HTTP_GET, [] (AsyncWebServerRequest *request){
    JsonDocument jdoc;
    String message = "";

    rssi = WiFi.RSSI();
    
    jdoc["WIFIRSSI"]      = rssi;
    jdoc["WIFIQUALITI"]   = dBmtoPercent(rssi);
    jdoc["CLIENTID"]      = clientId;
    jdoc["MQTTSERVER"]    = mqtt_server;
    jdoc["MQTTPORT"]      = mqtt_port;
    jdoc["MQTTROOT1"]     = topic_root;
    jdoc["CBMQTTSTATE"]   = checkbox_mqttenabled; //checkbox mqtt ein/aus

    if(checkbox_mqttenabled){
        jdoc["MQTTSTATE"] = "EIN";
      }else{
        jdoc["MQTTSTATE"] = "AUS";
    }
    
    jdoc["MQTTSTATECL"]   = mqtt_state;
    jdoc["VOLTAGES"]      = bat_voltages;
	  jdoc["CURRENT"]       = bat_current;

    if(bat_leistung >= 1000){
      String kw = String(bat_leistung, 0) + " kW";
      jdoc["LEISTUNG"]      = kw;  
    }else{
      String w = String(bat_leistung, 0) + " W";
      jdoc["LEISTUNG"]      = w;
    }

    jdoc["RINGSOC"]       = calcAngel(bat_remain_ah, 0, 100);
    jdoc["SOC"]           = bat_soc;
    jdoc["TOTALCAPACITY"] = bat_totalcapacity;
    jdoc["TEMPCELLS"]     = bat_temp_cells;
    jdoc["TEMPMOSFET"]    = bat_temp_mosfet;

	  jdoc["MAXCELLVOLTAGE"] = bat_maxcellvoltage;
	  jdoc["MINCELLVOLTAGE"] = bat_mincellvoltage;  
	  jdoc["CELLDELTA"]     = bat_celldelta;   
	  jdoc["CELLMAX"]       = bat_cellmax;     
	  jdoc["CELLMIN"]       = bat_cellmin;
    jdoc["CYCLES"]        = bat_cycles;    
	  jdoc["CELL01VOLTAGE"] = bat_cellvoltages[0];   
	  jdoc["CELL02VOLTAGE"] = bat_cellvoltages[1];   
	  jdoc["CELL03VOLTAGE"] = bat_cellvoltages[2];   
	  jdoc["CELL04VOLTAGE"] = bat_cellvoltages[3];   
	  jdoc["CELL05VOLTAGE"] = bat_cellvoltages[4];   
	  jdoc["CELL06VOLTAGE"] = bat_cellvoltages[5];
	  jdoc["CELL07VOLTAGE"] = bat_cellvoltages[6];   
	  jdoc["CELL08VOLTAGE"] = bat_cellvoltages[7];   
	  jdoc["CELL09VOLTAGE"] = bat_cellvoltages[8];  
	  jdoc["CELL10VOLTAGE"] = bat_cellvoltages[9];   
	  jdoc["CELL11VOLTAGE"] = bat_cellvoltages[10];  
	  jdoc["CELL12VOLTAGE"] = bat_cellvoltages[11];  
	  jdoc["CELL13VOLTAGE"] = bat_cellvoltages[12];  
	  jdoc["CELL14VOLTAGE"] = bat_cellvoltages[13];  
	  jdoc["CELL15VOLTAGE"] = bat_cellvoltages[14];   
	  jdoc["CELL16VOLTAGE"] = bat_cellvoltages[15];

    serializeJson(jdoc, message);
    request->send(200, "application/json", message);
  });

  // check checkboxes
  server.on("/checkbox", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String checkbox_id;
      String checkbox_value;
     
      if (request->hasParam("cbid") && request->hasParam("state")) {
        checkbox_id = request->getParam("cbid")->value();
        checkbox_value = request->getParam("state")->value();
       
        if(checkbox_id.equals("CBMQTTSTATE")){
          if(checkbox_value.equals("1")){
            checkbox_mqttenabled = true;
          } else {
            checkbox_mqttenabled = false;
          }
        }
        
      }    
      request->send_P(200, "text/html", index_html, processor);
    });

  // start AP Mode
  server.on("/apmode", HTTP_GET, [](AsyncWebServerRequest *request) {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
      
    ESP.restart();
    request->send_P(200, "text/html", index_html, processor);
  });

   // restart system
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {   
    DBG_PRINTLN("/restart");  
    ESP.restart();
    request->send_P(200, "text/html", index_html, processor);
  });

  // save settings
  server.on("/savesettings", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String value;

    value =  request->getParam("mqttserver")->value();
    memset(mqtt_server, 0, sizeof(mqtt_server)); 
    strcat(mqtt_server, value.c_str());

    value =  request->getParam("mqttport")->value();
    memset(mqtt_port, 0, sizeof(mqtt_port)); 
    strcat(mqtt_port, value.c_str());

    saveConfig(); 

    request->send_P(200, "text/html", index_html, processor);
  });

  //*****************************
 
  ElegantOTA.begin(&server);
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
    
  server.onNotFound(notFound);
  server.addHandler(&events);
  server.begin();

  rssi = WiFi.RSSI();

  telnetServer.begin();
  telnetServer.setNoDelay(true);
  Serial.println("Please connect Telnet Client, exit with ^] and 'quit'");

} 


void loop() {

   if (telnetServer.hasClient()) {
    if (!serverClient || !serverClient.connected()) {
      if (serverClient) {
        serverClient.stop();
        Serial.println("Telnet Client Stop");
      }
      serverClient = telnetServer.available();
      Serial.println("New Telnet client");
      serverClient.println("client ist connected...");
      serverClient.flush();  // clear input buffer, else you get strange characters 
    }
  }

  ElegantOTA.loop();
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Hält die Verbindung am Leben


  // send bms request 
  if (millis() - startTime > 10000) { // run every 2000 ms
    startTime = millis();
    sendBmsRequest(REQUEST);  
  }

 
  while (BMS_SERIAL.available()) {
    char c = BMS_SERIAL.read();

    // Startzeichen erkannt, Puffer zurücksetzen und Datenframe erfassen
    if (c == '~') {
      DBG_PRINTLN("Startzeichen erkannt: ~");
      rxIndex = 0;
      recording = true;
    }
    
    if (recording) {
      if (rxIndex < MAX_FRAME_LENGTH -  1) {
        rxBuffer[rxIndex++] = c;
        rxBuffer[rxIndex] = '\0';
      }

      // Ende der Nachricht (Carriage Return oder Line Feed)
      if (c == '\r' || c == '\n') {
        DBG_PRINTLN("Ende der Nachricht erkannt");
        recording = false;
        // Nur parsen, wenn der Frame eine Mindestlänge hat
        if (rxIndex > 20) {
          if(calculatePylontechChecksum(rxBuffer)){
            parsePylontechFrame(rxBuffer);
          }
        }
        rxIndex = 0;
      }
    }     
  }
  
}
