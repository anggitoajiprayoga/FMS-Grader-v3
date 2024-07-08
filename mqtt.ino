void setup_mqtt() {
  Serial.println();
  Serial.println("Searching for provider.");
  byte count_network = 0;
  while (!modem.waitForNetwork() and count_network < 3)
  {
    count_network++;
    Serial.println("fail, count return: " + String(count_network));
    delay(250);
  }

  if (!modem.waitForNetwork()) {
    Serial.println("Not Connect to provider.");
  }
  else {
    Serial.println("Connected to provider.");
    Serial.println("Signal Quality: " + String(modem.getSignalQuality()));
    Serial.println();
  }

  Serial.println("Connecting to GPRS network.");
  byte count_gprs = 0;
  while (!modem.gprsConnect("internet", "", "") and count_gprs < 3)
  {
    count_gprs++;
    Serial.println("fail, count return: " + String(count_gprs));
    delay(250);
  }

  if (!modem.gprsConnect("internet", "", "")) {
    Serial.println("Not Connect to GPRS: " + String(apn));
    Serial.println();
  }
  else {
    Serial.println("Connected to GPRS: " + String(apn));
    Serial.println();
  }

  mqtt.setServer((char*) broker.c_str(), 1883);
  mqtt.setCallback(callback);

  Serial.println("Connecting to MQTT Broker: " + String(broker));
  Serial.println("username: " + username);
  Serial.println("password: " + password);
  byte count_mqtt = 0;
  while (mqttConnect() == false and count_mqtt < 3) {
    count_mqtt++;
    Serial.println("fail, count return: " + String(count_mqtt));
    delay(250);
  }
  Serial.print("mqtt state: " + String(mqtt.state()));
  Serial.println();
}

void loop_mqtt() {
  if (!mqtt.connected()) {
    static int count_reconnect = 0;
    static int count_rst = 0;
    rssi = "unknown";
    mqtt_state = "Not_Connect";
    esp_task_wdt_reset();
    if (count_reconnect < 3) {
      count_reconnect++;
      reconnect();
    }
    else {
      count_reconnect++;
      if (count_reconnect > 3000) {
        count_rst++;
        count_reconnect = 0;
      }
    }

    if (count_rst == 3) {
      Serial.println();
      Serial.println("Restart System !!!");
      Serial.println();
      delay(3000);
      ESP.restart();
    }
  }
  else {
    rssi = String(-113 + (2 * modem.getSignalQuality()));
    mqtt.loop();
    mqtt_state = "Connect";
    static unsigned long prev_mqtt = 0;
    if ((millis() - prev_mqtt) > (interval_mqtt.toInt() * 1000)) {
      String get_liveLocations = liveLocations();
      pub("fms/liveLocations", get_liveLocations);
      delay(150);
      last_update();
      prev_mqtt = millis();
    }
  }
}

void pub(String topic, String _jsonDataString)
{
  if (en_lte == "true" and en_mqtt == "true") {
    if (mqtt.connected()) {
      char _jsonDataStringChar[_jsonDataString.length() + 1];
      char topicChar[topic.length() + 1];
      _jsonDataString.toCharArray(_jsonDataStringChar, _jsonDataString.length() + 1);
      topic.toCharArray(topicChar, topic.length() + 1);

      mqtt.publish(topicChar, _jsonDataStringChar);
      Serial.println();
      Serial.println(topic);
      Serial.println(_jsonDataString);
      Serial.println(_jsonDataString.length());
      Serial.println();
    }
  }
}

