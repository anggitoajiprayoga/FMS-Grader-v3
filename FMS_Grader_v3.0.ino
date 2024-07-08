#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <ArduinoHttpClient.h>
#include <Adafruit_ADS1X15.h>
#include <Arduino.h>
#include <esp_task_wdt.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <Update.h>

#define SerialAT Serial1
#define PROTOCOL SERIAL_8N1
#define rx 26
#define tx 27
#define RST 14

#define setpoin_altenator 200
#define setpoin_parking_break 200
#define setpoin_netral 200


#define WDT_TIMEOUT 60
#define SD_CS 5

#define ads_address 0x48
#define RXPin 33
#define TXPin 32
#define GPSBaud 9600

const char apn[]      = "Internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

String version = "Firm v3.0 - FMS Grader";
String firm_ver = "v3.0";
String xfirm_ver;
String type_board = "FMS Grader Board";
String type_unit = "GD";
String timeZone = "+8:00";
String sn_dev;

String http_username;
String http_password;
String ssid_wifi_client;
String pass_wifi_client;
String ssid_wifi_ap;
String pass_wifi_ap;
String freesp, usedstr, totalstr;

String webpage = "";
bool shouldReboot = false;
String listFiles(bool ishtml = false);

String server_ota, resource_ota;
const int port_ota = 80;

const String default_ssid = "";
const String default_wifipassword = "SS6";
const String default_httpuser = "admin";
const String default_httppassword = "admin";
const int default_webserverporthttp = 80;

String ssid;
String wifipassword;
String httpuser;
String httppassword;
int webserverporthttp;

uint64_t sizeTotal, sizeKB, totalKB, usedKB, usedB, spaceKB;
uint32_t usedByte;
bool SD_present = false, deleting = false;

byte shift, xshift;
int Year;
byte Month;
byte Day;
byte Hour;
byte Minute;
byte Second;
char duration[20];

int adc_altenator, adc_parking, adc_netral, adc_fuel;
String engine_state, engine_condition, hm, time_update;
String no_simcard, imei, rssi;
String value_year, value_month, value_day, value_hour, value_minute, value_second;
String en_lte, en_mqtt, broker, username, password, mqtt_state, interval_mqtt;
String cn, sn, brand;
String dates, times, dateTime, board_temp, ind;
int fuel;
bool altenator, parking_break, netral;
float latitude, longitude, speed, altitude, heading, satelite;
bool en_update, state_gps, state_rtc, state_sdcard;

String running_historical;
String idle_historical;
String off_historical;

char datetime_start_running[25];
char datetime_start_idle[25];
char datetime_start_off[25];
char datetime_stop_running[25];
char datetime_stop_idle[25];
char datetime_stop_off[25];
char duration_running[15];
char duration_idle[15];
char duration_off[15];
char running_realtime[15];
char idle_realtime[15];
char off_realtime[15];
char hm_unit[15];
char hm_realtime[15];

bool engine_running, engine_idle, engine_off;
bool en_write_running, en_write_idle, en_write_off;

byte jam_running, menit_running, detik_running;
byte jam_idle, menit_idle, detik_idle;
byte jam_off, menit_off, detik_off;

byte start_tanggal_running, start_bulan_running, start_jam_running, start_menit_running, start_detik_running;
byte start_tanggal_idle, start_bulan_idle, start_jam_idle, start_menit_idle, start_detik_idle;
byte start_tanggal_off, start_bulan_off, start_jam_off, start_menit_off, start_detik_off;
int start_tahun_off, start_tahun_idle, start_tahun_running;

byte stop_tanggal_running, stop_bulan_running, stop_jam_running, stop_menit_running, stop_detik_running;
byte stop_tanggal_idle, stop_bulan_idle, stop_jam_idle, stop_menit_idle, stop_detik_idle;
byte stop_tanggal_off, stop_bulan_off, stop_jam_off, stop_menit_off, stop_detik_off;
int stop_tahun_off, stop_tahun_idle, stop_tahun_running;

String hm_stop_running, hm_stop_idle, hm_stop_off;
String hm_start_running, hm_start_idle, hm_start_off;

