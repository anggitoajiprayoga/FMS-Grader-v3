void data_default() // kalau masih  perawan, belum ada database // ntar lu isi defaultnya apa
{
  if (String(readFile(SPIFFS, "/cn.txt")) == "") {
    cn = "FMS Grader";
    writeFile(SPIFFS, "/cn.txt", "FMS Grader");
  }
  else {
    cn = readFile(SPIFFS, "/cn.txt");
  }

  writeFile(SPIFFS, "/http_username.txt", "admin");
  writeFile(SPIFFS, "/http_password.txt", "admin");
  writeFile(SPIFFS, "/cn.txt", cn.c_str());
  writeFile(SPIFFS, "/sn.txt", "1234");
  writeFile(SPIFFS, "/hm.txt", "0");
  writeFile(SPIFFS, "/brand.txt", "Komatsu GD825");
  writeFile(SPIFFS, "/ssid_wifi_ap.txt", cn.c_str());
  writeFile(SPIFFS, "/pass_wifi_ap.txt", "12345678");
  writeFile(SPIFFS, "/en_lte.txt", "false");
  writeFile(SPIFFS, "/no_simcard.txt", "08xxxxxxxxx");
  writeFile(SPIFFS, "/en_mqtt.txt", "false");
  writeFile(SPIFFS, "/broker.txt", "103.88.152.49");
  writeFile(SPIFFS, "/username.txt", "device-client");
  writeFile(SPIFFS, "/password.txt", "yC9QDtWnYM65VERhkswmpT");
  writeFile(SPIFFS, "/interval_mqtt.txt", "5");
  writeFile(SPIFFS, "/server_ota.txt", "storage.ppa-bib.net");
  writeFile(SPIFFS, "/resource_ota.txt", "/firmware/towerlamp/update.bin");
  writeFile(SPIFFS, "/firm_ver.txt", "v3.0");
  Serial.println("default ok");
}