boolean mqttConnect()
{
  Serial.print("Connecting to MQTT broker...");
  if (!mqtt.connect(cn.c_str(), username.c_str(), password.c_str()))
  {
    Serial.print("Failed to connect, state: ");
    int state = mqtt.state();
    Serial.println(state);

    switch (state) {
      case -4:
        Serial.println("Error: MQTT_CONNECTION_TIMEOUT - Waktu habis saat menunggu untuk koneksi");
        break;
      case -3:
        Serial.println("Error: MQTT_CONNECTION_LOST - Koneksi hilang");
        break;
      case -2:
        Serial.println("Error: MQTT_CONNECT_FAILED - Koneksi gagal");
        break;
      case -1:
        Serial.println("Error: MQTT_DISCONNECTED - Terputus");
        break;
      case 1:
        Serial.println("Error: MQTT_CONNECT_BAD_PROTOCOL - Protokol tidak valid");
        break;
      case 2:
        Serial.println("Error: MQTT_CONNECT_BAD_CLIENT_ID - Client ID tidak valid");
        break;
      case 3:
        Serial.println("Error: MQTT_CONNECT_UNAVAILABLE - Server tidak tersedia");
        break;
      case 4:
        Serial.println("Error: MQTT_CONNECT_BAD_CREDENTIALS - Kredensial tidak valid");
        break;
      case 5:
        Serial.println("Error: MQTT_CONNECT_UNAUTHORIZED - Tidak diizinkan");
        break;
      default:
        Serial.println("Error: Unknown error");
        break;
    }
    return false;
  }

  String topic_sub = "fms/gd/remote/" + cn;
  char _jsonDataStringChar[topic_sub.length() + 1];
  topic_sub.toCharArray(_jsonDataStringChar, topic_sub.length() + 1);
  mqtt.subscribe(_jsonDataStringChar);
  Serial.println("sub: " + topic_sub);

  String topic_sub1 = "fms/gd/remote";
  char _jsonDataStringChar1[topic_sub1.length() + 1];
  topic_sub1.toCharArray(_jsonDataStringChar1, topic_sub1.length() + 1);
  mqtt.subscribe(_jsonDataStringChar1);
  Serial.println("sub1: " + topic_sub1);

  Serial.println("Connected to broker.");
  return mqtt.connected();
}

void callback(char* topic, byte * payload, unsigned int length)
{
  String pay = parsing_subscribe(topic, payload, length);
  Serial.print("Data Payload Subscribe : ");
  Serial.println(pay);
  websocket_parse(pay);
  pub("fms/gd/device/callback/" + cn, "device message received");

  if (pay == "update")
  {
    pub("fms/gd/device/callback/" + cn, "update OTA proccess");
    Serial.println("Update Firmware by OTA");
    delay(2000);
    ota();
  }

  if (pay == "restart")
  {
    pub("fms/gd/device/callback/" + cn, "restrat device");
    Serial.println("restart device");
    delay(2000);
    ESP.restart();
  }
}

String parsing_subscribe(char* topic, byte * payload, unsigned int length)
{
  String _receivedTopic = String(topic);
  String payloadString, parsedStringx;
  for (int i = 0; i < length; i++)
  {
    payloadString += char(payload[i]);
  }
  parsedStringx = payloadString;
  return parsedStringx;
}

void reconnect() {
  byte count_network = 0;
  while (!modem.waitForNetwork() and count_network < 3)
  {
    count_network++;
    Serial.println("fail, count return: " + String(count_network));
  }

  if (!modem.waitForNetwork()) {
    Serial.println("Not Connect to provider.");
  }
  else {
    Serial.println("Connected to provider.");
    Serial.println("Signal Quality: " + String(modem.getSignalQuality()));
    Serial.println();
  }

  Serial.println("Connecting to GPRS network.");
  byte count_gprs = 0;
  while (!modem.gprsConnect("internet", "", "") and count_gprs < 3)
  {
    count_gprs++;
    Serial.println("fail, count return: " + String(count_gprs));
  }

  if (!modem.gprsConnect("internet", "", "")) {
    Serial.println("Not Connect to GPRS: " + String(apn));
    Serial.println();
  }
  else {
    Serial.println("Connected to GPRS: " + String(apn));
    Serial.println();
  }

  mqtt.setServer((char*) broker.c_str(), 1883);
  mqtt.setCallback(callback);

  Serial.println("Connecting to MQTT Broker: " + String(broker));
  Serial.println("username: " + username);
  Serial.println("password: " + password);
  byte count_mqtt = 0;
  while (mqttConnect() == false and count_mqtt < 3) {
    count_mqtt++;
    Serial.println("fail, count return: " + String(count_mqtt));
  }
  Serial.print("mqtt state: " + String(mqtt.state()));
  Serial.println();
}