int hour_realtime;
int minute_realtime;
int second_realtime;

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient mqtt(client);
Adafruit_ADS1015 ads;
TinyGPSPlus gps;
SoftwareSerial serial_gps(RXPin, TXPin);
RtcDS3231<TwoWire> Rtc(Wire);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiServer serverAP(80);

String payload() {
  String jsonString = "";
  DynamicJsonDocument buffer(2048);
  JsonObject obj           = buffer.to<JsonObject>();
  obj["sn"]                = sn;
  obj["cn"]                = cn;
  obj["dateTime"]          = dateTime;
  obj["timeZone"]          = timeZone;
  obj["board_temp"]        = board_temp;
  obj["adc_altenator"]     = altenator;
  obj["adc_parking_break"] = parking_break;
  obj["adc_netral"]       = netral;
  obj["adc_fuel"]         = fuel;
  obj["engine_condition"] = engine_state;
  obj["hm"]               = hm_realtime;
  obj["rssi"]             = rssi;
  obj["lat"]              = latitude;
  obj["lng"]              = longitude;
  obj["alt"]              = roundToDecimalPlaces(altitude, 1);
  obj["spd"]              = int(speed);
  obj["hdg"]              = int(heading);
  obj["ind"]              = ind;
  serializeJsonPretty(obj, jsonString);
  return jsonString;
}

String payload_production(String condition, int _shift, String time_start, String time_stop, String duration, String hmstart, String hmstop) {
  String json_production = "";
  //  DynamicJsonDocument buffer(1024);
  StaticJsonDocument<1024> buffer;
  JsonObject obj  = buffer.to<JsonObject>();
  obj["sn"]       = sn;
  obj["cn"]       = cn;
  obj["brand"]    = brand;
  obj["type"]     = type_unit;
  obj["shift"]    = _shift;
  obj["dateTime"] = dateTime;
  obj["timeZone"] = timeZone;
  obj["cdt"]      = condition;
  obj["str"]      = time_start;
  obj["stp"]      = time_stop;
  obj["drt"]      = duration;
  obj["hmstr"]    = atof(hmstart.c_str());
  obj["hmstp"]    = atof(hmstop.c_str());

  JsonObject dataSupport = obj.createNestedObject("data");
  dataSupport["dateTime"] = dateTime;
  dataSupport["timeZone"] = timeZone;
  dataSupport["lat"] = latitude;
  dataSupport["lng"] = longitude;
  dataSupport["alt"] = roundToDecimalPlaces(altitude, 1);
  dataSupport["spd"] = int(speed);
  dataSupport["hdg"] = int(heading);
  serializeJsonPretty(obj, json_production);
  return json_production;
}

String liveLocations() {
  String livelocation = "";
  DynamicJsonDocument buffer(1024);
  JsonObject obj  = buffer.to<JsonObject>();
  obj["sn"]       = sn;
  obj["cn"]       = cn;
  obj["type"]     = type_unit;
  obj["dateTime"] = dateTime;
  obj["timeZone"] = timeZone;
  obj["lat"]      = latitude;
  obj["lng"]      = longitude;
  obj["alt"]      = roundToDecimalPlaces(altitude, 1);
  obj["spd"]      = int(speed);
  obj["hdg"]      = int(heading);
  obj["rssi"]     = rssi;
  obj["firm"]     = firm_ver;
  serializeJsonPretty(obj, livelocation);
  return livelocation;
}

void notifyClients(String payload) {
  ws.textAll(payload);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    Serial.println((char*)data);
    websocket_parse(String((char*)data));
    String get_payload = payload();
    //    Serial.println("//web_socket_payload_receive//");
    //    Serial.println(get_payload);
    notifyClients(get_payload);
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
      Serial.printf("WebSocket client #%u pong received\n", client->id());
      break;
    case WS_EVT_ERROR:
      Serial.println("WebSocket error occurred. Restarting ESP32...");
      delay(2000);
      ESP.restart();
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  read_setting();
  setup_webserver();
  setup_wifi();
  initWebSocket();
  setup_gps();
  setup_ads();
  setup_sd_card();
  setup_rtc();
  setup_main();
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);
}

void loop() {
  loop_gps();
  loop_ads();
  loop_rtc();
  loop_main();
  loop_wifi();
  loop_logging();
  changeShift();
  serial_monitor();
  esp_task_wdt_reset();
}