void read_setting()
{
  cn = readFile(SPIFFS, "/cn.txt");
  sn = readFile(SPIFFS, "/sn.txt");

  if (cn == "" and sn == "")
  {
    data_default();
    delay(200);
  }

  cn = readFile(SPIFFS, "/cn.txt");
  sn = readFile(SPIFFS, "/sn.txt");
  hm = readFile(SPIFFS, "/hm.txt");
  brand = readFile(SPIFFS, "/brand.txt");
  http_username = readFile(SPIFFS, "/http_username.txt");
  http_password = readFile(SPIFFS, "/http_password.txt");
  ssid_wifi_ap = cn;
  ssid_wifi_ap =  readFile(SPIFFS, "/cn.txt");
  pass_wifi_ap =  readFile(SPIFFS, "/pass_wifi_ap.txt");
  sn_dev = readFile(SPIFFS, "/sn_dev.txt");
  en_lte = readFile(SPIFFS, "/en_lte.txt");
  no_simcard = readFile(SPIFFS, "/no_simcard.txt");
  en_mqtt = readFile(SPIFFS, "/en_mqtt.txt");
  broker = readFile(SPIFFS, "/broker.txt");
  username = readFile(SPIFFS, "/username.txt");
  password = readFile(SPIFFS, "/password.txt");
  interval_mqtt = readFile(SPIFFS, "/interval_mqtt.txt");
  server_ota = readFile(SPIFFS, "/server_ota.txt");
  resource_ota = readFile(SPIFFS, "/resource_ota.txt");
  xfirm_ver = readFile(SPIFFS, "/xfirm_ver.txt");
  xshift = readFile(SPIFFS, "/xshift.txt").toInt();

  en_write_running = readFile(SPIFFS, "/en_write_running.txt").toInt();
  en_write_idle = readFile(SPIFFS, "/en_write_idle.txt").toInt();
  en_write_off = readFile(SPIFFS, "/en_write_off.txt").toInt();

  engine_running = readFile(SPIFFS, "/engine_running.txt").toInt();;
  engine_idle = readFile(SPIFFS, "/engine_idle.txt").toInt();;
  engine_off = readFile(SPIFFS, "/engine_off.txt").toInt();;

  start_tanggal_running = readFile(SPIFFS, "/start_tanggal_running.txt").toInt();
  start_bulan_running = readFile(SPIFFS, "/start_bulan_running.txt").toInt();
  start_tahun_running = readFile(SPIFFS, "/start_tahun_running.txt").toInt();
  start_jam_running = readFile(SPIFFS, "/start_jam_running.txt").toInt();
  start_menit_running = readFile(SPIFFS, "/start_menit_running.txt").toInt();
  start_detik_running = readFile(SPIFFS, "/start_detik_running.txt").toInt();

  start_tanggal_idle = readFile(SPIFFS, "/start_tanggal_idle.txt").toInt();
  start_bulan_idle = readFile(SPIFFS, "/start_bulan_idle.txt").toInt();
  start_tahun_idle = readFile(SPIFFS, "/start_tahun_idle.txt").toInt();
  start_jam_idle = readFile(SPIFFS, "/start_jam_idle.txt").toInt();
  start_menit_idle = readFile(SPIFFS, "/start_menit_idle.txt").toInt();
  start_detik_idle = readFile(SPIFFS, "/start_detik_idle.txt").toInt();

  start_tanggal_off = readFile(SPIFFS, "/start_tanggal_off.txt").toInt();
  start_bulan_off = readFile(SPIFFS, "/start_bulan_off.txt").toInt();
  start_tahun_off = readFile(SPIFFS, "/start_tahun_off.txt").toInt();
  start_jam_off = readFile(SPIFFS, "/start_jam_off.txt").toInt();
  start_menit_off = readFile(SPIFFS, "/start_menit_off.txt").toInt();
  start_detik_off = readFile(SPIFFS, "/start_detik_off.txt").toInt();

  readFile(SPIFFS, "/datetime_start_running.txt").toCharArray(datetime_start_running, 25);
  readFile(SPIFFS, "/datetime_start_idle.txt").toCharArray(datetime_start_idle, 25);
  readFile(SPIFFS, "/datetime_start_off.txt").toCharArray(datetime_start_off, 25);
  readFile(SPIFFS, "/hm_unit.txt").toCharArray(hm_unit, 15);
  readFile(SPIFFS, "/hm_unit.txt").toCharArray(hm_realtime, 15);

  Serial.print("SPIFFS Free: "); Serial.println(humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes())));
  Serial.print("SPIFFS Used: "); Serial.println(humanReadableSize(SPIFFS.usedBytes()));
  Serial.print("SPIFFS Total: "); Serial.println(humanReadableSize(SPIFFS.totalBytes()));
}

String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    return String();
  }
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message)
{
  File file = fs.open(path, "w");
  if (!file)
  {
    return;
  }
  file.print(message);
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.print(F("Message appending : "));
    Serial.println(message);
    Serial.println("Message appended");
  }
  else {
    Serial.println("Append failed");
  }
  file.close();
}

String humanReadableSize(const size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void notFoundd(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String processor(const String& var)
{
  if (var == "version")
  {
    return version;
  }
  else if (var == "type_board")
  {
    return type_board;
  }
  else if (var == "sn_dev")
  {
    return readFile(SPIFFS, "/sn_dev.txt");
  }
  else if (var == "cn")
  {
    return readFile(SPIFFS, "/cn.txt");
  }
  else if (var == "sn")
  {
    return readFile(SPIFFS, "/sn.txt");
  }
  else if (var == "hm_hour")
  {
    return String(hour_realtime);
  }
  else if (var == "hm_minute")
  {
    return String(minute_realtime);
  }
  else if (var == "hm_second")
  {
    return String(second_realtime);
  }
  else if (var == "brand")
  {
    return readFile(SPIFFS, "/brand.txt");
  }
  else if (var == "set_time")
  {
    return times;
  }
  else if (var == "set_date")
  {
    return dates;
  }
  else if (var == "en_lte")
  {
    return en_lte;
  }
  else if (var == "no_simcard")
  {
    return no_simcard;
  }
  else if (var == "selected_enablelte")
  {
    String x = readFile(SPIFFS, "/en_lte.txt");

    if (x == "true")
    {
      return "selected";
    }
    else if (x == "false")
    {
      return " ";
    }
  }
  else if (var == "selected_disablelte")
  {
    String x = readFile(SPIFFS, "/en_lte.txt");

    if (x == "true")
    {
      return " ";
    }
    else if (x == "false")
    {
      return "selected";
    }
  }
  else if (var == "en_mqtt")
  {
    return en_mqtt;
  }
  else if (var == "selected_enablemqtt")
  {
    String x = readFile(SPIFFS, "/en_mqtt.txt");

    if (x == "true")
    {
      return "selected";
    }
    else if (x == "false")
    {
      return " ";
    }
  }
  else if (var == "selected_disablemqtt")
  {
    String x = readFile(SPIFFS, "/en_mqtt.txt");

    if (x == "true")
    {
      return " ";
    }
    else if (x == "false")
    {
      return "selected";
    }
  }
  else if (var == "mqtt_state")
  {
    return mqtt_state;
  }
  else if (var == "broker")
  {
    return broker;
  }
  else if (var == "username")
  {
    return username;
  }
  else if (var == "password")
  {
    return password;
  }
  else if (var == "interval_mqtt")
  {
    return readFile(SPIFFS, "/interval_mqtt.txt");
  }
  else if (var == "server_ota")
  {
    return readFile(SPIFFS, "/server_ota.txt");
  }
  else if (var == "resource_ota")
  {
    return readFile(SPIFFS, "/resource_ota.txt");
  }
  else if (var == "freespace") {
    int xfreesp = SD.totalBytes() / (1024 * 1024);
    freesp = String(xfreesp) + " MB";
    return freesp;
  }
  else if (var == "usedstorage") {
    int xusedstr = SD.usedBytes() / (1024 * 1024);
    usedstr = String(xusedstr) + " MB";
    return usedstr;
  }
  else if (var == "totalStorage") {
    int xtotalstr = SD.cardSize() / (1024 * 1024);
    totalstr = String(xtotalstr) + " MB";
    return totalstr;
  }
  else
  {
    return String();
  }
}

void server_begin()
{
  server.serveStatic("/", SPIFFS, "/");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    read_setting();
    request->send(SPIFFS, "/dashboard_ws.html", String(), false, processor);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(401);
  });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(401);
  });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    Serial.println("================");
    Serial.println("Device Restart");
    Serial.println("================");
    delay(3000);
    ESP.restart();
    request->send(SPIFFS, "/dashboard_ws.html", String(), false, processor);
  });

  server.on("/default", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    Serial.println();
    Serial.println("__________________RESET DEFAULT_______________________");
    Serial.println();
    delay(2000);
    data_default();
    delay(200);
    ESP.restart();
    request->send(SPIFFS, "/dashboard_ws.html", String(), false, processor);
  });

  server.on("/push_traccar", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    Serial.println();
    Serial.println("__________________traccar_test_______________________");
    Serial.println();
    request->send(SPIFFS, "/dashboard_ws.html", String(), false, processor);
  });

  server.on("/publish", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    Serial.println();
    Serial.println("__________________mqtt_test_______________________");
    Serial.println();
    request->send(SPIFFS, "/dashboard_ws.html", String(), false, processor);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request)
  {
    if (!request->authenticate(http_username.c_str(), http_password.c_str()))
      return request->requestAuthentication();
    read_setting();
    request->redirect("/");
  });

  server.onNotFound(notFoundd);
  server.begin();
}
